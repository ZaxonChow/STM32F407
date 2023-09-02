#include "bsp_key.h"

void KEY_GPIO_Config(void)
{
	/*第一步：打开GPIO的时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	
	
	/*第二步：定义一个GPIO初始化结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*第三步：配置GPIO初始化结构体成员*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	/*第四步：调用GPIO初始化函数，把配置好的结构体成员参数写入寄存器*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*第三步：配置GPIO初始化结构体成员*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	/*第四步：调用GPIO初始化函数，把配置好的结构体成员参数写入寄存器*/
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
