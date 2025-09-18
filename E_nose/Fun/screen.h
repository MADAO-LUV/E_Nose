/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SCREEN_H
#define __SCREEN_H

#include "headfile.h"
#include "CoreData.h"

extern QueueHandle_t xCmdQueue;    // 串口屏命令队列

void UartScreenTask(void *argument);

/****
解析指令 并且通过消息队列进行传输

进行字符串匹配
****/
void ParseCommand(char *cmd);


#endif /* __MAIN_H */