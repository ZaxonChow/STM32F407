//bsp: board support package �弶֧�ְ�
#include "bsp_led.h"

void LED_GPIO_Config(void)
{
	/*���²����ʺ���������ĳ�ʼ��*/
	
	/*��һ������GPIO��ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/*�ڶ���������һ��GPIO��ʼ���ṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*������������GPIO��ʼ���ṹ���Ա*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	/*���Ĳ�������GPIO��ʼ�������������úõĽṹ���Ա����д��Ĵ���*/
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}
