#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"
#include "bsp_usart.h"

extern uint8_t SendBuff[SENDBUFF_SIZE]; 

void Delay( uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	uint16_t i;
	/* 初始化USART */
	Debug_USART_Config(); 

	/* 配置使用DMA模式 */
	USART_DMA_Config();

	/* 配置RGB彩色灯 */
	LED_GPIO_Config();

	printf("\r\n USART1 DMA TX 测试 \r\n");

	/*填充将要发送的数据*/
	for(i=0;i<SENDBUFF_SIZE;i++)
	{
		SendBuff[i]	 = 'A';
	}

	/*为演示DMA持续运行而CPU还能处理其它事情，持续使用DMA发送数据，量非常大，
	*长时间运行可能会导致电脑端串口调试助手会卡死，鼠标乱飞的情况，
	*或把DMA配置中的循环模式改为单次模式*/		

	/* USART1 向 DMA发出TX请求 */
	USART_DMACmd(DEBUG_USART, USART_DMAReq_Tx, ENABLE);

	/* 此时CPU是空闲的，可以干其他的事情 */  
	//例如同时控制LED
	while(1)
	{
		LED1_TOGGLE;
		Delay(0xFFFFFF);
	}
}

