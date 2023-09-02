#include "stm32f4xx.h"                  // Device header
#include "bsp_key.h"
#include "./tim/bsp_advance_tim.h"

extern __IO uint16_t ChannelPulse;

int main(void)
{
	KEY_GPIO_Config();
	
	/* 初始化高级控制定时器，设置PWM模式，使能通道1互补输出 */
	TIMx_Configuration();
	while (1)
	{
		/* 扫描KEY1 */
		if(KEY_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON  )
		{
			/* 增大占空比 */
			if(ChannelPulse < 960)
				ChannelPulse += 64;
			else
				ChannelPulse = 1024;
			TIM_SetCompare1(ADVANCE_TIM, ChannelPulse);
		}   
		/* 扫描KEY2 */
		if(KEY_Scan(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON  )
		{
			/* 减小占空比 */
			if(ChannelPulse >= 64)
				ChannelPulse -= 64;
			else
				ChannelPulse = 0;
			TIM_SetCompare1(ADVANCE_TIM, ChannelPulse);
		}  
	}
}

