#include "nose.h"

QueueHandle_t xSensorQueue;  //���������ݶ���

uint16_t Nose_Value[CAHNNELS];   // ԭʼADCֵ
float Nose_Vout[CAHNNELS];       // ת����ĵ�ѹֵ

extern uint8_t start_flag;   //START/STOP ����

// ����DMA�ɼ�
void Nose_Read_Value(void)
{
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)Nose_Value,CAHNNELS);
}


// �����ͨ����ѹ
void Get_Vout(void)
{
	uint8_t i;
	for(i = 0; i < CAHNNELS; i++)
	{
		Nose_Vout[i] = (Nose_Value[i] / ADC_RESOLUTION) * VREF;
		
	}
}


// ���㴫��������RS
float Get_RS(float Vout,float VC,float RL)
{
	if(Vout < 0.01f) return -1; // �������
	return RL * (VC - Vout) / Vout;
	
}


// ====== ����Ũ�ȼ��㣨��Ҫ�궨�����������ʾ����======

// �ƾ������� GM-302C
float CalcAlcoholPPM(float Vout)
{
    float RL = 10e3;   // ���ص��� (10k��)
    float VC = 3.3f;   // ��·��ѹ
    float RS = Get_RS(Vout, VC, RL);

    // �����ѱ궨���趨 R0 = RS(����)��ϵ�� a��b ��ϵõ�
    float R0 = 20e3;
    float a = -0.45f, b = 1.2f; // ʾ����������ʵ��궨
    float ratio = RS / R0;

    float logC = (log10f(ratio) - b) / a;
    return powf(10, logC);  // ppm
}

// VOC ������ GM-502B
float CalcVocPPM(float Vout)
{
    float RL = 10e3;
    float VC = 5.0f;
    float RS = Get_RS(Vout, VC, RL);

    float R0 = 25e3;
    float a = -0.38f, b = 1.1f; // ʾ����������ʵ��궨
    float ratio = RS / R0;

    float logC = (log10f(ratio) - b) / a;
    return powf(10, logC);
}

// ========== FreeRTOS ���������� ==========
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
        vTaskDelay(pdMS_TO_TICKS(1000)); // ÿ1��ɼ�һ��
    }
}