//bsp: board support package 板级支持包
#include "bsp_led.h"

void LED_GPIO_Config(void)
{
	/*以下步骤适合所有外设的初始化*/
	
	/*第一步：打开GPIO的时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/*第二步：定义一个GPIO初始化结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*第三步：配置GPIO初始化结构体成员*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	/*第四步：调用GPIO初始化函数，把配置好的结构体成员参数写入寄存器*/
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}
