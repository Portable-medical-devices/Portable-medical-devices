/****************************************************************************************
@file     key.c
@brief    KEY驱动源文件
@author   石国强
@version  V1.0
@introduction KEY1->PA11 KEY2->PA12
*****************************************************************************************/

#include "key.h"

void KEY_Init(void) {                                       //按键初始化
	GPIO_InitTypeDef GPIO_InitStructure;                    //GPIO实例化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //使能GPIOA
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;    //PA11/PA12  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;             //上拉模式
	GPIO_Init(GPIOA,&GPIO_InitStructure);                   //初始化GPIOA
}
