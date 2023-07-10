#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"
#include "bsp_usart.h"

extern uint8_t SendBuff[SENDBUFF_SIZE]; 

void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	uint16_t i;
	/* ��ʼ��USART */
	Debug_USART_Config(); 

	/* ����ʹ��DMAģʽ */
	USART_DMA_Config();

	/* ����RGB��ɫ�� */
	LED_GPIO_Config();

	printf("\r\n USART1 DMA TX ���� \r\n");

	/*��佫Ҫ���͵�����*/
	for(i=0;i<SENDBUFF_SIZE;i++)
	{
		SendBuff[i]	 = 'A';
	}

	/*Ϊ��ʾDMA�������ж�CPU���ܴ����������飬����ʹ��DMA�������ݣ����ǳ���
	*��ʱ�����п��ܻᵼ�µ��Զ˴��ڵ������ֻῨ��������ҷɵ������
	*���DMA�����е�ѭ��ģʽ��Ϊ����ģʽ*/		

	/* USART1 �� DMA����TX���� */
	USART_DMACmd(DEBUG_USART, USART_DMAReq_Tx, ENABLE);

	/* ��ʱCPU�ǿ��еģ����Ը����������� */  
	//����ͬʱ����LED
	while(1)
	{
		LED1_TOGGLE;
		Delay(0xFFFFFF);
	}
}

