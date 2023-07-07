#include "bsp_exti.h"

static void NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;	//定义NVIC初始化结构体
	//Key1
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	//Key2
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
}

void EXTI_Key_Config(void)
{
	//第一步：初始化需要连接到EXTI线的GPIO
	RCC_AHB1PeriphClockCmd(KEY1_INT_GPIO_CLK | KEY2_INT_GPIO_CLK ,ENABLE);	//打开两个按键时钟
	GPIO_InitTypeDef GPIO_InitStructure;	//定义初始化GPIO结构体
	//KEY1
	GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);
	//KEY2
	GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
	GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);
	
	
	//第二步：初始化EXTI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	//打开 SYSCFG 时钟
	
	EXTI_InitTypeDef EXTI_InitStructure;	//定义初始化EXTI结构体
	//连接 EXTI 中断源 到Key1引脚并初始化EXTI
	SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//上升沿
	EXTI_Init(&EXTI_InitStructure);
	//连接 EXTI 中断源 到Key2引脚并初始化EXTI
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿
	EXTI_Init(&EXTI_InitStructure);
	
	//第三步：配置NVIC
	NVIC_Configuration();
	
	//第四步：编写中断服务函数
	
}
