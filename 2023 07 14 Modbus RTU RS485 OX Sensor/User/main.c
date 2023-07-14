#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./485/bsp_485.h"
#include "./key/bsp_key.h"
#include "./timer/bsp_timer.h"

uint8_t RS485_RX_BUFF[2048];	//接收缓冲区2048字节
uint16_t RS485_RX_CNT = 0;		//接收计数器
uint8_t RS485_RxFlag = 0;		//接收一帧结束标记


uint8_t Read_Val[8] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x02, 0x95, 0xCB};	//读测量值指令

void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{	
	uint8_t i = 0;
	
	LED_GPIO_Config();	
	
	//Key_GPIO_Config();

	/*初始化USART1*/
	Debug_USART_Config();
	
	/*初始化485使用的串口，使用中断模式接收*/
	_485_Config();
	
	LED_BLUE;	

	printf("\r\n 485读取溶解氧实验例程\r\n");
	
	//发送一个指令
	
	_485_TX_EN();
	
	for(i = 0; i < 8;i++)
	{
		_485_SendByte(Read_Val[i]);	 //发送数据
		Delay(0xFFFF);
	}
	
	/*加短暂延时，保证485发送数据完毕*/
	Delay(0xFFFF);
	
	_485_RX_EN();
	
	LED_GREEN;
	
	printf("\r\n发送指令成功！\r\n"); //使用调试串口打印调试信息到终端
	
	
	while(1)
	{
		
//		/* 按一次按键发送一个指令 */
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
//				_485_SendByte(i);	 //发送数据
//			}
//			
//			/*加短暂延时，保证485发送数据完毕*/
//			Delay(0xFFF);
//			_485_RX_EN();
//			
//			LED_GREEN;
//			
//			printf("\r\n发送数据成功！\r\n"); //使用调试串口打印调试信息到终端

//		}
//		else
//		{		
//			LED_BLUE;
//			
//			pbuf = get_rebuff(&len);
//			if(len >= 256)
//			{
//				LED_GREEN;
//				printf("\r\n接收到长度为%d的数据\r\n",len);	
//				_485_DEBUG_ARRAY((uint8_t*)pbuf,len);
//				clean_rebuff();
//			}
//		}
	}
}

