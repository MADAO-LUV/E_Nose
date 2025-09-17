#include "nose.h"

// 存放电子鼻的读取数据
uint16_t Nose_Value[CAHNNELS];

// 存放转换好的读取数据
float Nose_Vout[CAHNNELS];


// DMA 多通道读取ADC值
void Nose_Read_Value(void)
{
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)Nose_Value,sizeof(Nose_Value) / sizeof(Nose_Value[0]));
}

// 获取电压值
void Get_Vout(void)
{
	uint8_t i;

	for(i = 0; i < CAHNNELS;i++)
	{
		Nose_Vout[i] = (Nose_Value[i] / 4095.09f) * VREF;
	}
}

// 计算传感器电阻 RS
// 浓度转换公式 log10(C) = (log10(RS/R0) - b) / a
// 这里的Vc应该是和我给传感器的供电值 3.3v一致
float Get_RS(float Vout,float VC,float RL)
{
	if(Vout < 0.01f) return -1; // 无效值 除零保护
	return RL * (VC - Vout) / Vout;
}