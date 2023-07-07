#include "bsp_exti.h"

static void NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;	//����NVIC��ʼ���ṹ��
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
	//��һ������ʼ����Ҫ���ӵ�EXTI�ߵ�GPIO
	RCC_AHB1PeriphClockCmd(KEY1_INT_GPIO_CLK | KEY2_INT_GPIO_CLK ,ENABLE);	//����������ʱ��
	GPIO_InitTypeDef GPIO_InitStructure;	//�����ʼ��GPIO�ṹ��
	//KEY1
	GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);
	//KEY2
	GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
	GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);
	
	
	//�ڶ�������ʼ��EXTI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	//�� SYSCFG ʱ��
	
	EXTI_InitTypeDef EXTI_InitStructure;	//�����ʼ��EXTI�ṹ��
	//���� EXTI �ж�Դ ��Key1���Ų���ʼ��EXTI
	SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//������
	EXTI_Init(&EXTI_InitStructure);
	//���� EXTI �ж�Դ ��Key2���Ų���ʼ��EXTI
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½���
	EXTI_Init(&EXTI_InitStructure);
	
	//������������NVIC
	NVIC_Configuration();
	
	//���Ĳ�����д�жϷ�����
	
}
