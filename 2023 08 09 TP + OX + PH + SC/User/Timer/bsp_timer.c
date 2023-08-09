#include "./timer/bsp_timer.h"

uint8_t RS485_Frame_Distance = 4;	//数据帧最小间隔(ms),超过此时间则认为是下一帧，Timer7负责延时

//Timer7 初始化	
//功能：判断从机返回的数据是否接受完成
void Timer7_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseStructure.TIM_Period = RS485_Frame_Distance * 10; 	//设置在下一个更新事件装入活动的自动重装载寄存器周期的值;;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;						//设置用来作为 TIMx 时钟频率除数的预分频值 设置计数频率为10kHz
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE );					//TIM7 允许更新中断
	
	//TIM7中断分组配置
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  			//TIM7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢优先级2级，抢优先级值越小，优先级越高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  		//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);
}

//定时器4初始化
//定时1s进行通讯，分2步，前500ms进行发送功能，后500ms处理从机返回的数据
void Timer4_enable(uint16_t arr)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; 	//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM4
		TIM_IT_Update |		//TIM 中断源
		TIM_IT_Trigger,		//TIM 触发中断源 
		ENABLE  //使能
		);

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  			//TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设							 
}

void Timer4_disable(void)//TIM4失能
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE); //时钟失能
	TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_Trigger, DISABLE );
	TIM_Cmd(TIM4, DISABLE);  //失能TIMx外设
}


