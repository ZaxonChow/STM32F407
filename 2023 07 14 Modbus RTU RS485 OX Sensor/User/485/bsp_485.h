#ifndef __485_H
#define	__485_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "./timer/bsp_timer.h"
#include "math.h"


#define _485_USART                              USART2
#define _485_USART_CLK                          RCC_APB1Periph_USART2
#define _485_USART_BAUDRATE                     9600
 
#define _485_USART_RX_GPIO_PORT                 GPIOA
#define _485_USART_RX_GPIO_CLK                  RCC_AHB1Periph_GPIOA
#define _485_USART_RX_PIN                       GPIO_Pin_3
#define _485_USART_RX_AF                        GPIO_AF_USART2
#define _485_USART_RX_SOURCE                    GPIO_PinSource3

#define _485_USART_TX_GPIO_PORT                 GPIOA
#define _485_USART_TX_GPIO_CLK                  RCC_AHB1Periph_GPIOA
#define _485_USART_TX_PIN                       GPIO_Pin_2
#define _485_USART_TX_AF                        GPIO_AF_USART2
#define _485_USART_TX_SOURCE                  	GPIO_PinSource2

#define _485_RE_GPIO_PORT						GPIOC
#define _485_RE_GPIO_CLK						RCC_AHB1Periph_GPIOC
#define _485_RE_PIN								GPIO_Pin_0

#define _485_INT_IRQ                 			USART2_IRQn
#define _485_IRQHandler                         USART2_IRQHandler


// ����ȷ����ʱ
static void _485_delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 


/*�����շ�����*/
//�������ģʽ,����Ҫ����ʱ�ȴ�485����������
#define _485_RX_EN()			_485_delay(1000); GPIO_ResetBits(_485_RE_GPIO_PORT,_485_RE_PIN);  _485_delay(1000);
//���뷢��ģʽ,����Ҫ����ʱ�ȴ�485����������
#define _485_TX_EN()			_485_delay(1000); GPIO_SetBits(_485_RE_GPIO_PORT,_485_RE_PIN);  _485_delay(1000);



/*debug*/

#define _485_DEBUG_ON          1
#define _485_DEBUG_ARRAY_ON    1
#define _485_DEBUG_FUNC_ON     1
   
   
// Log define
#define _485_INFO(fmt,arg...)           printf("<<-_485-INFO->> "fmt"\n",##arg)
#define _485_ERROR(fmt,arg...)          printf("<<-_485-ERROR->> "fmt"\n",##arg)
#define _485_DEBUG(fmt,arg...)          do{\
											 if(_485_DEBUG_ON)\
											 printf("<<-_485-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
										 }while(0)

#define _485_DEBUG_ARRAY(array, num)    do{\
                                         int32_t i;\
                                         uint8_t* a = array;\
                                         if(_485_DEBUG_ARRAY_ON)\
                                         {\
                                            printf("<<-_485-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printf("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printf("\n");\
                                                }\
                                            }\
                                            printf("\n");\
                                        }\
                                       }while(0)

#define _485_DEBUG_FUNC()               do{\
                                         if(_485_DEBUG_FUNC_ON)\
                                         printf("<<-_485-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


void _485_Config(void);
void _485_SendByte(  uint8_t ch );
void _485_SendStr_length( uint8_t *str,uint32_t strlen );
void _485_SendString(  uint8_t *str);



void bsp_485_IRQHandler(void);
char *get_rebuff(uint16_t *len);
void clean_rebuff(void);
									   
void Modbus_RTU(void);
#endif /* __485_H */

