#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

#define STR_LENGTH 		100
#define RINGBUFF_LEN	500     //定义最大接收字节数 500

#define usize 		getRingBuffLenght()
#define code_c() 	initRingBuff()
#define udelete(x) 	deleteRingBuff(x)
#define u(x) 		read1BFromRingBuff(x)

typedef struct
{
    uint16_t Head;
    uint16_t Tail;
    uint16_t Length;
    uint8_t  Ring_data[RINGBUFF_LEN];
}RingBuff_t;



//USART1 电脑调试引脚定义
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK                         RCC_APB2Periph_USART1
#define DEBUG_USART_BAUDRATE                    9600  //串口波特率

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_RX_PIN                      GPIO_Pin_10
#define DEBUG_USART_RX_AF                       GPIO_AF_USART1
#define DEBUG_USART_RX_SOURCE                   GPIO_PinSource10

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_TX_PIN                      GPIO_Pin_9
#define DEBUG_USART_TX_AF                       GPIO_AF_USART1
#define DEBUG_USART_TX_SOURCE                   GPIO_PinSource9
/************************************************************/

void TJCPrintf (const char *str, ...);
void initRingBuff(void);
void writeRingBuff(uint8_t data);
void deleteRingBuff(uint16_t size);
uint16_t getRingBuffLenght(void);
uint8_t read1BFromRingBuff(uint16_t position);

void Debug_USART1_Config(void);
void Debug_USART6_Config(void);
void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString(USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord(USART_TypeDef * pUSARTx, uint16_t ch);

#endif  //__BSP_USART_H
