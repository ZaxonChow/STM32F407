#include "stm32f4xx.h"
#include "../system/Delay.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./485/bsp_485.h"
#include "./key/bsp_key.h"
#include "./timer/bsp_timer.h"
#include "./modbus_rtu/bsp_master.h"

extern uint8_t RS485_RxFlag;

int main(void)
{	
	LED_GPIO_Config();	
	
	Key_GPIO_Config();

	/* 初始化USART1 */
	Debug_USART_Config();
	
	/* 初始化 485 使用的串口 USART2，使用中断模式接收 */
	RS485_Config();
	
	Timer4_enable(5000);
		
	while(1)
	{
		if(RS485_RxFlag == 1)
		{
			LED_GREEN;
			//printf("\r\n 数据接收成功 \r\n");
			Receive_Service();
		}
		else
		{
			LED_BLUE;
		}
	}
}

