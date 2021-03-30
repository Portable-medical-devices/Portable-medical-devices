/****************************************************************************************
@file     led.h
@brief    LEDͷ�ļ�
@author   ʯ��ǿ
@version  V1.0
@function LED0(0) ����
*****************************************************************************************/

#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x_gpio.h"

#define LED(x) \
 if(x) GPIO_SetBits(GPIOC,GPIO_Pin_13); \
 else  GPIO_ResetBits(GPIOC,GPIO_Pin_13);

void LED_Init(void);

#endif
