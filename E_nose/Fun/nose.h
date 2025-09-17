/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NOSE_H
#define __NOSE_H

#include "headfile.h"

#define CAHNNELS	4  // 电子鼻读取通道
#define VREF		3.3f //参考电压

extern float Nose_Vout[CAHNNELS];


// DMA 多通道读取ADC值
void Nose_Read_Value(void);

// 获取电压值
void Get_Vout(void);

// 计算传感器电阻 RS
// 浓度转换公式 log10(C) = (log10(RS/R0) - b) / a
// 这里的Vc应该是和我给传感器的供电值 3.3v一致
float Get_RS(float Vout,float VC,float RL);
#endif /* __MAIN_H */