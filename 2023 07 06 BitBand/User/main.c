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

	}
}

