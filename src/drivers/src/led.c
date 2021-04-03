/****************************************************************************************
@file     led.c
@brief    LED�����ļ�
@author   ʯ��ǿ
@version  V1.0
@introduction LED->PC13
*****************************************************************************************/

#include "led.h"

void LED_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;                   //ʵ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   //GPIOC��ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;               //LED->PC13
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;         //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;        //�������50HMz
	GPIO_Init(GPIOC,&GPIO_InitStructure);                  //��ʼ��GPIOC,13
	GPIO_SetBits(GPIOC,GPIO_Pin_13);                       //PC13�����
}
