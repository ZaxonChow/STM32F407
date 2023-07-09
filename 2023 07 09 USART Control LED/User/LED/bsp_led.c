//bsp: board support package 板级支持包
#include "bsp_led.h"

void LED_GPIO_Config(void)
{
	/*第一步：打开GPIO的时钟*/
	RCC_AHB1PeriphClockCmd(LED1_GPIO_CLK|
	                      LED2_GPIO_CLK|
	                      LED3_GPIO_CLK, ENABLE);
	
	/*第二步：定义一个GPIO初始化结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*第三步：配置GPIO初始化结构体成员*/
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	/*第四步：调用GPIO初始化函数，把配置好的结构体成员参数写入寄存器*/
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
	
	/*选择要控制的GPIO引脚*/															   
	GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
    
    /*选择要控制的GPIO引脚*/															   
	GPIO_InitStructure.GPIO_Pin = LED3_PIN;	
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
		
	/*关闭RGB灯*/
	LED_RGBOFF;
}
