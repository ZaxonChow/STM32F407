#ifndef _SCREEN_H
#define _SCREEN_H

#include "stm32f4xx.h"
#include "./usart/bsp_usart.h"
#include "delay.h"
#include "./modbus_rtu/bsp_master.h"

#define   	  AT	"AT\r\n"
#define 	AT_T 	"AT+T\r\n"

void Screen(void);
void Get_Status(void);

#endif
