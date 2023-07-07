#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"
#include "./rcc/bsp_rcc.h"

void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	//User_SetSysClock();
	
	//�������ⲿ������25M,�޸ĵ�һ���β�
	//��Ƶ�޸ĵڶ����β�
	//HSE_SetSysClock(25, 336, 2, 7);
	
	HSI_SetSysClock(16, 336, 2, 7);
	
	LED_GPIO_Config();
//	MCO1_GPIO_Config();
//	MCO2_GPIO_Config();
//	
//	//MCO1/2�ϲ鿴���໷/ϵͳ�ϵ�ʱ���ź�Ƶ��
//	RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_1);
//	RCC_MCO2Config(RCC_MCO2Source_SYSCLK, RCC_MCO2Div_1);
	
	while (1)
	{
		LED1(ON);
		Delay(0x3FFFFF);
		LED1(OFF);
		Delay(0x3FFFFF);
	}
}

