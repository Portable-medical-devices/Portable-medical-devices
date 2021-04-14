/****************************************************************************************
@file     key.c
@brief    KEY����Դ�ļ�
@author   ʯ��ǿ
@version  V1.0
@introduction KEY1->PA11 KEY2->PA12
*****************************************************************************************/

#include "key.h"

void KEY_Init(void) {                                       //������ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;                    //GPIOʵ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //ʹ��GPIOA
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;    //PA11/PA12  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;             //����ģʽ
	GPIO_Init(GPIOA,&GPIO_InitStructure);                   //��ʼ��GPIOA
}
