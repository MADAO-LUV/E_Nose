/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NOSE_H
#define __NOSE_H

#include "headfile.h"
#include "CoreData.h"

#define CAHNNELS	4
// ADC 分辨率 & 参考电压
#define ADC_RESOLUTION 4095.0f
#define VREF           3.3f

extern QueueHandle_t xSensorQueue;  //传感器数据队列
void Nose_Read_Value(void);
// 计算各通道电压
void Get_Vout(void);

// 计算传感器电阻RS
float Get_RS(float Vout,float VC,float RL);
// ====== 气体浓度计算（需要标定，这里仅给出示例）======

// 酒精传感器 GM-302C
float CalcAlcoholPPM(float Vout);
// VOC 传感器 GM-502B
float CalcVocPPM(float Vout);
// ========== FreeRTOS 传感器任务 ==========
void SensorTask(void *argument);
#endif /* __MAIN_H */