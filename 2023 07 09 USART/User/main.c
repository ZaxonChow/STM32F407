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
	//����һ���ַ� a
	Usart_SendByte(DEBUG_USART, 'a');
	//����һ���ַ���
	Usart_SendString(DEBUG_USART, "\n����һ�������жϽ��ջ���ʵ��\n");
	printf("����һ�������жϽ��ջ���ʵ��\n");
	
	while (1)
	{

	}
}

