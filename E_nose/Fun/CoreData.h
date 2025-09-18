/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COREDATA_H
#define __COREDATA_H

#include "headfile.h"



// ���������� �ṹ��
typedef struct
{
	float alcohol_ppm; //�ƾ�Ũ��
	float voc_ppm;     //voc�л���Ũ��
}SensorData_t;

// ����ö��
typedef enum {
    CMD_START,
    CMD_STOP,
    CMD_WIFI
} CmdType_t;

// ������Ϣ
typedef struct {
    CmdType_t type;
    char ssid[32]; //���񼯱�ʶ��
    char pass[32];
} CmdMsg_t;

#endif /* __MAIN_H */