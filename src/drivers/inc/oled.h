/****************************************************************************************
@file     oled.h
@brief    oled驱动头文件
@author   石国强
@version  V1.0
@introduction D0->PA5 D1->PA7 RES->PB0 DC->PB1 CS->GND
*****************************************************************************************/

#ifndef __OLED_H__
#define __OLED_H__

#include "stm32f10x_gpio.h"
#include "delay.h"

#define OLED_CMD  0                             //写命令
#define OLED_DATA 1                             //写数据

//RESET宏函数
#define OLED_RESET(x) { \
	if(x) { GPIO_SetBits(GPIOB,GPIO_Pin_0);   } \
	else  { GPIO_ResetBits(GPIOB,GPIO_Pin_0); } \
}
	
//DC宏函数
#define OLED_DC(x) { \
	if(x) { GPIO_SetBits(GPIOB,GPIO_Pin_1);   } \
	else  { GPIO_ResetBits(GPIOB,GPIO_Pin_1); } \
}

//D0(SCK)宏函数
#define OLED_SCK(x) { \
	if(x) { GPIO_SetBits(GPIOA,GPIO_Pin_5);   } \
	else  { GPIO_ResetBits(GPIOA,GPIO_Pin_5); } \
}

//D1(MOSI)宏函数
#define OLED_MOSI(x) { \
	if(x) { GPIO_SetBits(GPIOA,GPIO_Pin_7);   } \
	else  { GPIO_ResetBits(GPIOA,GPIO_Pin_7); } \
}

extern u8 OLED_GRAM[128][8];                              //显存
                                                      
void OLED_Reset(void);                                    //OLED复位
void OLED_Init(void);                                     //OLED初始化
void OLED_WR_Byte(u8 data,u8 cmd);                        //写数据
void OLED_Clear(void);                                    //清屏函数
void OLED_Full(void);                                     //全亮
void OLED_DisplayTurn(u8 i);                              //屏幕旋转180度
void OLED_Display_On(void);                               //开启OLED显示
void OLED_Display_Off(void);                              //关闭OLED显示
void OLED_Refresh(void);                                  //根据显存更新
void OLED_DrawPoint(u8 x,u8 y);                           //画点
void OLED_ClearPoint(u8 x,u8 y);                          //清除一个点
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2);              //画线
void OLED_ColorTurn(u8 i);                                //显示模式
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size);             //显示字符
void OLED_ShowString(u8 x,u8 y,char *chr,u8 size);        //显示字符串
void OLED_ShowNum(u8 x,u8 y,float num,u8 size);           //显示数字
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);  //显示图片

#endif

