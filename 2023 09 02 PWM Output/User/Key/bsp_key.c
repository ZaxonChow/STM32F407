#include "bsp_key.h"

void KEY_GPIO_Config(void)
{
	/*��һ������GPIO��ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	
	
	/*�ڶ���������һ��GPIO��ʼ���ṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*������������GPIO��ʼ���ṹ���Ա*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	/*���Ĳ�������GPIO��ʼ�������������úõĽṹ���Ա����д��Ĵ���*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*������������GPIO��ʼ���ṹ���Ա*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	/*���Ĳ�������GPIO��ʼ�������������úõĽṹ���Ա����д��Ĵ���*/
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

uint8_t KEY_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON)
	{
		while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON);
		return KEY_ON;
	}
	else
		return KEY_OFF;
}
