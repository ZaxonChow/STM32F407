#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"
#include "bsp_usart.h"

//static void Show_Message(void);
#define   	  AT	"AT\r\n"
#define 	AT_T 	"AT+T\r\n"

//�����¶�����
char Get_Temperature[20];
extern uint8_t flag;

void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	uint8_t i = 0;
	
	/* ��ʼ��USART1 ����ģʽΪ 9600 8-N-1 */
	Debug_USART1_Config();
	/* ��ʼ��USART2 ����ģʽΪ 9600 8-N-1 */
	Debug_USART2_Config();	
	
	//USART2 ���ʹ�����ATָ��
	Usart_SendString(USART2, AT_T);
	
	while (1)
	{
		if(flag == 1)
		{
			while(Get_Temperature[i] != '\0')
			{
				Usart_SendByte(USART1, Get_Temperature[i]);
				i++;
			}
			i = 0;
			flag = 0;
			Usart_SendString(USART2, AT_T);
		}	
	}
}


