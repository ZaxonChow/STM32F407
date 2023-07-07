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
	
	//开发板外部晶振不是25M,修改第一个形参
	//超频修改第二个形参
	//HSE_SetSysClock(25, 336, 2, 7);
	
	HSI_SetSysClock(16, 336, 2, 7);
	
	LED_GPIO_Config();
//	MCO1_GPIO_Config();
//	MCO2_GPIO_Config();
//	
//	//MCO1/2上查看锁相环/系统上的时钟信号频率
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

