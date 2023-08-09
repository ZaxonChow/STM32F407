#include "./485/bsp_485.h"

//���� USART2 �����ж�
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
 * ����  ��USART GPIO ����,����ģʽ����
 * ����  ����
 * ���  : ��
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
  
	/* 485�շ�����ʹ�ܹܽ� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
	USART_DeInit(USART2);//��λ����2
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = _485_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(_485_USART, &USART_InitStructure);
	
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_Configuration();
	
	USART_Cmd(_485_USART, ENABLE);
		
	GPIO_ResetBits(GPIOC, GPIO_Pin_0); 	//Ĭ�Ͻ������ģʽ	
	
	Timer7_Init();						//��ʱ��7��ʼ�������ڼ��ӿ���ʱ��		
}

/***************** ����һ���ַ� **********************/
//ʹ�õ��ֽ����ݷ���ǰҪʹ�ܷ������ţ����ͺ�Ҫʹ�ܽ������š�
void RS485_SendByte(uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART2 */
	USART_SendData(USART2, ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	
}

/***************** ���� n ���ַ� **********************
	*Buff: �������׵�ַ
	*Len:  ���͵��ֽ���
******************************************************/
void RS485_SendData(uint8_t *Buff, uint8_t Len)
{ 
	_485_TX_EN();	//ʹ�ܷ��ͣ�PC0
	while(Len--)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//�ȴ�������Ϊ��
		USART_SendData(USART2, *(Buff++));
	}
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);			//�ȴ��������
	_485_RX_EN();	//������ɣ�ʹ�ܽ��գ�PC0
}
