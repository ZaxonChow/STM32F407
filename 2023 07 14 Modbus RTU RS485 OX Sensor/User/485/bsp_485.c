/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @brief   485����
  ******************************************************************************
  */ 
#include "./485/bsp_485.h"
#include "bsp_timer.h"
#include <stdarg.h>

extern uint8_t RS485_RxFlag;
extern uint8_t RS485_RX_BUFF[2048];	//���ջ�����2048�ֽ�
extern uint16_t RS485_RX_CNT;

uint8_t RS485_TX_BUFF[2048];	//���ͻ�����
uint16_t RS485_TX_CNT = 0;		//���ͼ�����
uint8_t RS485_TxFlag = 0;		//����һ֡�������

uint8_t state = 0, ox_state = 0, ph_state = 0;

//����������
u8 SlaverAddr_1=0x01;   //�ӻ���ַ1 ph
u8 SlaverAddr_2=0x02;   //�ӻ���ַ2 ox
u8 SlaverAddr;

//0x01,0x03,0x00,0x00,0x00,0x04
u8 Fuction_1 = 0x03;
u8 ji_dizhi_gao_1 = 0x00;
u8 ji_dizhi_di_1 = 0x00;
u8 ji_shuliang_gao_1 = 0x00;
u8 ji_shuliang_di_1 = 0x04;

//0x02,0x03,0x00,0x00,0x00,0x04
u8 Fuction_2 = 0x03;
u8 ji_dizhi_gao_2 = 0x00;
u8 ji_dizhi_di_2 = 0x00;
u8 ji_shuliang_gao_2 = 0x00;
u8 ji_shuliang_di_2 = 0x04;

//������Ҫ���豸��ַ������֡�޸�
u8 SlaverAddr_6 = 0x06;
u8 Fuction_6 = 0x06;
u8 ji_dizhi_gao_6 = 0x20;
u8 ji_dizhi_di_6 = 0x02;
u8 ji_shuliang_gao_6 = 0x00;
u8 ji_shuliang_di_6 = 0x02;


//CRCУ��
//********************************************************************************************************************
const uint8_t auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x9A, 0x28, 0xEA, 0x49,
0x8F, 0xF9, 0x41, 0x79, 0xFA}; 

const uint8_t auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,0x43, 0x83, 0x41, 0x81, 0x80, 0x40, 0x9A, 0x69, 0xAB, 0xC8, 
0x8F, 0x41, 0xF9, 0x79, 0xFA};
//********************************************************************************************************************

uint16_t CRC_Compute(uint8_t *puchMsg, uint16_t usDataLen) 
{ 
	uint8_t uchCRCHi = 0xFF ; 
	uint8_t uchCRCLo = 0xFF ; 
	uint32_t uIndex ; 
	while(usDataLen--) 
	{ 
		uIndex = uchCRCHi ^ *puchMsg++ ; 
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
		uchCRCLo = auchCRCLo[uIndex] ; 
	} 
	//printf("uchCRCHi:%d,uchCRCLo:%d\r\d",uchCRCHi,uchCRCLo);
	return ((uchCRCHi<< 8)  | (uchCRCLo)) ; 
}

static void Delay(__IO u32 nCount); 

//���� USART2 �����ж�
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

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
void _485_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Config USART clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
	
	/* Connect PA3 to USART2_RX*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	/* Connect PA2 to USART2_TX*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	
	/* USART2 GPIO config */
	/* Configure USART2 TX as alternate function push-pull */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 RX as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	/* 485�շ�����ʹ�ܹܽ� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  ;
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
	
	USART_Cmd(_485_USART, ENABLE);
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_Configuration();
	
	GPIO_ResetBits(_485_RE_GPIO_PORT,_485_RE_PIN); //Ĭ�Ͻ������ģʽ
	
	Timer7_Init();//��ʱ��7��ʼ�������ڼ��ӿ���ʱ��
}

/***************** ����һ���ַ�  **********************/
//ʹ�õ��ֽ����ݷ���ǰҪʹ�ܷ������ţ����ͺ�Ҫʹ�ܽ������š�
void _485_SendByte(  uint8_t ch )
{
	/* ����һ���ֽ����ݵ�USART1 */
	USART_SendData(_485_USART, ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(_485_USART, USART_FLAG_TXE) == RESET);	
}

/*****************  ����ָ�����ȵ��ַ��� **********************/
void _485_SendStr_length(uint8_t *str, uint32_t strlen)
{
	unsigned int k = 0;

	_485_TX_EN()	;	//ʹ�ܷ�������	
    do 
    {
        _485_SendByte( *(str + k) );
        k++;
    } while(k < strlen);
		
	/*�Ӷ�����ʱ����֤485�����������*/
	Delay(0xFFF);
		
	_485_RX_EN();		//ʹ�ܽ�������
}


/*****************  �����ַ��� **********************/
void _485_SendString(  uint8_t *str)
{
	unsigned int k=0;
	
	_485_TX_EN();		//ʹ�ܷ�������
	
    do 
    {
        _485_SendByte(  *(str + k) );
        k++;
    } while(*(str + k)!='\0');
	
	/*�Ӷ�����ʱ����֤485�����������*/
	Delay(0xFFF);
		
	_485_RX_EN();		//ʹ�ܽ�������
}

//����n���ֽ����� ���������ݽ��з���
//buff:�������׵�ַ
//len�����͵��ֽ���
void RS485_SendData(uint8_t *buff,u8 Len)
{ 
		_485_TX_EN();	//�л�Ϊ����ģʽ
		while(Len--)
		{
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);//�ȴ�������Ϊ��
			USART_SendData(USART2, *(buff++));
		}
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);//�ȴ��������
		TX_RX_SET=1; //����������ɣ���ʱ��T4������յ�������
		_485_TX_EN();
}

//��ַ��01�������///
void Master_01_Slove(u8 SlaverAddr_1,u8 Fuction_1,u8 ji_dizhi_gao_1,u8 ji_dizhi_di_1,u8 ji_shuliang_gao_1,u8 ji_shuliang_di_1)
{ 
	u16 calCRC;
	RS485_TX_BUFF[0] = SlaverAddr_1; //��ַ1
	RS485_TX_BUFF[1] = Fuction_1;    //modbus ָ����
	RS485_TX_BUFF[2] = ji_dizhi_gao_1;  
	RS485_TX_BUFF[3] = ji_dizhi_di_1; 
	RS485_TX_BUFF[4] = ji_shuliang_gao_1;
	RS485_TX_BUFF[5] = ji_shuliang_di_1;
	calCRC=CRC_Compute(RS485_TX_BUFF,6);
	RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;//CRC���ֽ�
	RS485_TX_BUFF[7]=(calCRC)&0xFF;//CRC���ֽ�
	RS485_SendData(RS485_TX_BUFF,8);
}

//��ַ��02�������///
void Master_03_Slove(u8 SlaverAddr_2,u8 Fuction_2,u8 ji_dizhi_gao_2,u8 ji_dizhi_di_2,u8 ji_shuliang_gao_2,u8 ji_shuliang_di_2)
{ 
	u16 calCRC;
	RS485_TX_BUFF[0] = SlaverAddr_2; 	//��ַ
	RS485_TX_BUFF[1] = Fuction_2;    	//modbus ָ����
	RS485_TX_BUFF[2] = ji_dizhi_gao_2;  
	RS485_TX_BUFF[3] = ji_dizhi_di_2; 
	RS485_TX_BUFF[4] = ji_shuliang_gao_2;
	RS485_TX_BUFF[5] = ji_shuliang_di_2;
	calCRC=CRC_Compute(RS485_TX_BUFF,6);
	RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;	//CRC���ֽ�
	RS485_TX_BUFF[7]=(calCRC)&0xFF;		//CRC���ֽ�
	RS485_SendData(RS485_TX_BUFF,8);
}

//Modbus������06�������///
void Master_06_Slove(u8 SlaverAddr_6,u8 Fuction_6,u8 ji_dizhi_gao_6,u8 ji_dizhi_di_6,u8 ji_shuliang_gao_6,u8 ji_shuliang_di_6)
{ 
	u16 calCRC;
	RS485_TX_BUFF[0] = SlaverAddr_6; 	//��ַ
	RS485_TX_BUFF[1] = Fuction_6;    	//modbus ָ����
	RS485_TX_BUFF[2] = ji_dizhi_gao_6;  
	RS485_TX_BUFF[3] = ji_dizhi_di_6; 
	RS485_TX_BUFF[4] = ji_shuliang_gao_6;
	RS485_TX_BUFF[5] = ji_shuliang_di_6;
	calCRC=CRC_Compute(RS485_TX_BUFF,6);
	RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;	//CRC���ֽ�
	RS485_TX_BUFF[7]=(calCRC)&0xFF;		//CRC���ֽ�
	RS485_SendData(RS485_TX_BUFF,8);
}

void Master_Service(uint8_t SlaverAddr)
{    
	switch(SlaverAddr)
	{
		case 01://��ph
				Master_01_Slove(SlaverAddr_1, Fuction_1, ji_dizhi_gao_1, ji_dizhi_di_1, ji_shuliang_gao_1, ji_shuliang_di_1);
				break;
		case 02://��ox 
				Master_03_Slove(SlaverAddr_2, Fuction_2, ji_dizhi_gao_2, ji_dizhi_di_2, ji_shuliang_gao_2, ji_shuliang_di_2);
				break;
		case 06://�ĵ�ַ  ��Ƶ��
				Master_06_Slove(SlaverAddr_6, Fuction_6, ji_dizhi_gao_6, ji_dizhi_di_6, ji_shuliang_gao_6, ji_shuliang_di_6);
				break;
	}		
}

//ox
//�ų��˼Ĵ������ֽ���֤���ƣ�����Ĭ�ϵ���ѯ������ջ�����
void Modbus_01_Solve(void)
{
	uint8_t rxlen = RS485_RX_CNT;    
	//uint8_t i = 0;		
	double ph_value;
	if(rxlen == RS485_RX_CNT&&rxlen)	//���յ�������,�ҽ��������
	{
//		for(i=0;i < rxlen;i++)
//		{
//			printf("%x    ",RS485_RX_BUFF[i]);	
//		}
        //printf("\r\n\r\n");			
		RS485_RX_CNT=0;		//����	
		//ph		
		ph_value=((double)RS485_RX_BUFF[3] * 256 + RS485_RX_BUFF[4]) * (double)pow(0.1, (double)(RS485_RX_BUFF[5] * 256 + RS485_RX_BUFF[6]));
		printf("ph_value=%f \r\n",ph_value);		
	}
	TX_RX_SET=0; //�������		
		
}
// ox
//�ų��˼Ĵ������ֽ���֤���ƣ�����Ĭ�ϵ���ѯ������ջ�����
void Modbus_02_Solve(void)
{
	uint8_t rxlen = RS485_RX_CNT;
	//uint8_t i = 0;
	double ox_value;
	double tem_value;
	if(rxlen == RS485_RX_CNT && rxlen)	//���յ�������,�ҽ��������
	{
//		for(i=0;i<rxlen;i++)
//		{
//			printf("%x    ",RS485_RX_BUFF[i]);	
//		}
        //printf("\r\n\r\n");			
		RS485_RX_CNT=0;		//����

		//ox
		ox_value=((double)RS485_RX_BUFF[3] * 256 + RS485_RX_BUFF[4]) * (double)pow(0.1, (double)(RS485_RX_BUFF[5] * 256 + RS485_RX_BUFF[6]));
		printf("ox_value=%f \r\n",ox_value);
		tem_value=((double)RS485_RX_BUFF[7] * 256 + RS485_RX_BUFF[8]) * (double)pow(0.1, (double)(RS485_RX_BUFF[9] * 256 + RS485_RX_BUFF[10]));
		printf("tem_value=%f \r\n",tem_value);
	}
	TX_RX_SET=0; //�������		
}

//Modbus������06�������
//�ų��˼Ĵ������ֽ���֤���ƣ�����Ĭ�ϵ���ѯ������ջ�����
void Modbus_06_Solve(void)
{
	uint8_t rxlen = RS485_RX_CNT;
	//uint8_t i = 0;
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
//		for(i=0;i<rxlen;i++)
//		{
//			printf("%d    ",RS485_RX_BUFF[i]);		
//		}		
		RS485_RX_CNT=0;		//����
	}
	TX_RX_SET=0; //�������		
}

//RS485����������ڴ�����յ�������
void RS485_RX_Service(void)
{
	uint16_t calCRC;
	uint16_t recCRC;
	
	if(RS485_RxFlag == 1)
	{
		if(RS485_RX_BUFF[0] == SlaverAddr_1 || RS485_RX_BUFF [0] == SlaverAddr_2)//��ַ��ȷ
		{
			if(RS485_RX_BUFF[1]==01 || RS485_RX_BUFF[1]==03 || RS485_RX_BUFF[1] == 06)//��������ȷ
			{
				calCRC = CRC_Compute(RS485_RX_BUFF, RS485_RX_CNT - 2);//�������������ݵ�CRC
				recCRC = RS485_RX_BUFF[RS485_RX_CNT-1] | (((uint16_t)RS485_RX_BUFF[RS485_RX_CNT-2]) << 8);//���յ���CRC(���ֽ���ǰ�����ֽ��ں�)
				if(calCRC == recCRC)//CRCУ����ȷ
				{
					switch(RS485_RX_BUFF[0])//���ݲ�ͬ�ĵ�ַ���д���
					{    
						case 01:
							Modbus_01_Solve();
							//printf("modbus_ph\r\n");
							break;
						case 02: 
							Modbus_02_Solve();
							//printf("modbus_ox\r\n");
							break;
						case 06:
							Modbus_06_Solve();
							//printf("check!\r\n");
							break;
					 }
															
				}
			}                                                                                       
		}
	}                              
	RS485_RxFlag = 0;		//��λ֡������־
	RS485_RX_CNT = 0;		//���ռ���������
	_485_TX_EN();		//��������ģʽ  
	TX_RX_SET=0; 		//�������
}

//ͨѶ����
void Modbus_RTU(void)   //������ TIM4 �жϵ��õ�
{	
	static uint8_t i = 0;
	switch(i)
	{
		case 0: //modbusִ�������һ����
				//�ڴ˴�Ҳ����ֱ��дMaster_Service( SlaverAddr, Fuction, StartAddr, ValueOrLenth);
				//���ͨѶ������԰������Ƶķ�װ������д
				//��ӻ�����һ�����󡣾��ڴ�ʱ�������TX_RX_SET=1 ����������ɣ���ʱ��T4������յ�������
				if(ox_state == 0)
				{
					Master_Service(0x02);	//���ܽ���
					ox_state = 1;
				}
				else
				{
					Master_Service(0x01);	//��ph
					ox_state = 0;
				}
				break;
		case 1: //modbus����ִ�еڶ�����
				RS485_RX_Service();  		//ִ�����ݽ��� 
				i = 0;						//���䲻�ϲ�ph �ܽ���
				break;					
	}
}



static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
