/****************************************************************************************
@file     beep.c
@brief    beep�����ļ�
@author   ʯ��ǿ
@version  V1.0
@introduction BEEP->PC14
*****************************************************************************************/

#include "beep.h"

void BEEP_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;                   //ʵ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   //GPIOC��ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;               //BEEP->PC14
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;         //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;        //�������50HMz
	GPIO_Init(GPIOC,&GPIO_InitStructure);                  //��ʼ��GPIOC,14
	GPIO_ResetBits(GPIOC,GPIO_Pin_14);                     //PC14�����
}
