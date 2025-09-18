#include "nose.h"

QueueHandle_t xSensorQueue;  //传感器数据队列

uint16_t Nose_Value[CAHNNELS];   // 原始ADC值
float Nose_Vout[CAHNNELS];       // 转换后的电压值

extern uint8_t start_flag;   //START/STOP 控制

// 启动DMA采集
void Nose_Read_Value(void)
{
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)Nose_Value,CAHNNELS);
}


// 计算各通道电压
void Get_Vout(void)
{
	uint8_t i;
	for(i = 0; i < CAHNNELS; i++)
	{
		Nose_Vout[i] = (Nose_Value[i] / ADC_RESOLUTION) * VREF;
		
	}
}


// 计算传感器电阻RS
float Get_RS(float Vout,float VC,float RL)
{
	if(Vout < 0.01f) return -1; // 避免除零
	return RL * (VC - Vout) / Vout;
	
}


// ====== 气体浓度计算（需要标定，这里仅给出示例）======

// 酒精传感器 GM-302C
float CalcAlcoholPPM(float Vout)
{
    float RL = 10e3;   // 负载电阻 (10kΩ)
    float VC = 3.3f;   // 回路电压
    float RS = Get_RS(Vout, VC, RL);

    // 假设已标定，设定 R0 = RS(空气)，系数 a、b 拟合得到
    float R0 = 20e3;
    float a = -0.45f, b = 1.2f; // 示例参数，需实验标定
    float ratio = RS / R0;

    float logC = (log10f(ratio) - b) / a;
    return powf(10, logC);  // ppm
}

// VOC 传感器 GM-502B
float CalcVocPPM(float Vout)
{
    float RL = 10e3;
    float VC = 5.0f;
    float RS = Get_RS(Vout, VC, RL);

    float R0 = 25e3;
    float a = -0.38f, b = 1.1f; // 示例参数，需实验标定
    float ratio = RS / R0;

    float logC = (log10f(ratio) - b) / a;
    return powf(10, logC);
}

// ========== FreeRTOS 传感器任务 ==========
void SensorTask(void *argument)
{
    SensorData_t data;

    for(;;)
    {
        if(start_flag)
        {
            Nose_Read_Value();
            Get_Vout();

            data.alcohol_ppm = CalcAlcoholPPM(Nose_Vout[0]);
            data.voc_ppm     = CalcVocPPM(Nose_Vout[1]);

            xQueueSend(xSensorQueue, &data, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // 每1秒采集一次
    }
}