#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "stm32f4xx.h"                  // Device header            

//���Ŷ���
/*******************************************************/
//R ��ɫ��
#define LED1_PIN                  GPIO_Pin_6                 
#define LED1_GPIO_PORT            GPIOF                      
#define LED1_GPIO_CLK             RCC_AHB1Periph_GPIOF

//G ��ɫ��
#define LED2_PIN                  GPIO_Pin_7                 
#define LED2_GPIO_PORT            GPIOF                      
#define LED2_GPIO_CLK             RCC_AHB1Periph_GPIOF

//B ��ɫ��
#define LED3_PIN                  GPIO_Pin_8                 
#define LED3_GPIO_PORT            GPIOF                       
#define LED3_GPIO_CLK             RCC_AHB1Periph_GPIOF
/************************************************************/


/** ����LED������ĺ꣬
	* LED�͵�ƽ��������ON=0��OFF=1
	* ��LED�ߵ�ƽ�����Ѻ����ó�ON=1 ��OFF=0 ����
	*/
#define ON  0
#define OFF 1

/* ���κ꣬��������������һ��ʹ�� */
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_PIN)

#define LED3(a)	if (a)	\
					GPIO_SetBits(LED3_GPIO_PORT,LED3_PIN);\
					else		\
					GPIO_ResetBits(LED3_GPIO_PORT,LED3_PIN)

/* �������IO�ĺ� */
#define LED1_TOGGLE		GPIO_ToggleBits(LED1_GPIO_PORT,LED1_PIN)	//����Ϊ�ߵ�ƽ
#define LED1_OFF		GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN)		//����͵�ƽ
#define LED1_ON			GPIO_ResetBits(LED1_GPIO_PORT,LED1_PIN)		//�����ת״̬

#define LED2_TOGGLE		GPIO_ToggleBits(LED2_GPIO_PORT,LED2_PIN)
#define LED2_OFF		GPIO_SetBits(LED2_GPIO_PORT,LED2_PIN)
#define LED2_ON			GPIO_ResetBits(LED2_GPIO_PORT,LED2_PIN)

#define LED3_TOGGLE		GPIO_ToggleBits(LED3_GPIO_PORT,LED3_PIN)
#define LED3_OFF		GPIO_SetBits(LED3_GPIO_PORT,LED3_PIN)
#define LED3_ON			GPIO_ResetBits(LED3_GPIO_PORT,LED3_PIN)

/* ������ɫ������߼��÷�ʹ��PWM�ɻ��ȫ����ɫ,��Ч������ */

//��
#define LED_RED  \
					LED1_ON;\
					LED2_OFF;\
					LED3_OFF

//��
#define LED_GREEN		\
					LED1_OFF;\
					LED2_ON;\
					LED3_OFF

//��
#define LED_BLUE	\
					LED1_OFF;\
					LED2_OFF;\
					LED3_ON

					
//��(��+��)					
#define LED_YELLOW	\
					LED1_ON;\
					LED2_ON;\
					LED3_OFF
//��(��+��)
#define LED_PURPLE	\
					LED1_ON;\
					LED2_OFF;\
					LED3_ON

//��(��+��)
#define LED_CYAN \
					LED1_OFF;\
					LED2_ON;\
					LED3_ON
					
//��(��+��+��)
#define LED_WHITE	\
					LED1_ON;\
					LED2_ON;\
					LED3_ON
					
//��(ȫ���ر�)
#define LED_RGBOFF	\
					LED1_OFF;\
					LED2_OFF;\
					LED3_OFF		




void LED_GPIO_Config(void);

#endif  //_BSP_LED_H
