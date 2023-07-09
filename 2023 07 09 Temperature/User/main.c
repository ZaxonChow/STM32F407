/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   串口接发测试，串口接收到数据后马上回传。
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "bsp_adc.h"
#include "math.h"

__IO uint16_t ADC_ConvertedValue;

// PT100温度转换函数
float RTD2Temperature(float R) {
    float temperature = 0.0;
    float RTD_A = 3.9083e-003f;
    float RTD_B = -5.775e-007f;

    temperature = (-RTD_A + sqrtf(RTD_A * RTD_A - 4 * RTD_B * (1 - R / 100))) / (2 * RTD_B);
    return temperature;
}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
	/*初始化USART 配置模式为 115200 8-N-1，中断接收*/
	Debug_USART_Config();
	Rheostat_Init();	

	while (1)
	{
		float resistance = 110; 							// 铂电阻的电阻值
        float temperature = RTD2Temperature(resistance); 	// 算出温度值
		printf("\r\n Temperature: %.1f C\r\n",temperature);      
		Delay(0xffffee);  
	}	
}



/*********************************************END OF FILE**********************/

