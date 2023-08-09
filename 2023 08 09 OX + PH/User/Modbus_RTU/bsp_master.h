#ifndef _BSP_MASTER_H
#define _BSP_MASTER_H

#include "stm32f4xx.h"
#include "./timer/bsp_timer.h"
#include "./485/bsp_485.h"
#include "math.h"
#include <stdlib.h>

//定于指令结构体
typedef struct 
{
	uint8_t SlaverAddr;		//从机地址
	uint8_t FuctionNum;		//操作数
	uint8_t AddrH;			//寄存器起始地址 高 8 位地址
	uint8_t AddrL;			//寄存器起始地址 低 8 位地址
	uint8_t QuantityH;		//寄存器数量 高 8 位
	uint8_t QuantityL;		//寄存器数量 低 8 位
	uint8_t CRCL;			//CRC校验低 8 位
	uint8_t CRCH;			//CRC校验高 8 位
}Command;

static uint16_t CRC_Compute(uint8_t* puchMsg, uint16_t usDataLen);	//CRC校验函数

void ReadVal_Service(uint8_t SlaverAddr);			//主机读测量值服务程序
static void Read_Val(Command* Read_VAL_Cmd);		//向传感器发送读测量值指令
void Receive_Service(void);							//接收数据处理函数
void Modbus_OX_Solve(void);							//测量值输出显示函数
void Modbus_PH_Solve(void);							//测量值输出显示函数
void Modbus_RTU(void);


#endif

