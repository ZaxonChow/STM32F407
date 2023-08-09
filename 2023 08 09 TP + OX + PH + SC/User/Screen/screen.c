#include "./screen/screen.h"

#define FRAMELENGTH 7

uint8_t Mode = 0;			//操作模式
uint8_t Auto_Control = 0;	//自动模式切换：0 - 关闭自动模式，1 - 起用自动模式
uint8_t Motor = 0;			//电机序号
uint8_t Motor_Mode = 0;		//电机工作状态：0 - 停止，1 - 启动

uint16_t Set_Data = 0;		//暂养箱系统参数：上限、下限、设定值
uint16_t PH_Upper = 0;		//PH上限值
uint16_t PH_Low   = 0;		//PH上限值
int16_t  TP_Upper = 0;		//温度上限值
int16_t  TP_Low   = 0;		//温度下限值
uint16_t OX_Upper = 0;		//溶解氧上限值
uint16_t OX_Low   = 0;		//溶解氧下限值

int16_t  TP_Set = 0;		//温度设定值
uint16_t PH_Set = 0;		//PH设定值

//保存温度数据
uint8_t Get_Temperature[20];
int16_t TP = 0;
extern uint8_t flag;

extern int16_t Temperature;

//接收到的串口屏数据：7个字节
//数据格式：0x55 xx xx xx FF FF FF
//xx xx xx 表示：
//	1、操作模式序号 + 手动控制开关 + 0xFF(保留字节)
//	2、操作模式序号 + 电机序号 + 电机状态
//	3、操作模式序号 + 两字节设定值
extern RingBuff_t ringBuff;
extern uint8_t RS485_RxFlag;

void Screen(void)
{
	static int16_t TP = 0;
	if(flag == 1)
	{	//温度TP
		TP = (Get_Temperature[3]-0x30)*1000 + (Get_Temperature[4]-0x30)*100 + (Get_Temperature[6]-0x30)*10 + (Get_Temperature[7]-0x30);
		TJCPrintf("page0.x0.val=%d\xff\xff\xff\r\n", TP);	//显示温度
		flag = 0;
	}
	
	if(RS485_RxFlag == 1)
	{
		//溶解氧 OX 或 PH
		Receive_Service();
	}
	
	Get_Status();
	
	//uint16_t PH = 2799;
	//uint16_t OX = 2799;
	
	//TJCPrintf("page0.x1.val=%d\xff\xff\xff", PH);//显示ph
	//TJCPrintf("page0.x2.val=%d\xff\xff\xff", OX);//显示溶解氧

/*
	//温度报警下限		  uint16_t s = (u(1)<< 8) | u(2);
	TJCPrintf("page0.wav0.en=%d\xff\xff\xff",1);//开启报警声
	udelete(FRAMELENGTH);
	//温度报警上限	  uint16_t s = (u(1)<< 8) | u(2);
	TJCPrintf("page0.wav0.en=%d\xff\xff\xff",1);//开启报警声
	udelete(FRAMELENGTH);
	//ph报警下限  uint16_t s = (u(1)<< 8) | u(2);
	TJCPrintf("page0.wav0.en=%d\xff\xff\xff",1);//开启报警声
	udelete(FRAMELENGTH);
	//ph报警上限  uint16_t s = (u(1)<< 8) | u(2);
	TJCPrintf("page0.wav0.en=%d\xff\xff\xff",1);//开启报警声
	udelete(FRAMELENGTH);
	//溶解氧报警下限		uint16_t s = (u(1)<< 8) | u(2);
	TJCPrintf("page0.wav0.en=%d\xff\xff\xff",1);//开启报警声
	udelete(FRAMELENGTH);		
	//溶解氧报警上限	uint16_t s = (u(1)<< 8) | u(2);
	TJCPrintf("page0.wav0.en=%d\xff\xff\xff",1);//开启报警声
	udelete(FRAMELENGTH);
*/	
}

void Get_Status(void)
{
	if(ringBuff.Length >= FRAMELENGTH)
	{
		//校验帧头帧尾数据格式正确
		if(u(0) != 0x55 || u(4) != 0xff || u(5) != 0xff || u(6) != 0xff)
		{
			//不匹配删除接受的数据并返回
			udelete(FRAMELENGTH);
			return;
		}
	}
	else
		return;
	
	Mode = u(1);
	Auto_Control = u(2);
	Motor = u(2);
	Motor_Mode = u(3);
	Set_Data = u(3);
	Set_Data = u(2) | (Set_Data << 8); 
	switch(Mode)
	{
		case 0x01://自动模式
			if(Auto_Control == 0x00)		//关闭自动模式
			{
				//关闭自动模式服务函数
				
			}
			else if(Auto_Control == 0x01)	//开启自动模式
			{
				//开启自动模式服务函数
				
			}
			udelete(FRAMELENGTH);
			break;
		case 0x02://升温电机 - 手动
			if(Motor == 0x01 && Motor_Mode == 0x01)		//启动1号升温电机
			{
				//启动1号升温电机服务函数
				
			}
			else if(Motor == 0x01 && Motor_Mode == 0x00)//关闭1号升温电机
			{
				//关闭1号升温电机服务函数
				
			}
			udelete(FRAMELENGTH);
			break;
		case 0x03:
			//降温电机
			if(Motor == 0x01 && Motor_Mode == 0x01)		//启动1号降温电机
			{
				//启动1号降温电机服务函数
				
			}
			else if(Motor == 0x01 && Motor_Mode == 0x00)//关闭1号降温电机
			{
				//关闭1号降温电机服务函数
				
			}
			udelete(FRAMELENGTH);
			break;
		case 0x04:
			//加酸电机
			if(Motor == 0x01 && Motor_Mode == 0x01)		//启动1号加酸电机
			{
				//启动1号加酸电机服务函数
				
			}
			else if(Motor == 0x01 && Motor_Mode == 0x00)//关闭1号加酸电机
			{
				//关闭1号加酸电机服务函数
				
			}
			udelete(FRAMELENGTH);
			break;
		case 0x05:
			//加碱电机
			if(Motor == 0x01 && Motor_Mode == 0x01)		//启动1号加碱电机
			{
				//启动1号加碱电机服务函数
				
			}
			else if(Motor == 0x01 && Motor_Mode == 0x00)//关闭1号加碱电机
			{
				//关闭1号加碱电机服务函数
				
			}
			udelete(FRAMELENGTH);
			break;
		case 0x06:
			//温度下限设定
			TP_Low = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x07:
			//温度上限
			TP_Upper = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x08:
			//PH下限
			PH_Low = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x09:
			//PH上限
			PH_Upper = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x10:
			//溶解氧下限
			OX_Low = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x11:
			//溶解氧上限
			OX_Upper = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x12:
			//温度设定值
			TP_Set = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x13:
			//PH设定值
			PH_Set = Set_Data;
			udelete(FRAMELENGTH);
			break;
	}	
}
