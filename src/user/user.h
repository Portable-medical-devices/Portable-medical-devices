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
#include "hc05.h"
#include "ads1292.h"
#include "math.h"
#include "arm_math.h"
#include "fdacoefs.h"
#include "stm32f10x_exti.h"
#include "dma.h"

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

//start����
//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		512
//������ƿ�
extern OS_TCB StartTaskTCB;
//�����ջ	
extern CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//OLED����
//�������ȼ�
#define OLED_TASK_PRIO		4
//�����ջ��С	
#define OLED_STK_SIZE 		128
//������ƿ�
extern OS_TCB OledTaskTCB;
//�����ջ	
extern CPU_STK OLED_TASK_STK[OLED_STK_SIZE];
void oled_task(void *p_arg);

//�������ȼ�
#define KEYPROCESS_TASK_PRIO 	3
//�����ջ��С	
#define KEYPROCESS_STK_SIZE 	128
//������ƿ�
extern OS_TCB Keyprocess_TaskTCB;
//�����ջ	
extern CPU_STK KEYPROCESS_TASK_STK[KEYPROCESS_STK_SIZE];
//������
void Keyprocess_task(void *p_arg);

////////////////////////��Ϣ����//////////////////////////////
#define KEYMSG_Q_NUM	1	//������Ϣ���е�����
extern OS_Q KEY_Msg;				//����һ����Ϣ���У����ڰ�����Ϣ���ݣ�ģ����Ϣ����

void Software_Init(void);          //�����ʼ��
void Dirver_Init(void);            //Ӳ����ʼ��
void Show_Logo(void);              //��ʾLogo
void Show_LMT70_Init(void);        //��ʾ��ʼ��LMT70
void Show_MPU6050_Init(void);      //��ʾ��ʼ��MPU6050
void Show_HC05_Init(void);         //��ʾ��ʼ��HC05
void Show_ADS1292_Init(void);      //��ʾ��ʼ��ADS1292
void Show_Menu(void);              //��ʾ�˵�
void Show_Temputre(void);          //��ʾ�¶�
void Show_Step(void);              //��ʾ�ƶ���Ϣ
u8   Scan_Key(void);               //ɨ�谴��
void Change_Mode(u8 y);            //ģʽ�л�
void Applicaton(void);             //Ӧ��

extern User user;                  //�û�����

#endif
