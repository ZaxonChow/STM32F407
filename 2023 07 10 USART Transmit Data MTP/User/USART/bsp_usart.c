#include "bsp_usart.h"

uint8_t SendBuff[SENDBUFF_SIZE];

 /**
  * @brief  USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void Debug_USART_Config(void)
{
	//第一步：初始化GPIO
	RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK | DEBUG_USART_TX_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* 配置Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	/* 配置Rx引脚为复用功能 */
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	//GPIO复用第二功能
	/* 连接 PXx 到 USARTx_Rx*/
	GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE,DEBUG_USART_RX_AF);
	/*  连接 PXx 到 USARTx__Tx*/
	GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);
	
	
	//第二步：配置串口初始化结构体
	/* 使能 USART 时钟 */
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
	/* 配置串DEBUG_USART 模式 */
	USART_InitTypeDef USART_InitStructure;
	/* 波特率设置：DEBUG_USART_BAUDRATE */
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	/* 字长(数据位+校验位)：8 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* 停止位：1个停止位 */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* 校验位选择：不使用校验 */
	USART_InitStructure.USART_Parity = USART_Parity_No;
	/* 硬件流控制：不使用硬件流 */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USART模式控制：同时使能接收和发送 */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* 完成USART初始化配置 */
	USART_Init(DEBUG_USART, &USART_InitStructure); 
	
//	//第三步：配置串口接收中断
//	/* 嵌套向量中断控制器NVIC配置 */
//	NVIC_Configuration();
//  
//	/* 使能串口接收中断 */
//	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
	
	//第四步：使能串口
	/* 使能串口 */
	USART_Cmd(DEBUG_USART, ENABLE);
}

/*****************  发送一个字符 **********************/
void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  发送字符串 **********************/
void Usart_SendString(USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
	do 
	{
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	}while(*(str + k)!='\0');

	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC)==RESET);
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(DEBUG_USART, (uint8_t) ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		

	return (ch);
	}

//重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(DEBUG_USART);
}

/**
  * @brief  USART1 TX DMA 配置，内存到外设(USART1->DR)
  * @param  无
  * @retval 无
  */
void USART_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/*开启DMA时钟*/
	RCC_AHB1PeriphClockCmd(DEBUG_USART_DMA_CLK, ENABLE);
  
	/* 复位初始化DMA数据流 */
	DMA_DeInit(DEBUG_USART_DMA_STREAM);

	/* 确保DMA数据流复位完成 */
	while (DMA_GetCmdStatus(DEBUG_USART_DMA_STREAM) != DISABLE){}

	/*usart1 tx对应dma2，通道4，数据流7*/	
	DMA_InitStructure.DMA_Channel = DEBUG_USART_DMA_CHANNEL;  
	/*设置DMA源：串口数据寄存器地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = DEBUG_USART_DR_BASE;	 
	/*内存地址(要传输的变量的指针)*/
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SendBuff;
	/*方向：从内存到外设*/		
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	
	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
	DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
	/*外设地址不增*/	    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	/*内存地址自增*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	/*外设数据单位*/	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/*内存数据单位 8bit*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	/*DMA模式：不断循环*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 
	/*优先级：中*/	
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;      
	/*禁用FIFO*/
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;    
	/*存储器突发传输 16 个节拍*/
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
	/*外设突发传输 1 个节拍*/
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    
	/*配置DMA2的数据流7*/		   
	DMA_Init(DEBUG_USART_DMA_STREAM, &DMA_InitStructure);

	/*使能DMA*/
	DMA_Cmd(DEBUG_USART_DMA_STREAM, ENABLE);

	/* 等待DMA数据流有效*/
	while(DMA_GetCmdStatus(DEBUG_USART_DMA_STREAM) != ENABLE)
	{
	}   
}
