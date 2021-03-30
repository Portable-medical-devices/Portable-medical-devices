/****************************************************************************************
@file     beep.c
@brief    BEEPͷ�ļ�
@author   ʯ��ǿ
@version  V1.0
@function BEEP(1)��������
*****************************************************************************************/

#ifndef __BEEP_H__
#define __BEEP_H__

#include "stm32f10x_gpio.h"

#define BEEP(x) \
 if(x) GPIO_SetBits(GPIOC,GPIO_Pin_14); \
 else  GPIO_ResetBits(GPIOC,GPIO_Pin_14);

void BEEP_Init(void);

#endif
