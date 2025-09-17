#include "nose.h"

// ��ŵ��ӱǵĶ�ȡ����
uint16_t Nose_Value[CAHNNELS];

// ���ת���õĶ�ȡ����
float Nose_Vout[CAHNNELS];


// DMA ��ͨ����ȡADCֵ
void Nose_Read_Value(void)
{
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)Nose_Value,sizeof(Nose_Value) / sizeof(Nose_Value[0]));
}

// ��ȡ��ѹֵ
void Get_Vout(void)
{
	uint8_t i;

	for(i = 0; i < CAHNNELS;i++)
	{
		Nose_Vout[i] = (Nose_Value[i] / 4095.09f) * VREF;
	}
}

// ���㴫�������� RS
// Ũ��ת����ʽ log10(C) = (log10(RS/R0) - b) / a
// �����VcӦ���Ǻ��Ҹ��������Ĺ���ֵ 3.3vһ��
float Get_RS(float Vout,float VC,float RL)
{
	if(Vout < 0.01f) return -1; // ��Чֵ ���㱣��
	return RL * (VC - Vout) / Vout;
}