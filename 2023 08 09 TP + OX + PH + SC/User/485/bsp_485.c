#include "./485/bsp_485.h"

//配置 USART2 接收中断
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 描述  ：USART GPIO 配置,工作模式配置
 * 输入  ：无
 * 输出  : 无
 */
void RS485_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Config USART clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
	
	/* Connect PA3 to USART2_RX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	/* Connect PA2 to USART2_TX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	
	/* USART2 GPIO config */
	/* Configure USART2 TX as alternate function push-pull */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 RX as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	/* 485收发控制使能管脚 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
	USART_DeInit(USART2);//复位串口2
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = _485_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(_485_USART, &USART_InitStructure);
	
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	/* 使能串口接收中断 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_Configuration();
	
	USART_Cmd(_485_USART, ENABLE);
		
	GPIO_ResetBits(GPIOC, GPIO_Pin_0); 	//默认进入接收模式	
	
	Timer7_Init();						//定时器7初始化，用于监视空闲时间		
}

/***************** 发送一个字符 **********************/
//使用单字节数据发送前要使能发送引脚，发送后要使能接收引脚。
void RS485_SendByte(uint8_t ch)
{
	/* 发送一个字节数据到USART2 */
	USART_SendData(USART2, ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	
}

/***************** 发送 n 个字符 **********************
	*Buff: 发送区首地址
	*Len:  发送的字节数
******************************************************/
void RS485_SendData(uint8_t *Buff, uint8_t Len)
{ 
	_485_TX_EN();	//使能发送，PC0
	while(Len--)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//等待发送区为空
		USART_SendData(USART2, *(Buff++));
	}
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);			//等待发送完成
	_485_RX_EN();	//发送完成，使能接收，PC0
}
