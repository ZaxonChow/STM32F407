#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"

/* 求出位带别名区地址 */
#define BITBAND(addr, bitnum)	((addr & 0xF0000000) + 0x2000000 + ((addr & 0x000FFFFF) << 5) + (bitnum << 2))

/* 对地址进行指针操作 */
#define MEM(addr)	(*(volatile unsigned long*)(addr))
	
#define BIT_ADDR(addr, bitnum)	MEM(BITBAND(addr, bitnum))

#define GPIOF_ODR_ADDR	(GPIOF_BASE + 0x14)

#define PFout(n)	BIT_ADDR(GPIOF_ODR_ADDR, n)	



void Delay( uint32_t count)
{
	for(; count!=0; count--);
}



int main(void)
{
	LED_GPIO_Config();
	
	while (1)
	{
		PFout(6) = 0;
		Delay(0xFFFFFF);
		
		PFout(6) = 1;
		Delay(0xFFFFFF);
	}
}

