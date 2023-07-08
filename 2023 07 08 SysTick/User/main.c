#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"
#include "bsp_systick.h"

void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	LED_GPIO_Config();
	
	SysTick_Init();
	
	while (1)
	{
		LED1(ON);
		Delay_ms(1000);
		LED1(OFF);
		Delay_ms(1000);
	}
}

