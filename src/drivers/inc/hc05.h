/****************************************************************************************
@file     HC05.c
@brief    ��������Դ�ļ�
@author   ʯ��ǿ
@version  V1.0
@introduction HC05(RX)->PA2(TX) HC05(TX)->PA3(RX) USART2
*****************************************************************************************/

#ifndef __HC05_H__
#define __HC05_H__

#include "user.h"

#define USART2_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN];   //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		    //����״̬���	

void HC05_Init(u32 bound);                  //HC05��ʼ��
void HC05_Send_Data(u8 *buf,u8 len);        //HC05��������

#endif
