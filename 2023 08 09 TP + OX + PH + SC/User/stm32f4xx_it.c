/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.1
  * @date    27-January-2022
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "bsp_usart.h"
#include "./screen/screen.h"
#include "./timer/bsp_timer.h"
#include "./modbus_rtu/bsp_master.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

extern uint8_t RxBuff[1];

/********************************************************
��������  	USART1_IRQHandler
���ܣ�    	���ڽ����ж�,���մ���������������
**********************************************************/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		RxBuff[0] = USART_ReceiveData(USART1);
		writeRingBuff(RxBuff[0]);
	}
}

extern int8_t Get_Temperature[20];
uint8_t flag=0;

/********************************************************
��������  	USART6_IRQHandler
���ܣ�    	���ڽ����ж�,���� PT100 �¶�����
**********************************************************/
void USART6_IRQHandler(void)
{
	static uint8_t i = 0;
	if(USART_GetFlagStatus(USART6, USART_FLAG_RXNE) != RESET)
	{
		Get_Temperature[i] = USART_ReceiveData(USART6);
		i++;
		if(Get_Temperature[i - 1]=='\n')
		{
			Get_Temperature[i] = '\0';
			i = 0;
			flag = 1;
		}
	}	
}

extern uint32_t RS485_RX_BUFF[20];	//���ջ�����
extern uint16_t RS485_RX_CNT;
extern uint8_t  RS485_RxFlag;

/********************************************************
��������  	USART2_IRQHandler
���ܣ�    	���ڽ����ж�,�����ܽ�����PH�������ɼ�������
Э�飺		Modbus RTU RS485
**********************************************************/
void USART2_IRQHandler(void)//����2�жϷ�����򣬽��մӻ�����
{  	   
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		RS485_RX_BUFF[RS485_RX_CNT] = USART_ReceiveData(USART2); 	//�����յ����ֽڣ�ͬʱ��ر�־�Զ����
		RS485_RX_CNT++;
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);	//�����ʱ������ж�
		TIM_SetCounter(TIM7, 0);					//�����յ�һ���µ��ֽڣ�����ʱ��7��λΪ0�����¼�ʱ
		TIM_Cmd(TIM7, ENABLE);						//��ʼ��ʱ
	}
}


/***************************************************************
��������  	TIM4_IRQHandler
���ܣ�    	��ʱ�� 4 ��ʱ�ж�,��ʱ500ms���л���ȡ�ܽ�����PH������
****************************************************************/
void TIM4_IRQHandler(void)   //TIM4�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)	//���ָ����TIM�жϷ������
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);		//���TIM4���жϴ�����λ
		Modbus_RTU();									//�л�������
	}
}

/***************************************************************
��������  	TIM7_IRQHandler
���ܣ�    	��ʱ�� 7 ��ʱ�ж�,
			�жϽ��տ���ʱ��,������ʱ�����ָ��ʱ�䣬��Ϊһ֡����
****************************************************************/         
void TIM7_IRQHandler(void)
{                                                                   
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);	//����жϱ�־
		TIM_Cmd(TIM7, DISABLE);						//һ֡������ֹͣ��ʱ��
		_485_TX_EN();								//ֹͣ���գ��л�Ϊ����״̬��PC0
		RS485_RxFlag = 1;							//��λ֡�������
	}
}
