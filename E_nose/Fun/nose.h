/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NOSE_H
#define __NOSE_H

#include "headfile.h"

#define CAHNNELS	4  // ���ӱǶ�ȡͨ��
#define VREF		3.3f //�ο���ѹ

extern float Nose_Vout[CAHNNELS];


// DMA ��ͨ����ȡADCֵ
void Nose_Read_Value(void);

// ��ȡ��ѹֵ
void Get_Vout(void);

// ���㴫�������� RS
// Ũ��ת����ʽ log10(C) = (log10(RS/R0) - b) / a
// �����VcӦ���Ǻ��Ҹ��������Ĺ���ֵ 3.3vһ��
float Get_RS(float Vout,float VC,float RL);
#endif /* __MAIN_H */