/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NOSE_H
#define __NOSE_H

#include "headfile.h"
#include "CoreData.h"

#define CAHNNELS	4
// ADC �ֱ��� & �ο���ѹ
#define ADC_RESOLUTION 4095.0f
#define VREF           3.3f

extern QueueHandle_t xSensorQueue;  //���������ݶ���
void Nose_Read_Value(void);
// �����ͨ����ѹ
void Get_Vout(void);

// ���㴫��������RS
float Get_RS(float Vout,float VC,float RL);
// ====== ����Ũ�ȼ��㣨��Ҫ�궨�����������ʾ����======

// �ƾ������� GM-302C
float CalcAlcoholPPM(float Vout);
// VOC ������ GM-502B
float CalcVocPPM(float Vout);
// ========== FreeRTOS ���������� ==========
void SensorTask(void *argument);
#endif /* __MAIN_H */