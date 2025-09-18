#include "screen.h"

uint8_t Screen_Data;
char Screen_Buf[128];
uint8_t idx = 0;


QueueHandle_t xCmdQueue;    // �������������

uint8_t start_flag;   //��ʼ��־λ
/****
Screen Command ����������ִ��

ʹ�ô���Ϊ Uart3
****/
void UartScreenTask(void *argument)
{
	SensorData_t rxData;
	CmdMsg_t cmd;
	
	for(;;)
	{
		// 1 ���ʹ��������ݸ�������
		if(xQueueReceive(xSensorQueue,&rxData,0) == pdPASS)
		{
			char msg[64];
			sprintf(msg,"Alcohol=%.2f,VOC=%.2f\n",rxData.alcohol_ppm,rxData.voc_ppm);
			HAL_UART_Transmit(&huart3,(uint8_t *)msg,strlen(msg),100);
			
		}
		
		// 2 ������Ļ����������
		if(xQueueReceive(xCmdQueue,&cmd,0) == pdPASS)
		{
			switch(cmd.type)
			{
				case CMD_START: start_flag = 1; break;
				case CMD_STOP: start_flag = 0; break;
				case CMD_WIFI:
					;   // ��������wifi
				break;
			}
			
		}
		
		vTaskDelay(pdMS_TO_TICKS(200)); //�������̬ 200Ticks
	}
	
	
}

/****
����ָ�� ����ͨ����Ϣ���н��д���

�����ַ���ƥ��
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


// �ַ����жϽ���
/***
���жϽ��պ��� --->
1.���Ͻ����ַ�
2.�ж���ȫ���պ� ---> ParseCommand���� --->���͵�Cmd��Ϣ������
***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        if(Screen_Data == '\n')
        {
            Screen_Buf[idx] = '\0';
            ParseCommand(Screen_Buf);  // ��������Ž� xCmdQueue
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





