/****************************************************************************************
@file     user.h
@brief    用户操作函数
@author   石国强
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
	unsigned long   step;           //步数
	float           distance;       //距离
}Walk;

typedef struct {
	Walk walk;                     //行走数据
	float temputre;                //温度数据
	Mode mode;                     //模式数据
}User;

//start任务
//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
extern OS_TCB StartTaskTCB;
//任务堆栈	
extern CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//OLED任务
//任务优先级
#define OLED_TASK_PRIO		4
//任务堆栈大小	
#define OLED_STK_SIZE 		128
//任务控制块
extern OS_TCB OledTaskTCB;
//任务堆栈	
extern CPU_STK OLED_TASK_STK[OLED_STK_SIZE];
void oled_task(void *p_arg);

//任务优先级
#define KEYPROCESS_TASK_PRIO 	3
//任务堆栈大小	
#define KEYPROCESS_STK_SIZE 	128
//任务控制块
extern OS_TCB Keyprocess_TaskTCB;
//任务堆栈	
extern CPU_STK KEYPROCESS_TASK_STK[KEYPROCESS_STK_SIZE];
//任务函数
void Keyprocess_task(void *p_arg);

////////////////////////消息队列//////////////////////////////
#define KEYMSG_Q_NUM	1	//按键消息队列的数量
extern OS_Q KEY_Msg;				//定义一个消息队列，用于按键消息传递，模拟消息邮箱

void Software_Init(void);          //软件初始化
void Dirver_Init(void);            //硬件初始化
void Show_Logo(void);              //显示Logo
void Show_LMT70_Init(void);        //显示初始化LMT70
void Show_MPU6050_Init(void);      //显示初始化MPU6050
void Show_HC05_Init(void);         //显示初始化HC05
void Show_ADS1292_Init(void);      //显示初始化ADS1292
void Show_Menu(void);              //显示菜单
void Show_Temputre(void);          //显示温度
void Show_Step(void);              //显示移动信息
u8   Scan_Key(void);               //扫描按键
void Change_Mode(u8 y);            //模式切换
void Applicaton(void);             //应用

extern User user;                  //用户数据

#endif
