#include "bsp_usart.h"

RingBuff_t 	ringBuff;	//创建一个ringBuff的缓冲区
uint8_t 	RxBuff[1];

/**
  * @brief  DEBUG_USART GPIO 配置,工作模式配置。9600 8-N-1 ，中断接收模式
  * @param  无
  * @retval 无
*/
void Debug_USART1_Config(void)
{
	/* 使能 GPIO 时钟 */
	RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK | DEBUG_USART_TX_GPIO_CLK, ENABLE); //USART1 的 TX、RX 在 A9\A10 引脚
	/* 定义 GPIO 初始化结构体 */
	GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* 配置 Tx 引脚为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin  = DEBUG_USART_TX_PIN;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	/* 配置 Rx 引脚为复用功能 */
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	//GPIO 复用第二功能
	/* 连接 PXx 到 USARTx_Rx*/
	GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT, DEBUG_USART_RX_SOURCE, DEBUG_USART_RX_AF);
	/*  连接 PXx 到 USARTx__Tx*/
	GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT, DEBUG_USART_TX_SOURCE, DEBUG_USART_TX_AF);
	
	/* 使能 USART 时钟 */
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
	/* 定义 USART 初始化结构体 */
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
	
	/* 定义中断初始化结构体 */
	NVIC_InitTypeDef NVIC_InitStructure;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* 配置USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	/* 抢断优先级 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* 子优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 使能串口 */
	USART_Cmd(DEBUG_USART, ENABLE);
}

/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* 配置USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	/* 抢断优先级为1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	/* 子优先级为1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  DEBUG_USART6 GPIO 配置,工作模式配置。9600 8-N-1
  * @param  无
  * @retval 无
  */
void Debug_USART6_Config(void)
{
	//第一步：初始化GPIO PC6-TX PC7-RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* 配置Tx引脚为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* 配置Rx引脚为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO复用第二功能
	/* 连接 PXx 到 USARTx_Tx */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	/* 连接 PXx 到 USARTx_Rx */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
	
	
	//第二步：配置串口初始化结构体
	/* 使能 USART 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	/* 配置串DEBUG_USART 模式 */
	USART_InitTypeDef USART6_InitStructure;
	/* 波特率设置：DEBUG_USART_BAUDRATE */
	USART6_InitStructure.USART_BaudRate = 9600;
	/* 字长(数据位+校验位)：8 */
	USART6_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* 停止位：1个停止位 */
	USART6_InitStructure.USART_StopBits = USART_StopBits_1;
	/* 校验位选择：不使用校验 */
	USART6_InitStructure.USART_Parity = USART_Parity_No;
	/* 硬件流控制：不使用硬件流 */
	USART6_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USART模式控制：同时使能接收和发送 */
	USART6_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* 完成USART初始化配置 */
	USART_Init(USART6, &USART6_InitStructure); 
	
	/* 嵌套向量中断控制器NVIC配置 */
	NVIC_Configuration();
	
	/* 使能串口接收中断 */
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART6, ENABLE);
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

/********************************************************
函数名：  	TJCPrintf
功能：    	向串口打印数据,使用前请将USART_SCREEN_write这个函数改为你的单片机的串口发送单字节函数
输入参数：	参考printf
返回值： 	打印到串口的数量
**********************************************************/
void TJCPrintf(const char *str, ...)
{ 
	char buffer[STR_LENGTH + 1];  // 数据长度
	uint8_t i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, str);  
	vsnprintf(buffer, STR_LENGTH+1, str, arg_ptr);
	va_end(arg_ptr);
	while ((i < STR_LENGTH) && (i < strlen(buffer)))
	{
        USART_SendData(USART1, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
	}
	USART_SendData(USART1, (uint8_t)0xff);		//这个函数改为你的单片机的串口发送单字节函数
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
	USART_SendData(USART1,(uint8_t)0xff);		//这个函数改为你的单片机的串口发送单字节函数
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
	USART_SendData(USART1,(uint8_t)0xff);		//这个函数改为你的单片机的串口发送单字节函数
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
}

/********************************************************
函数名：  	initRingBuff
功能：    	初始化环形缓冲区
返回值： 	void
**********************************************************/
void initRingBuff(void)
{
	//初始化相关信息
	ringBuff.Head = 0;
	ringBuff.Tail = 0;
	ringBuff.Length = 0;
}

/********************************************************
函数名：  	writeRingBuff
功能：    	往环形缓冲区写入数据
返回值： 	void
**********************************************************/
void writeRingBuff(uint8_t data)
{
	if(ringBuff.Length >= RINGBUFF_LEN) //判断缓冲区是否已满
	{
		return;
	}
	ringBuff.Ring_data[ringBuff.Tail] = data;
	ringBuff.Tail = (ringBuff.Tail + 1) % RINGBUFF_LEN;	//防止越界非法访问
	ringBuff.Length++;
}

/********************************************************
函数名：  	deleteRingBuff
功能：    	删除串口缓冲区中相应长度的数据
输入参数：	要删除的长度
返回值： 	void
**********************************************************/
void deleteRingBuff(uint16_t size)
{
	if(size >= ringBuff.Length)
	{
	    initRingBuff();
	    return;
	}
	for(int i = 0; i < size; i++)
	{

		if(ringBuff.Length == 0)//判断非空
		{
			initRingBuff();
			return;
		}
		ringBuff.Head = (ringBuff.Head + 1) % RINGBUFF_LEN;//防止越界非法访问
		ringBuff.Length--;
	}
}

/********************************************************
函数名：  	read1BFromRingBuff
功能：    	从串口缓冲区读取1字节数据
输入参数：	position:读取的位置
返回值： 	所在位置的数据(1字节)
**********************************************************/
uint8_t read1BFromRingBuff(uint16_t position)
{
	uint16_t realPosition = (ringBuff.Head + position) % RINGBUFF_LEN;
	return ringBuff.Ring_data[realPosition];
}

/********************************************************
函数名：  	getRingBuffLenght
功能：    	获取串口缓冲区的数据数量
返回值： 	串口缓冲区的数据数量
**********************************************************/
uint16_t getRingBuffLenght()
{
	return ringBuff.Length;
}


/********************************************************
函数名：  	isRingBuffOverflow
功能：    	判断环形缓冲区是否已满
返回值： 	1:环形缓冲区已满 , 2:环形缓冲区未满
**********************************************************/
uint8_t isRingBuffOverflow()
{
	return ringBuff.Length == RINGBUFF_LEN;
}
