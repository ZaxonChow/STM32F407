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
#include "math.h"

__IO uint16_t ADC_ConvertedValue;

// PT100�¶�ת������
float RTD2Temperature(float R) {
    float temperature = 0.0;
    float RTD_A = 3.9083e-003f;
    float RTD_B = -5.775e-007f;

    temperature = (-RTD_A + sqrtf(RTD_A * RTD_A - 4 * RTD_B * (1 - R / 100))) / (2 * RTD_B);
    return temperature;
}

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
		float resistance = 110; 							// ������ĵ���ֵ
        float temperature = RTD2Temperature(resistance); 	// ����¶�ֵ
		printf("\r\n Temperature: %.1f C\r\n",temperature);      
		Delay(0xffffee);  
	}	
}



/*********************************************END OF FILE**********************/

