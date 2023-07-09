/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ���ڽӷ����ԣ����ڽ��յ����ݺ����ϻش���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue;

float ADC_Vol; 

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
	/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	Debug_USART_Config();
	Rheostat_Init();	

	while (1)
	{
		ADC_Vol =(float)ADC_ConvertedValue / 4096 * (float)3.3; // ��ȡת����ADֵ
		printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue);  
		printf("\r\n The current AD value = %f V \r\n",ADC_Vol);      
		Delay(0xffffee);  
	}	
}



/*********************************************END OF FILE**********************/

