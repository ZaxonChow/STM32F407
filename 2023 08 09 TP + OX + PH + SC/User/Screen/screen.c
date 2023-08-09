#include "./screen/screen.h"

#define FRAMELENGTH 7

uint8_t Mode = 0;			//����ģʽ
uint8_t Auto_Control = 0;	//�Զ�ģʽ�л���0 - �ر��Զ�ģʽ��1 - �����Զ�ģʽ
uint8_t Motor = 0;			//������
uint8_t Motor_Mode = 0;		//�������״̬��0 - ֹͣ��1 - ����

uint16_t Set_Data = 0;		//������ϵͳ���������ޡ����ޡ��趨ֵ
uint16_t PH_Upper = 0;		//PH����ֵ
uint16_t PH_Low   = 0;		//PH����ֵ
int16_t  TP_Upper = 0;		//�¶�����ֵ
int16_t  TP_Low   = 0;		//�¶�����ֵ
uint16_t OX_Upper = 0;		//�ܽ�������ֵ
uint16_t OX_Low   = 0;		//�ܽ�������ֵ

int16_t  TP_Set = 0;		//�¶��趨ֵ
uint16_t PH_Set = 0;		//PH�趨ֵ

//�����¶�����
uint8_t Get_Temperature[20];
int16_t TP = 0;
extern uint8_t flag;

extern int16_t Temperature;

//���յ��Ĵ��������ݣ�7���ֽ�
//���ݸ�ʽ��0x55 xx xx xx FF FF FF
//xx xx xx ��ʾ��
//	1������ģʽ��� + �ֶ����ƿ��� + 0xFF(�����ֽ�)
//	2������ģʽ��� + ������ + ���״̬
//	3������ģʽ��� + ���ֽ��趨ֵ
extern RingBuff_t ringBuff;
extern uint8_t RS485_RxFlag;

void Screen(void)
{
	static int16_t TP = 0;
	if(flag == 1)
	{	//�¶�TP
		TP = (Get_Temperature[3]-0x30)*1000 + (Get_Temperature[4]-0x30)*100 + (Get_Temperature[6]-0x30)*10 + (Get_Temperature[7]-0x30);
		TJCPrintf("page0.x0.val=%d\xff\xff\xff\r\n", TP);	//��ʾ�¶�
		flag = 0;
	}
	
	if(RS485_RxFlag == 1)
	{
		//�ܽ��� OX �� PH
		Receive_Service();
	}
	
	Get_Status();
	
	//uint16_t PH = 2799;
	//uint16_t OX = 2799;
	
	//TJCPrintf("page0.x1.val=%d\xff\xff\xff", PH);//��ʾph
	//TJCPrintf("page0.x2.val=%d\xff\xff\xff", OX);//��ʾ�ܽ���

/*
	//�¶ȱ�������		  uint16_t s = (u(1)<< 8) | u(2);
	TJCPrintf("page0.wav0.en=%d\xff\xff\xff",1);//����������
	udelete(FRAMELENGTH);
	//�¶ȱ�������	  uint16_t s = (u(1)<< 8) | u(2);
	TJCPrintf("page0.wav0.en=%d\xff\xff\xff",1);//����������
	udelete(FRAMELENGTH);
	//ph��������  uint16_t s = (u(1)<< 8) | u(2);
	TJCPrintf("page0.wav0.en=%d\xff\xff\xff",1);//����������
	udelete(FRAMELENGTH);
	//ph��������  uint16_t s = (u(1)<< 8) | u(2);
	TJCPrintf("page0.wav0.en=%d\xff\xff\xff",1);//����������
	udelete(FRAMELENGTH);
	//�ܽ�����������		uint16_t s = (u(1)<< 8) | u(2);
	TJCPrintf("page0.wav0.en=%d\xff\xff\xff",1);//����������
	udelete(FRAMELENGTH);		
	//�ܽ�����������	uint16_t s = (u(1)<< 8) | u(2);
	TJCPrintf("page0.wav0.en=%d\xff\xff\xff",1);//����������
	udelete(FRAMELENGTH);
*/	
}

void Get_Status(void)
{
	if(ringBuff.Length >= FRAMELENGTH)
	{
		//У��֡ͷ֡β���ݸ�ʽ��ȷ
		if(u(0) != 0x55 || u(4) != 0xff || u(5) != 0xff || u(6) != 0xff)
		{
			//��ƥ��ɾ�����ܵ����ݲ�����
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
		case 0x01://�Զ�ģʽ
			if(Auto_Control == 0x00)		//�ر��Զ�ģʽ
			{
				//�ر��Զ�ģʽ������
				
			}
			else if(Auto_Control == 0x01)	//�����Զ�ģʽ
			{
				//�����Զ�ģʽ������
				
			}
			udelete(FRAMELENGTH);
			break;
		case 0x02://���µ�� - �ֶ�
			if(Motor == 0x01 && Motor_Mode == 0x01)		//����1�����µ��
			{
				//����1�����µ��������
				
			}
			else if(Motor == 0x01 && Motor_Mode == 0x00)//�ر�1�����µ��
			{
				//�ر�1�����µ��������
				
			}
			udelete(FRAMELENGTH);
			break;
		case 0x03:
			//���µ��
			if(Motor == 0x01 && Motor_Mode == 0x01)		//����1�Ž��µ��
			{
				//����1�Ž��µ��������
				
			}
			else if(Motor == 0x01 && Motor_Mode == 0x00)//�ر�1�Ž��µ��
			{
				//�ر�1�Ž��µ��������
				
			}
			udelete(FRAMELENGTH);
			break;
		case 0x04:
			//������
			if(Motor == 0x01 && Motor_Mode == 0x01)		//����1�ż�����
			{
				//����1�ż�����������
				
			}
			else if(Motor == 0x01 && Motor_Mode == 0x00)//�ر�1�ż�����
			{
				//�ر�1�ż�����������
				
			}
			udelete(FRAMELENGTH);
			break;
		case 0x05:
			//�Ӽ���
			if(Motor == 0x01 && Motor_Mode == 0x01)		//����1�żӼ���
			{
				//����1�żӼ���������
				
			}
			else if(Motor == 0x01 && Motor_Mode == 0x00)//�ر�1�żӼ���
			{
				//�ر�1�żӼ���������
				
			}
			udelete(FRAMELENGTH);
			break;
		case 0x06:
			//�¶������趨
			TP_Low = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x07:
			//�¶�����
			TP_Upper = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x08:
			//PH����
			PH_Low = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x09:
			//PH����
			PH_Upper = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x10:
			//�ܽ�������
			OX_Low = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x11:
			//�ܽ�������
			OX_Upper = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x12:
			//�¶��趨ֵ
			TP_Set = Set_Data;
			udelete(FRAMELENGTH);
			break;
		case 0x13:
			//PH�趨ֵ
			PH_Set = Set_Data;
			udelete(FRAMELENGTH);
			break;
	}	
}
