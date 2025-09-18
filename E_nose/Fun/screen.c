#include "screen.h"

uint8_t Screen_Data;
char Screen_Buf[128];
uint8_t idx = 0;


QueueHandle_t xCmdQueue;    // 串口屏命令队列

uint8_t start_flag;   //开始标志位
/****
Screen Command 串口屏任务执行

使用串口为 Uart3
****/
void UartScreenTask(void *argument)
{
	SensorData_t rxData;
	CmdMsg_t cmd;
	
	for(;;)
	{
		// 1 发送传感器数据给串口屏
		if(xQueueReceive(xSensorQueue,&rxData,0) == pdPASS)
		{
			char msg[64];
			sprintf(msg,"Alcohol=%.2f,VOC=%.2f\n",rxData.alcohol_ppm,rxData.voc_ppm);
			HAL_UART_Transmit(&huart3,(uint8_t *)msg,strlen(msg),100);
			
		}
		
		// 2 处理屏幕发来的命令
		if(xQueueReceive(xCmdQueue,&cmd,0) == pdPASS)
		{
			switch(cmd.type)
			{
				case CMD_START: start_flag = 1; break;
				case CMD_STOP: start_flag = 0; break;
				case CMD_WIFI:
					;   // 这里配置wifi
				break;
			}
			
		}
		
		vTaskDelay(pdMS_TO_TICKS(200)); //进入挂起态 200Ticks
	}
	
	
}

/****
解析指令 并且通过消息队列进行传输

进行字符串匹配
****/
void ParseCommand(char *cmd)
{
    CmdMsg_t msg;
    if(strncmp(cmd, "START", 5) == 0) {
        msg.type = CMD_START;
        xQueueSend(xCmdQueue, &msg, 0);
    } else if(strncmp(cmd, "STOP", 4) == 0) {
        msg.type = CMD_STOP;
        xQueueSend(xCmdQueue, &msg, 0);
    } else if(strncmp(cmd, "WIFI:", 5) == 0) {
        msg.type = CMD_WIFI;
        sscanf(cmd, "WIFI:SSID=%[^,],PASS=%s", msg.ssid, msg.pass);
        xQueueSend(xCmdQueue, &msg, 0);
    }
}


// 字符串中断接收
/***
该中断接收函数 --->
1.不断接收字符
2.判定完全接收后 ---> ParseCommand处理 --->发送到Cmd消息队列中
***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        if(Screen_Data == '\n')
        {
            Screen_Buf[idx] = '\0';
            ParseCommand(Screen_Buf);  // 解析命令，放进 xCmdQueue
            idx = 0;
        }
        else
        {
            Screen_Buf[idx++] = Screen_Data;
            if(idx >= sizeof(Screen_Buf)) idx = 0;
        }
        HAL_UART_Receive_IT(&huart1, &Screen_Data, 1);
    }
}





