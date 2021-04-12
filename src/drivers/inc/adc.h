/****************************************************************************************
@file     adc.h
@brief    ADCͷ�ļ�
@author   ʯ��ǿ
@version  V1.0
@function ADC1 1 -> PA1
*****************************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__

#include "sys.h"
#include "stm32f10x_adc.h"

void Adc_Init(void);                      //ADC��ʼ��
u16  Get_Adc(u8 ch);                      //��ȡchͨ����ADCת����ֵ
u16  Get_Adc_Average(u8 ch,u8 times);     //ƽ��times���ȡ�ĵ�ѹ
u16  Get_Adc_Middle(u8 ch,u16 times);      //��ֵ��ȡ�ĵ�ѹ

#endif
