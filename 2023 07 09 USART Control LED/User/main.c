#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"
#include "bsp_usart.h"

static void Show_Message(void);

int main(void)
{
	char ch;
	
	/* ��ʼ��RGB�ʵ� */
	LED_GPIO_Config();
	
	/* ��ʼ��USART ����ģʽΪ 115200 8-N-1 */
	Debug_USART_Config();
	
	/* ��ӡָ��������ʾ��Ϣ */
	Show_Message();
	
	while (1)
	{
		/* ��ȡ�ַ�ָ�� */
		ch = getchar();
		printf("���յ��ַ���%c\n",ch);
		
		switch(ch)
		{
			case '1':	LED_RED;
			break;
			case '2':	LED_GREEN;
			break;
			case '3':	LED_BLUE;
			break;
			case '4':	LED_YELLOW;
			break;
			case '5':	LED_PURPLE;
			break;
			case '6':	LED_CYAN;
			break;
			case '7':	LED_WHITE;
			break;
			case '8':	LED_RGBOFF;
			break;
			default:
			/* �������ָ��ָ���ַ�����ӡ��ʾ��Ϣ */
			Show_Message();
			break;      
		} 
	}
}

/**
  * @brief  ��ӡָ��������ʾ��Ϣ
  * @param  ��
  * @retval ��
  */
static void Show_Message(void)
{
	printf("\r\n   ����һ��ͨ������ͨ��ָ�����RGB�ʵ�ʵ�� \n");
	printf("ʹ��  USART1  ����Ϊ��%d 8-N-1 \n", DEBUG_USART_BAUDRATE);
	printf("������ӵ�ָ������RGB�ʵ���ɫ��ָ���Ӧ���£�\n");
	printf("   ָ��   ------ �ʵ���ɫ \n");
	printf("     1    ------    �� \n");
	printf("     2    ------    �� \n");
	printf("     3    ------    �� \n");
	printf("     4    ------    �� \n");
	printf("     5    ------    �� \n");
	printf("     6    ------    �� \n");
	printf("     7    ------    �� \n");
	printf("     8    ------    �� \n");  
}