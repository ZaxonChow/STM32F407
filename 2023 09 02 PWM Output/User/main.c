#include "stm32f4xx.h"                  // Device header
#include "bsp_key.h"
#include "./tim/bsp_advance_tim.h"

extern __IO uint16_t ChannelPulse;

int main(void)
{
	KEY_GPIO_Config();
	
	/* ��ʼ���߼����ƶ�ʱ��������PWMģʽ��ʹ��ͨ��1������� */
	TIMx_Configuration();
	while (1)
	{
		/* ɨ��KEY1 */
		if(KEY_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON  )
		{
			/* ����ռ�ձ� */
			if(ChannelPulse < 960)
				ChannelPulse += 64;
			else
				ChannelPulse = 1024;
			TIM_SetCompare1(ADVANCE_TIM, ChannelPulse);
		}   
		/* ɨ��KEY2 */
		if(KEY_Scan(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON  )
		{
			/* ��Сռ�ձ� */
			if(ChannelPulse >= 64)
				ChannelPulse -= 64;
			else
				ChannelPulse = 0;
			TIM_SetCompare1(ADVANCE_TIM, ChannelPulse);
		}  
	}
}

