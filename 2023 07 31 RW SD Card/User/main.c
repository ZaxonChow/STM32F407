#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"
#include "bsp_usart.h"
#include "./key/bsp_key.h"

extern void SD_Test(void);

void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	Key_GPIO_Config();
	Debug_USART_Config();
	LED_GPIO_Config();
	LED_BLUE;
	
    printf("\r\n欢迎使用野火  STM32 F407 开发板。\r\n");	
	printf("在开始进行SD卡基本测试前，请给开发板插入32G以内的SD卡\r\n");			
	printf("本程序会对SD卡进行 非文件系统 方式读写，会删除SD卡的文件系统\r\n");		
	printf("实验后可通过电脑格式化或使用SD卡文件系统的例程恢复SD卡文件系统\r\n");		
	printf("但sd卡内的原文件不可恢复，实验前务必备份SD卡内的原文件！！！\r\n");		
	printf("若已确认，请按开发板的KEY1按键，开始SD卡测试实验....\r\n");
	
	while (1)
	{
		if(Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
		{
			printf("开始进行SD卡读写实验\r\n");
			SD_Test();
		}
	}
}

