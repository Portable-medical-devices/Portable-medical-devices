/****************************************************************************************
@file     main.c
@brief    工程主函数文件
@author   石国强
@version  V1.0
@introduction 陀螺仪、计步器测试任务
*****************************************************************************************/

//UCOSIII中以下优先级用户程序不能使用，ALIENTEK
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()

#include "stm32f10x.h"
#include "includes.h"
#include "led.h"
#include "beep.h"
#include "delay.h"
#include "oled.h"
#include "oledbmp.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "stdio.h"

//start任务
//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//led任务
//任务优先级
#define LED_TASK_PRIO		7
//任务堆栈大小	
#define LED_STK_SIZE 		128
//任务控制块
OS_TCB LedTaskTCB;
//任务堆栈	
CPU_STK LED_TASK_STK[LED_STK_SIZE];
void led_task(void *p_arg);

//beep任务
//任务优先级
#define BEEP_TASK_PRIO		5
//任务堆栈大小	
#define BEEP_STK_SIZE 		128
//任务控制块
OS_TCB BeepTaskTCB;
//任务堆栈	
CPU_STK BEEP_TASK_STK[BEEP_STK_SIZE];
void beep_task(void *p_arg);

//OLED任务
//任务优先级
#define OLED_TASK_PRIO		6
//任务堆栈大小	
#define OLED_STK_SIZE 		500
//任务控制块
OS_TCB OledTaskTCB;
//任务堆栈	
CPU_STK OLED_TASK_STK[OLED_STK_SIZE];
void oled_task(void *p_arg);

int main() {
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组
	
	LED_Init();         //硬件初始化
	BEEP_Init();        //蜂鸣器初始化
	OLED_Init();        //OLED初始化
	//OLED_ShowNum(0,0,-123.22,16);
	//OLED_Refresh();
	MPU6050_Init();     //MPU6050初始化
	OSInit(&err);	    //初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	  
	OSStart(&err);  //开启UCOSIII
	while(1);
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区
	//创建LED任务
	OSTaskCreate((OS_TCB 	* )&LedTaskTCB,		
				 (CPU_CHAR	* )"led task", 		
                 (OS_TASK_PTR )led_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED_TASK_PRIO,     
                 (CPU_STK   * )&LED_TASK_STK[0],	
                 (CPU_STK_SIZE)LED_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);				
				 
	//创建BEEP
	OSTaskCreate((OS_TCB 	* )&BeepTaskTCB,		
				 (CPU_CHAR	* )"beeptask", 		
                 (OS_TASK_PTR )beep_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )BEEP_TASK_PRIO,     	
                 (CPU_STK   * )&BEEP_TASK_STK[0],	
                 (CPU_STK_SIZE)BEEP_STK_SIZE/10,	
                 (CPU_STK_SIZE)BEEP_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
	//创建OLED
	OSTaskCreate((OS_TCB 	* )&OledTaskTCB,		
				 (CPU_CHAR	* )"oledtask", 		
                 (OS_TASK_PTR )oled_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )OLED_TASK_PRIO,     	
                 (CPU_STK   * )&OLED_TASK_STK[0],	
                 (CPU_STK_SIZE)OLED_STK_SIZE/10,	
                 (CPU_STK_SIZE)OLED_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
			 							 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//退出临界区
}

//led任务函数
void led_task(void *p_arg) {
	OS_ERR err;
	p_arg = p_arg;
	while(1) {
		LED(0);
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
		LED(1);
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
	}
}

	char buf[20] ={0};

//oled任务函数
void oled_task(void *p_arg) {
	u8 i,size;
	char buf[3][20]={0};
//	float pitch,roll,yaw;  
	OS_ERR err;
	p_arg = p_arg;
	OLED_Clear();                                               //OLED清屏
	OLED_ShowPicture(0,0,128,8,LOGO);                           //显示LOGO
	OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);      //延时500ms
	OLED_ShowString(0,0," MPU6050 [init]",16);
	OLED_Refresh();                                             //OLED刷屏
	while(mpu_dmp_init()) {                                     //DMP初始化
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);    //延时5ms
	}
	OLED_ShowString(0,0," MPU6050 [OK]  ",16);
	OLED_Refresh();                                             //OLED刷屏
	OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);      //延时500ms
	OLED_Clear();                                               //清屏
	while(1) {
//		while(mpu_dmp_get_data(&pitch,&roll,&yaw)!=0)
//			OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err); 
//		sprintf(buf[0],"pitch:%f",pitch);
//		sprintf(buf[1],"roll:%f",roll);
//		sprintf(buf[2],"yaw:%f",yaw);
//		size=12;
//		for(i=0;i<3;++i) {
//			OLED_ShowString(0,0+(size/8+(size%8?1:0))*8*i,buf[i],size);
//		}
		MPU_Step_Count();
		sprintf(buf[0],"Sports:%d step",(int)walk.step);         //显示步数
		sprintf(buf[1],"Distance:%.0f m",walk.distance);         //显示距离
		size=16;
		for(i=0;i<2;++i) {
			OLED_ShowString(0,0+(size/8+(size%8?1:0))*8*i,buf[i],size);
		}		
		OLED_Refresh(); 
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);     //延时5ms
	}
}

//beep任务函数
void beep_task(void *p_arg) {
	int i;
	OS_ERR err;
	p_arg = p_arg;
	while(1) {
		for(i=0;i<10;++i) {
			BEEP(1);
			OSTimeDlyHMSM(0,0,0,25,OS_OPT_TIME_HMSM_STRICT,&err); //延时50ms
			BEEP(0);
			OSTimeDlyHMSM(0,0,0,25,OS_OPT_TIME_HMSM_STRICT,&err); //延时50ms
		}
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);    //延时500ms
	}
}



