/****************************************************************************************
@file     user.h
@brief    �û���������
@author   ʯ��ǿ
@version  V1.0
@introduction  
*****************************************************************************************/

#ifndef __USER_H__
#define __USER_H__

#include "led.h"
#include "beep.h"
#include "delay.h"
#include "oled.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "stdio.h"
#include "adc.h"
#include "math.h"
#include "usart.h"
#include "mymath.h"
#include "oledbmp.h"
#include "includes.h"
#include "stm32f10x.h"
#include "key.h"

typedef enum {
	TEMPUTRE_MODE,
	STEP_MODE,
	HART_MODE,
	DEFAULT,
}Mode;

typedef struct {
	unsigned long   step;           //����
	float           distance;       //����
}Walk;

typedef struct {
	Walk walk;                     //��������
	float temputre;                //�¶�����
	Mode mode;                     //ģʽ����
}User;

void Software_Init(void);          //������ʼ��
void Dirver_Init(void);            //Ӳ����ʼ��
void Show_Logo(void);              //��ʾLogo
void Show_LMT70_Init(void);        //��ʾ��ʼ��LMT70
void Show_MPU6050_Init(void);      //��ʾ��ʼ��MPU6050
void Show_Menu(void);              //��ʾ�˵�
void Show_Temputre(void);          //��ʾ�¶�
void Show_Step(void);              //��ʾ�ƶ���Ϣ
u8   Scan_Key(void);               //ɨ�谴��
void Change_Mode(u8 y);            //ģʽ�л�
void Applicaton(void);             //Ӧ��

extern User user;                  //�û�����

#endif