#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"


void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	LED_GPIO_Config();
	
	while (1)
	{
//		GPIO_ResetBits(GPIOF, GPIO_Pin_6);
//		Delay(0xffffff);
//		GPIO_SetBits(GPIOF, GPIO_Pin_6);
//		Delay(0xffffff);
	}
}

