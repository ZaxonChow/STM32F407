#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"
#include "bsp_usart.h"
#include "./can/bsp_can.h"
#include "./key/bsp_key.h"

__IO uint8_t flag = 0;		 //���ڱ�־�Ƿ���յ����ݣ����жϺ����и�ֵ
CanTxMsg TxMessage;			 //���ͻ�����
CanRxMsg RxMessage;			 //���ջ�����

void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	uint8_t mail_box;
	LED_GPIO_Config();
	
	/*��ʼ��USART1*/
	Debug_USART_Config();
	
	/*��ʼ������*/
	Key_GPIO_Config();
	
	/*��ʼ�� CAN,���жϽ��� CAN ���ݰ�*/
	CAN_Config();
	
	printf("\r\n CANͨѶ�ػ�ʵ������\r\n");
	
	printf("\r\n ʵ�鲽�裺\r\n");

	printf("\r\n 1.ʹ�ûػ�ģʽ������Ҫ�����ⲿоƬ\r\n");
	printf("\r\n 2.���¿������KEY1������ʹ��CAN���ⷢ��0-7�����ݰ���������չIDΪ0x1314 (���ڻػ�ģʽ�����Լ����͸��Լ�)\r\n");
	printf("\r\n 3.���������CAN���յ���չIDΪ0x1314�����ݰ�����������Դ�ӡ�����ڡ� \r\n");
	printf("\r\n 4.�����е�can������Ϊ1MBps��Ϊstm32��can������ʡ� \r\n");

	while (1)
	{
		/*��һ�ΰ�������һ������*/
		if(	Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
		{
			LED_BLUE;
			CAN_SetMsg(&TxMessage);
			
			CAN_Transmit(CANx, &TxMessage);
			
			//��������
			mail_box = CAN_Transmit(CANx, &TxMessage);
			
			if(mail_box != CAN_TxStatus_NoMailBox)
			{
				//�ȴ�ֱ�����ݷ������
				while(CAN_TransmitStatus(CANx, mail_box) != CAN_TxStatus_Ok);
				
				//��һ����ʱ��ȴ� CAN �շ��������ݷ��͵�����	
				Delay(0x1000);				
			}
			else
			{
				printf("\r\n no mail box");
			}
			
			//Delay(10000);//�ȴ�������ϣ���ʹ�� CAN_TransmitStatus �鿴״̬
			
			LED_GREEN;
			
			printf("\r\n��ʹ��CAN�������ݰ���\r\n"); 			
			printf("\r\n���͵ı�������Ϊ��\r\n");
			printf("\r\n ��չID��ExtId��0x%x \r\n",TxMessage.ExtId);
			CAN_DEBUG_ARRAY(TxMessage.Data,8); 
		}
		if(flag == 1)
		{		
			LED_GREEN;
			printf("\r\nCAN���յ����ݣ�\r\n");	

			CAN_DEBUG_ARRAY(RxMessage.Data,8); 
			
			flag=0;
		}
	
	}
}

