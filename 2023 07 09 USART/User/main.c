#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"
#include "bsp_usart.h"

void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	Debug_USART_Config();
	//发送一个字符 a
	Usart_SendByte(DEBUG_USART, 'a');
	//发送一个字符串
	Usart_SendString(DEBUG_USART, "\n这是一个串口中断接收回显实验\n");
	printf("这是一个串口中断接收回显实验\n");
	
	while (1)
	{

	}
}

