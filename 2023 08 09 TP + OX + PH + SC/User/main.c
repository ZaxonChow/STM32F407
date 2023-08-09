#include "stm32f4xx.h"
#include "../system/Delay.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./key/bsp_key.h"
#include "./screen/screen.h"
#include "./485/bsp_485.h"
#include "./timer/bsp_timer.h"
#include "./modbus_rtu/bsp_master.h"

#define   	  AT	"AT\r\n"
#define 	AT_T 	"AT+T\r\n"

int main(void)
{	
	/* 初始化USART1 - 串口屏 */
	Debug_USART1_Config();
	/* 初始化USART6 - PT100 */
	Debug_USART6_Config();
	/* 初始化 485 使用的串口 USART2，使用中断模式接收 */
	RS485_Config();
	/* 使能定时器 4，定时5ms */
	Timer4_enable(5000);
	
	/* USART6 给PT100发送一次AT指令 */
	Usart_SendString(USART6, AT_T);
		
	TJCPrintf("\x00");          //为确保串口HMI正常通信
	
	while(1)
	{
		Screen();
	}
}

