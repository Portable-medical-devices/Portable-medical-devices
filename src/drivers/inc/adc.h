/****************************************************************************************
@file     adc.h
@brief    ADC头文件
@author   石国强
@version  V1.0
@function ADC1 1 -> PA1
*****************************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__

#include "sys.h"
#include "stm32f10x_adc.h"

void Adc_Init(void);                      //ADC初始化
u16  Get_Adc(u8 ch);                      //获取ch通道的ADC转换的值
u16  Get_Adc_Average(u8 ch,u8 times);     //平均times后获取的电压
u16  Get_Adc_Middle(u8 ch,u16 times);      //中值获取的电压

#endif
