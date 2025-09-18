/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COREDATA_H
#define __COREDATA_H

#include "headfile.h"



// 传感器数据 结构体
typedef struct
{
	float alcohol_ppm; //酒精浓度
	float voc_ppm;     //voc有机物浓度
}SensorData_t;

// 命令枚举
typedef enum {
    CMD_START,
    CMD_STOP,
    CMD_WIFI
} CmdType_t;

// 命令信息
typedef struct {
    CmdType_t type;
    char ssid[32]; //服务集标识符
    char pass[32];
} CmdMsg_t;

#endif /* __MAIN_H */