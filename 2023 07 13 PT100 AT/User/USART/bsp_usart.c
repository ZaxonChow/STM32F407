#include "bsp_usart.h"

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
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  /* �������ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  DEBUG_USART1 GPIO ����,����ģʽ���á�9600 8-N-1
  * @param  ��
  * @retval ��
  */
void Debug_USART1_Config(void)
{
	//��һ������ʼ��GPIO
	RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK | DEBUG_USART_TX_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* ����Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	//GPIO���õڶ�����
	/* ���� PXx �� USARTx_Rx*/
	GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE,DEBUG_USART_RX_AF);
	/*  ���� PXx �� USARTx__Tx*/
	GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);
	
	
	//�ڶ��������ô��ڳ�ʼ���ṹ��
	/* ʹ�� USART1 ʱ�� */
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
	/* ���ô�DEBUG_USART ģʽ */
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
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	/* ���USART��ʼ������ */
	USART_Init(DEBUG_USART, &USART_InitStructure); 

	USART_Cmd(DEBUG_USART, ENABLE);
}

 /**
  * @brief  DEBUG_USART2 GPIO ����,����ģʽ���á�9600 8-N-1
  * @param  ��
  * @retval ��
  */
void Debug_USART2_Config(void)
{
	//��һ������ʼ��GPIO PA2 PA3
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* ����Tx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO���õڶ�����
	/* ���� PXx �� USARTx_Tx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	/* ���� PXx �� USARTx__Rx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	
	//�ڶ��������ô��ڳ�ʼ���ṹ��
	/* ʹ�� USART ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* ���ô�DEBUG_USART ģʽ */
	USART_InitTypeDef USART2_InitStructure;
	/* ���������ã�DEBUG_USART_BAUDRATE */
	USART2_InitStructure.USART_BaudRate = 9600;
	/* �ֳ�(����λ+У��λ)��8 */
	USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* ֹͣλ��1��ֹͣλ */
	USART2_InitStructure.USART_StopBits = USART_StopBits_1;
	/* У��λѡ�񣺲�ʹ��У�� */
	USART2_InitStructure.USART_Parity = USART_Parity_No;
	/* Ӳ�������ƣ���ʹ��Ӳ���� */
	USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
	USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* ���USART��ʼ������ */
	USART_Init(USART2, &USART2_InitStructure); 
	
	/* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);
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

	return ch;
}

//�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
//int fgetc(FILE *f)
//{
//	/* �ȴ������������� */
//	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

//	return USART_ReceiveData(USART2);
//}