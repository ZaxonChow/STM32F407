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
	/* ��ʼ��USART1 - ������ */
	Debug_USART1_Config();
	/* ��ʼ��USART6 - PT100 */
	Debug_USART6_Config();
	/* ��ʼ�� 485 ʹ�õĴ��� USART2��ʹ���ж�ģʽ���� */
	RS485_Config();
	/* ʹ�ܶ�ʱ�� 4����ʱ5ms */
	Timer4_enable(5000);
	
	/* USART6 ��PT100����һ��ATָ�� */
	Usart_SendString(USART6, AT_T);
		
	TJCPrintf("\x00");          //Ϊȷ������HMI����ͨ��
	
	while(1)
	{
		Screen();
	}
}

