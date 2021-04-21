/****************************************************************************************
@file     main.c
@brief    工程主函数文件
@author   石国强
@version  V1.0
@introduction 测温任务
*****************************************************************************************/

//UCOSIII中以下优先级用户程序不能使用，ALIENTEK
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
#include "user.h"

__align(8)

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

//OLED任务
//任务优先级
#define OLED_TASK_PRIO		4
//任务堆栈大小	
#define OLED_STK_SIZE 		1024
//任务控制块
OS_TCB OledTaskTCB;
//任务堆栈	
CPU_STK OLED_TASK_STK[OLED_STK_SIZE];
void oled_task(void *p_arg);

//任务优先级
#define KEYPROCESS_TASK_PRIO 	3
//任务堆栈大小	
#define KEYPROCESS_STK_SIZE 	128
//任务控制块
OS_TCB Keyprocess_TaskTCB;
//任务堆栈	
CPU_STK KEYPROCESS_TASK_STK[KEYPROCESS_STK_SIZE];
//任务函数
void Keyprocess_task(void *p_arg);

////////////////////////消息队列//////////////////////////////
#define KEYMSG_Q_NUM	1	//按键消息队列的数量
OS_Q KEY_Msg;				//定义一个消息队列，用于按键消息传递，模拟消息邮箱

int main() {
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组
	Software_Init();    //软件初始化
	Dirver_Init();      //硬件初始化
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
	//创建消息队列KEY_Msg
	OSQCreate ((OS_Q*		)&KEY_Msg,	//消息队列
                (CPU_CHAR*	)"KEY Msg",	//消息队列名称
                (OS_MSG_QTY	)KEYMSG_Q_NUM,	//消息队列长度，这里设置为1
                (OS_ERR*	)&err);		//错误码
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
	//创建按键任务
	OSTaskCreate((OS_TCB 	* )&Keyprocess_TaskTCB,		
				 (CPU_CHAR	* )"Keyprocess task", 		
                 (OS_TASK_PTR )Keyprocess_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )KEYPROCESS_TASK_PRIO,     
                 (CPU_STK   * )&KEYPROCESS_TASK_STK[0],	
                 (CPU_STK_SIZE)KEYPROCESS_STK_SIZE/10,	
                 (CPU_STK_SIZE)KEYPROCESS_STK_SIZE,		
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
//	double temp;
//	char buf[20]={0};
	u8 *key,menu_y=0;                                            //判断按键状态
	OS_MSG_SIZE size;
	OS_ERR err;
	p_arg = p_arg;
	Show_Logo();                                                //显示LOGO
	Show_LMT70_Init();                                          //LMT70初始化
	Show_MPU6050_Init();                                        //MPU6050初始化
	Show_HC05_Init();                                           //蓝牙初始化
	OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);        //延时1s
	OLED_Clear();                                               //清屏
	Show_Menu();                                                //显示菜单
	while(1) {
		//请求消息KEY_Msg
		key=OSQPend((OS_Q*			)&KEY_Msg,   
					(OS_TICK		)0,
                    (OS_OPT			)OS_OPT_PEND_BLOCKING,
                    (OS_MSG_SIZE*	)&size,		
                    (CPU_TS*		)0,
                    (OS_ERR*		)&err);
		switch(*key) {
			case KEY1_PERS:
				OLED_Clear();
				Show_Menu();
				menu_y=menu_y+20;
				if(menu_y==60) menu_y=0;
				break;
			case KEY2_PERS:
				Change_Mode(menu_y);
				Applicaton();
				Software_Init(); 
				OLED_Clear();
				Show_Menu();
				break;
		}
		OLED_ShowString(16,menu_y,"=>",16);
		OLED_Refresh();
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);    //延时5ms 
	}
}

void Keyprocess_task(void *p_arg) {	
	u8 key;
	OS_ERR err;
	while(1){
		//发送消息
		key=Scan_Key();
		OSQPost((OS_Q*		)&KEY_Msg,		
				(void*		)&key,
				(OS_MSG_SIZE)1,
				(OS_OPT		)OS_OPT_POST_FIFO,
				(OS_ERR*	)&err);
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);   //延时5ms
	}
}

