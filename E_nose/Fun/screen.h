/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SCREEN_H
#define __SCREEN_H

#include "headfile.h"
#include "CoreData.h"

extern QueueHandle_t xCmdQueue;    // �������������

void UartScreenTask(void *argument);

/****
����ָ�� ����ͨ����Ϣ���н��д���

�����ַ���ƥ��
****/
void ParseCommand(char *cmd);


#endif /* __MAIN_H */