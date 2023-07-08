#include "bsp_systick.h"

static __IO u32 TimingDelay;	//�ɶ�д��32λ��̬����	

void SysTick_Init(void)			//SysTick ��ʼ������
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if(SysTick_Config(SystemCoreClock / 1000))	//����ʱ����Ϊϵͳ�ڲ�ʱ�ӵ�1/1000����1ms
	{
		while(1);
	}
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�ر�ctrl�Ĵ���
}

/**
  * @brief   ms��ʱ����,10usΪһ����λ
  * @param  
  *		@arg
  * @retval  ��
  */
void Delay_ms(__IO u32 nTime)
{ 
	TimingDelay = nTime;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;		//����ctrl�Ĵ���	
	while(TimingDelay != 0);
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//�رռĴ���
}

/**
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  �� SysTick �жϺ��� SysTick_Handler()����
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
