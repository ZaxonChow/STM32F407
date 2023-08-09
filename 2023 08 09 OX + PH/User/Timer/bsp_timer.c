#include "./timer/bsp_timer.h"

uint8_t RS485_Frame_Distance = 4;	//����֡��С���(ms),������ʱ������Ϊ����һ֡��Timer7������ʱ

//Timer7 ��ʼ��	
//���ܣ��жϴӻ����ص������Ƿ�������
void Timer7_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���
	TIM_TimeBaseStructure.TIM_Period = RS485_Frame_Distance * 10; 	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ;;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;						//����������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ ���ü���Ƶ��Ϊ10kHz
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE );					//TIM7 ��������ж�
	
	//TIM7�жϷ�������
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  			//TIM7�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//�����ȼ�2���������ȼ�ֵԽС�����ȼ�Խ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  		//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

//��ʱ��4��ʼ��
//��ʱ1s����ͨѶ����2����ǰ500ms���з��͹��ܣ���500ms����ӻ����ص�����
void Timer4_enable(uint16_t arr)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; 	//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM4, //TIM4
		TIM_IT_Update |		//TIM �ж�Դ
		TIM_IT_Trigger,		//TIM �����ж�Դ 
		ENABLE  //ʹ��
		);

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  			//TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����							 
}

void Timer4_disable(void)//TIM4ʧ��
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE); //ʱ��ʧ��
	TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_Trigger, DISABLE );
	TIM_Cmd(TIM4, DISABLE);  //ʧ��TIMx����
}


