#include "stm32f4xx.h"                  // Device header

//头文件包含方法
//#include "./led/bsp_led.h"
#include "bsp_led.h"
#include "bsp_key.h"


void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	LED_GPIO_Config();
	
	KEY_GPIO_Config();
	
	while (1)
	{
		if(KEY_Scan(GPIOA, GPIO_Pin_0) == KEY_ON)
		{
			GPIO_ToggleBits(GPIOF, GPIO_Pin_6);
			
		}
		if(KEY_Scan(GPIOC, GPIO_Pin_13) == KEY_ON)
		{
			GPIO_ToggleBits(GPIOF, GPIO_Pin_7);
			
		}
	}
}

