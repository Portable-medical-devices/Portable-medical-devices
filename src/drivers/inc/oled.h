/****************************************************************************************
@file     oled.h
@brief    oled����ͷ�ļ�
@author   ʯ��ǿ
@version  V1.0
@introduction D0->PA5 D1->PA7 RES->PB0 DC->PB1 CS->GND
*****************************************************************************************/

#ifndef __OLED_H__
#define __OLED_H__

#include "stm32f10x_gpio.h"
#include "delay.h"

#define OLED_CMD  0                             //д����
#define OLED_DATA 1                             //д����

//RESET�꺯��
#define OLED_RESET(x) { \
	if(x) { GPIO_SetBits(GPIOB,GPIO_Pin_0);   } \
	else  { GPIO_ResetBits(GPIOB,GPIO_Pin_0); } \
}
	
//DC�꺯��
#define OLED_DC(x) { \
	if(x) { GPIO_SetBits(GPIOB,GPIO_Pin_1);   } \
	else  { GPIO_ResetBits(GPIOB,GPIO_Pin_1); } \
}

//D0(SCK)�꺯��
#define OLED_SCK(x) { \
	if(x) { GPIO_SetBits(GPIOA,GPIO_Pin_5);   } \
	else  { GPIO_ResetBits(GPIOA,GPIO_Pin_5); } \
}

//D1(MOSI)�꺯��
#define OLED_MOSI(x) { \
	if(x) { GPIO_SetBits(GPIOA,GPIO_Pin_7);   } \
	else  { GPIO_ResetBits(GPIOA,GPIO_Pin_7); } \
}

extern u8 OLED_GRAM[128][8];                              //�Դ�
                                                      
void OLED_Reset(void);                                    //OLED��λ
void OLED_Init(void);                                     //OLED��ʼ��
void OLED_WR_Byte(u8 data,u8 cmd);                        //д����
void OLED_Clear(void);                                    //��������
void OLED_Full(void);                                     //ȫ��
void OLED_DisplayTurn(u8 i);                              //��Ļ��ת180��
void OLED_Display_On(void);                               //����OLED��ʾ
void OLED_Display_Off(void);                              //�ر�OLED��ʾ
void OLED_Refresh(void);                                  //�����Դ����
void OLED_DrawPoint(u8 x,u8 y);                           //����
void OLED_ClearPoint(u8 x,u8 y);                          //���һ����
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2);              //����
void OLED_ColorTurn(u8 i);                                //��ʾģʽ
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size);             //��ʾ�ַ�
void OLED_ShowString(u8 x,u8 y,char *chr,u8 size);        //��ʾ�ַ���
void OLED_ShowNum(u8 x,u8 y,float num,u8 size);           //��ʾ����
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);  //��ʾͼƬ

#endif

