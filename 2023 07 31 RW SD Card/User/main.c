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
	
    printf("\r\n��ӭʹ��Ұ��  STM32 F407 �����塣\r\n");	
	printf("�ڿ�ʼ����SD����������ǰ��������������32G���ڵ�SD��\r\n");			
	printf("��������SD������ ���ļ�ϵͳ ��ʽ��д����ɾ��SD�����ļ�ϵͳ\r\n");		
	printf("ʵ����ͨ�����Ը�ʽ����ʹ��SD���ļ�ϵͳ�����ָ̻�SD���ļ�ϵͳ\r\n");		
	printf("��sd���ڵ�ԭ�ļ����ɻָ���ʵ��ǰ��ر���SD���ڵ�ԭ�ļ�������\r\n");		
	printf("����ȷ�ϣ��밴�������KEY1��������ʼSD������ʵ��....\r\n");
	
	while (1)
	{
		if(Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
		{
			printf("��ʼ����SD����дʵ��\r\n");
			SD_Test();
		}
	}
}

