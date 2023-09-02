#include "./tim/bsp_advance_tim.h"

__IO uint16_t ChannelPulse = 127;

static void TIMx_GPIO_Config(void) 
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启定时器相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd(ADVANCE_OCPWM_GPIO_CLK | ADVANCE_OCNPWM_GPIO_CLK| ADVANCE_BKIN_GPIO_CLK, ENABLE); 
	/* 指定引脚复用功能 */
	GPIO_PinAFConfig(ADVANCE_OCPWM_GPIO_PORT, ADVANCE_OCPWM_PINSOURCE, ADVANCE_OCPWM_AF); 
	GPIO_PinAFConfig(ADVANCE_OCNPWM_GPIO_PORT, ADVANCE_OCNPWM_PINSOURCE, ADVANCE_OCNPWM_AF); 
	GPIO_PinAFConfig(ADVANCE_BKIN_GPIO_PORT, ADVANCE_BKIN_PINSOURCE, ADVANCE_BKIN_AF); 
	
	/* 定时器功能引脚初始化 */															   
	GPIO_InitStructure.GPIO_Pin = ADVANCE_OCPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(ADVANCE_OCPWM_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADVANCE_OCNPWM_PIN;	
	GPIO_Init(ADVANCE_OCNPWM_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADVANCE_BKIN_PIN;	
	GPIO_Init(ADVANCE_BKIN_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef 结构体里面有5个成员，TIM6 和 TIM7 的寄存器里面只有
 * TIM_Prescaler 和 TIM_Period，所以使用 TIM6 和 TIM7 的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode		 TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

	// 开启TIMx_CLK,x[1,8]，即内部时钟CK_INT
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CLK, ENABLE); 

	/* 累计 TIM_Period 个后产生一个更新或者中断*/		
	// 当定时器从 0 计数到 1023，即为 1024 次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Period = 1024 - 1;
	// 高级控制定时器时钟源 TIMxCLK = HCLK = 168MHz 
	// 设定定时器频率为 = TIMxCLK / (TIM_Prescaler + 1) = 100kHz
	TIM_TimeBaseStructure.TIM_Prescaler = 1680 - 1;	
	// 采样时钟分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	// 计数方式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// 重复计数器
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	
	// 初始化定时器TIMx, x[1,8]
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

	/*PWM模式配置*/
	// 配置为 PWM 模式1，先树池高电平，达到比较值的时候再改变电平
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 主输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	// 互补输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	// 设置占空比大小
	TIM_OCInitStructure.TIM_Pulse = ChannelPulse;
	// 主输出高电平有效
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 互补输出高电平有效
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	// 主输出在被禁止时为高电平
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	// 互补输出在被禁止时为低电平
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	// 使能通道1
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);	 

	/* 使能通道1重载 */
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
		
	/* 自动输出使能，断路、死区时间和锁定配置 */
	// 有关刹车和死区结构体的成员具体可参考 BDTR 寄存器描述
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
	// 输出比较信号死区时间配置，具体如何计算可参考 BDTR:UTG[7:0] 的描述
	TIM_BDTRInitStructure.TIM_DeadTime = 11;
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	// 当 BKIN 引脚检测到高电平的时候，输出比较信号被禁止，就好像是刹车一样
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);

	// 使能定时器
	TIM_Cmd(ADVANCE_TIM, ENABLE);	

	/* 主动输出使能 */
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

void TIMx_Configuration(void)
{
	TIMx_GPIO_Config();
	
	TIM_Mode_Config();
}
