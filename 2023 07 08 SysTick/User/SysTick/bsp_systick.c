#include "bsp_systick.h"

static __IO u32 TimingDelay;	//可读写的32位静态变量	

void SysTick_Init(void)			//SysTick 初始化函数
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if(SysTick_Config(SystemCoreClock / 1000))	//设置时间间隔为系统内部时钟的1/1000，即1ms
	{
		while(1);
	}
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭ctrl寄存器
}

/**
  * @brief   ms延时程序,10us为一个单位
  * @param  
  *		@arg
  * @retval  无
  */
void Delay_ms(__IO u32 nTime)
{ 
	TimingDelay = nTime;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;		//开启ctrl寄存器	
	while(TimingDelay != 0);
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭寄存器
}

/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
