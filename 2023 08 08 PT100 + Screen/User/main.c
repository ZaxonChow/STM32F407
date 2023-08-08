#include "stm32f4xx.h"
#include "../system/Delay.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./key/bsp_key.h"
#include "./screen/screen.h"

#define   	  AT	"AT\r\n"
#define 	AT_T 	"AT+T\r\n"

int main(void)
{	
	/* ��ʼ��USART1 - ������ */
	Debug_USART1_Config();
	/* ��ʼ��USART6 - PT100 */
	Debug_USART6_Config();
	
	/* USART6 ��PT100����һ��ATָ�� */
	Usart_SendString(USART6, AT_T);
		
	TJCPrintf("\x00");          //Ϊȷ������HMI����ͨ��
	
	while(1)
	{
		Screen();
	}
}

