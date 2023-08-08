#include "bsp_usart.h"

RingBuff_t 	ringBuff;	//����һ��ringBuff�Ļ�����
uint8_t 	RxBuff[1];

/**
  * @brief  DEBUG_USART GPIO ����,����ģʽ���á�9600 8-N-1 ���жϽ���ģʽ
  * @param  ��
  * @retval ��
*/
void Debug_USART1_Config(void)
{
	/* ʹ�� GPIO ʱ�� */
	RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK | DEBUG_USART_TX_GPIO_CLK, ENABLE); //USART1 �� TX��RX �� A9\A10 ����
	/* ���� GPIO ��ʼ���ṹ�� */
	GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* ���� Tx ����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin  = DEBUG_USART_TX_PIN;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	/* ���� Rx ����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	//GPIO ���õڶ�����
	/* ���� PXx �� USARTx_Rx*/
	GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT, DEBUG_USART_RX_SOURCE, DEBUG_USART_RX_AF);
	/*  ���� PXx �� USARTx__Tx*/
	GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT, DEBUG_USART_TX_SOURCE, DEBUG_USART_TX_AF);
	
	/* ʹ�� USART ʱ�� */
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
	/* ���� USART ��ʼ���ṹ�� */
	USART_InitTypeDef USART_InitStructure;
	/* ���������ã�DEBUG_USART_BAUDRATE */
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	/* �ֳ�(����λ+У��λ)��8 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* ֹͣλ��1��ֹͣλ */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* У��λѡ�񣺲�ʹ��У�� */
	USART_InitStructure.USART_Parity = USART_Parity_No;
	/* Ӳ�������ƣ���ʹ��Ӳ���� */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* ���USART��ʼ������ */
	USART_Init(DEBUG_USART, &USART_InitStructure); 
	
	/* �����жϳ�ʼ���ṹ�� */
	NVIC_InitTypeDef NVIC_InitStructure;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	/* �������ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* ʹ�ܴ��� */
	USART_Cmd(DEBUG_USART, ENABLE);
}

/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	/* �������ȼ�Ϊ1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	/* �����ȼ�Ϊ1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  DEBUG_USART6 GPIO ����,����ģʽ���á�9600 8-N-1
  * @param  ��
  * @retval ��
  */
void Debug_USART6_Config(void)
{
	//��һ������ʼ��GPIO PC6-TX PC7-RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* ����Tx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO���õڶ�����
	/* ���� PXx �� USARTx_Tx */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	/* ���� PXx �� USARTx_Rx */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
	
	
	//�ڶ��������ô��ڳ�ʼ���ṹ��
	/* ʹ�� USART ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	/* ���ô�DEBUG_USART ģʽ */
	USART_InitTypeDef USART6_InitStructure;
	/* ���������ã�DEBUG_USART_BAUDRATE */
	USART6_InitStructure.USART_BaudRate = 9600;
	/* �ֳ�(����λ+У��λ)��8 */
	USART6_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* ֹͣλ��1��ֹͣλ */
	USART6_InitStructure.USART_StopBits = USART_StopBits_1;
	/* У��λѡ�񣺲�ʹ��У�� */
	USART6_InitStructure.USART_Parity = USART_Parity_No;
	/* Ӳ�������ƣ���ʹ��Ӳ���� */
	USART6_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
	USART6_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* ���USART��ʼ������ */
	USART_Init(USART6, &USART6_InitStructure); 
	
	/* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART6, ENABLE);
}

/*****************  ����һ���ַ� **********************/
void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart_SendString(USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
	do 
	{
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	}while(*(str + k)!='\0');

	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC)==RESET);
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(DEBUG_USART, (uint8_t) ch);

	/* �ȴ�������� */
	while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		

	return (ch);
}

//�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(DEBUG_USART);
}

/********************************************************
��������  	TJCPrintf
���ܣ�    	�򴮿ڴ�ӡ����,ʹ��ǰ�뽫USART_SCREEN_write���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
���������	�ο�printf
����ֵ�� 	��ӡ�����ڵ�����
**********************************************************/
void TJCPrintf(const char *str, ...)
{ 
	char buffer[STR_LENGTH + 1];  // ���ݳ���
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
	USART_SendData(USART1, (uint8_t)0xff);		//���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
	USART_SendData(USART1,(uint8_t)0xff);		//���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
	USART_SendData(USART1,(uint8_t)0xff);		//���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
}

/********************************************************
��������  	initRingBuff
���ܣ�    	��ʼ�����λ�����
����ֵ�� 	void
**********************************************************/
void initRingBuff(void)
{
	//��ʼ�������Ϣ
	ringBuff.Head = 0;
	ringBuff.Tail = 0;
	ringBuff.Length = 0;
}

/********************************************************
��������  	writeRingBuff
���ܣ�    	�����λ�����д������
����ֵ�� 	void
**********************************************************/
void writeRingBuff(uint8_t data)
{
	if(ringBuff.Length >= RINGBUFF_LEN) //�жϻ������Ƿ�����
	{
		return;
	}
	ringBuff.Ring_data[ringBuff.Tail] = data;
	ringBuff.Tail = (ringBuff.Tail + 1) % RINGBUFF_LEN;	//��ֹԽ��Ƿ�����
	ringBuff.Length++;
}

/********************************************************
��������  	deleteRingBuff
���ܣ�    	ɾ�����ڻ���������Ӧ���ȵ�����
���������	Ҫɾ���ĳ���
����ֵ�� 	void
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

		if(ringBuff.Length == 0)//�жϷǿ�
		{
			initRingBuff();
			return;
		}
		ringBuff.Head = (ringBuff.Head + 1) % RINGBUFF_LEN;//��ֹԽ��Ƿ�����
		ringBuff.Length--;
	}
}

/********************************************************
��������  	read1BFromRingBuff
���ܣ�    	�Ӵ��ڻ�������ȡ1�ֽ�����
���������	position:��ȡ��λ��
����ֵ�� 	����λ�õ�����(1�ֽ�)
**********************************************************/
uint8_t read1BFromRingBuff(uint16_t position)
{
	uint16_t realPosition = (ringBuff.Head + position) % RINGBUFF_LEN;
	return ringBuff.Ring_data[realPosition];
}

/********************************************************
��������  	getRingBuffLenght
���ܣ�    	��ȡ���ڻ���������������
����ֵ�� 	���ڻ���������������
**********************************************************/
uint16_t getRingBuffLenght()
{
	return ringBuff.Length;
}


/********************************************************
��������  	isRingBuffOverflow
���ܣ�    	�жϻ��λ������Ƿ�����
����ֵ�� 	1:���λ��������� , 2:���λ�����δ��
**********************************************************/
uint8_t isRingBuffOverflow()
{
	return ringBuff.Length == RINGBUFF_LEN;
}
