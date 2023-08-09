#ifndef _BSP_MASTER_H
#define _BSP_MASTER_H

#include "stm32f4xx.h"
#include "./timer/bsp_timer.h"
#include "./485/bsp_485.h"
#include "math.h"
#include <stdlib.h>

//����ָ��ṹ��
typedef struct 
{
	uint8_t SlaverAddr;		//�ӻ���ַ
	uint8_t FuctionNum;		//������
	uint8_t AddrH;			//�Ĵ�����ʼ��ַ �� 8 λ��ַ
	uint8_t AddrL;			//�Ĵ�����ʼ��ַ �� 8 λ��ַ
	uint8_t QuantityH;		//�Ĵ������� �� 8 λ
	uint8_t QuantityL;		//�Ĵ������� �� 8 λ
	uint8_t CRCL;			//CRCУ��� 8 λ
	uint8_t CRCH;			//CRCУ��� 8 λ
}Command;

static uint16_t CRC_Compute(uint8_t* puchMsg, uint16_t usDataLen);	//CRCУ�麯��

void ReadVal_Service(uint8_t SlaverAddr);			//����������ֵ�������
static void Read_Val(Command* Read_VAL_Cmd);		//�򴫸������Ͷ�����ֵָ��
void Receive_Service(void);							//�������ݴ�����
void Modbus_OX_Solve(void);							//����ֵ�����ʾ����
void Modbus_PH_Solve(void);							//����ֵ�����ʾ����
void Modbus_RTU(void);


#endif

