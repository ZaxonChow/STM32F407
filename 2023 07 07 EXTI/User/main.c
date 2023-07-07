#include "stm32f4xx.h"                  // Device header

//头文件包含方法
//#include "./led/bsp_led.h"
#include "bsp_led.h"
#include "./key/bsp_exti.h"


void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	LED_GPIO_Config();
	EXTI_Key_Config();
	while (1)
	{
		
	}
}

