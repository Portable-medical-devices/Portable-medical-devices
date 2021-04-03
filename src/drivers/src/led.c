/****************************************************************************************
@file     led.c
@brief    LED驱动文件
@author   石国强
@version  V1.0
@introduction LED->PC13
*****************************************************************************************/

#include "led.h"

void LED_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;                   //实例化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   //GPIOC初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;               //LED->PC13
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;         //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;        //推挽输出50HMz
	GPIO_Init(GPIOC,&GPIO_InitStructure);                  //初始化GPIOC,13
	GPIO_SetBits(GPIOC,GPIO_Pin_13);                       //PC13输出高
}
