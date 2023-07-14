#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./485/bsp_485.h"
#include "./key/bsp_key.h"
#include "./timer/bsp_timer.h"

uint8_t RS485_RX_BUFF[2048];	//���ջ�����2048�ֽ�
uint16_t RS485_RX_CNT = 0;		//���ռ�����
uint8_t RS485_RxFlag = 0;		//����һ֡�������


uint8_t Read_Val[8] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x02, 0x95, 0xCB};	//������ֵָ��

void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{	
	uint8_t i = 0;
	
	LED_GPIO_Config();	
	
	//Key_GPIO_Config();

	/*��ʼ��USART1*/
	Debug_USART_Config();
	
	/*��ʼ��485ʹ�õĴ��ڣ�ʹ���ж�ģʽ����*/
	_485_Config();
	
	LED_BLUE;	

	printf("\r\n 485��ȡ�ܽ���ʵ������\r\n");
	
	//����һ��ָ��
	
	_485_TX_EN();
	
	for(i = 0; i < 8;i++)
	{
		_485_SendByte(Read_Val[i]);	 //��������
		Delay(0xFFFF);
	}
	
	/*�Ӷ�����ʱ����֤485�����������*/
	Delay(0xFFFF);
	
	_485_RX_EN();
	
	LED_GREEN;
	
	printf("\r\n����ָ��ɹ���\r\n"); //ʹ�õ��Դ��ڴ�ӡ������Ϣ���ն�
	
	
	while(1)
	{
		
//		/* ��һ�ΰ�������һ��ָ�� */
//		if(	Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON)
//		{
//			uint16_t i;
//			
//			LED_BLUE;
//			
//			_485_TX_EN();
//			
//			for(i=0;i<=0xff;i++)
//			{
//				_485_SendByte(i);	 //��������
//			}
//			
//			/*�Ӷ�����ʱ����֤485�����������*/
//			Delay(0xFFF);
//			_485_RX_EN();
//			
//			LED_GREEN;
//			
//			printf("\r\n�������ݳɹ���\r\n"); //ʹ�õ��Դ��ڴ�ӡ������Ϣ���ն�

//		}
//		else
//		{		
//			LED_BLUE;
//			
//			pbuf = get_rebuff(&len);
//			if(len >= 256)
//			{
//				LED_GREEN;
//				printf("\r\n���յ�����Ϊ%d������\r\n",len);	
//				_485_DEBUG_ARRAY((uint8_t*)pbuf,len);
//				clean_rebuff();
//			}
//		}
	}
}

