/****************************************************************************************
@file     HC05.c
@brief    蓝牙驱动源文件
@author   石国强
@version  V1.0
@introduction HC05(RX)->PA2(TX) HC05(TX)->PA3(RX) USART2
*****************************************************************************************/

#ifndef __HC05_H__
#define __HC05_H__

#include "user.h"

#define USART2_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN];   //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		    //接收状态标记	

void HC05_Init(u32 bound);                  //HC05初始化
void HC05_Send_Data(u8 *buf,u8 len);        //HC05发送数据

#endif
