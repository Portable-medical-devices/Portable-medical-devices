/****************************************************************************************
@file     main.c
@brief    �����������ļ�
@author   ʯ��ǿ
@version  V1.0
@introduction �����ǡ��Ʋ�����������
*****************************************************************************************/

//UCOSIII���������ȼ��û�������ʹ�ã�ALIENTEK
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()

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

//start����
//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//led����
//�������ȼ�
#define LED_TASK_PRIO		7
//�����ջ��С	
#define LED_STK_SIZE 		128
//������ƿ�
OS_TCB LedTaskTCB;
//�����ջ	
CPU_STK LED_TASK_STK[LED_STK_SIZE];
void led_task(void *p_arg);

//beep����
//�������ȼ�
#define BEEP_TASK_PRIO		5
//�����ջ��С	
#define BEEP_STK_SIZE 		128
//������ƿ�
OS_TCB BeepTaskTCB;
//�����ջ	
CPU_STK BEEP_TASK_STK[BEEP_STK_SIZE];
void beep_task(void *p_arg);

//OLED����
//�������ȼ�
#define OLED_TASK_PRIO		6
//�����ջ��С	
#define OLED_STK_SIZE 		500
//������ƿ�
OS_TCB OledTaskTCB;
//�����ջ	
CPU_STK OLED_TASK_STK[OLED_STK_SIZE];
void oled_task(void *p_arg);

int main() {
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�жϷ���
	
	LED_Init();         //Ӳ����ʼ��
	BEEP_Init();        //��������ʼ��
	OLED_Init();        //OLED��ʼ��
	//OLED_ShowNum(0,0,-123.22,16);
	//OLED_Refresh();
	MPU6050_Init();     //MPU6050��ʼ��
	OSInit(&err);	    //��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	  
	OSStart(&err);  //����UCOSIII
	while(1);
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	//����LED����
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
				 
	//����BEEP
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
	//����OLED
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
			 							 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
}

//led������
void led_task(void *p_arg) {
	OS_ERR err;
	p_arg = p_arg;
	while(1) {
		LED(0);
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
		LED(1);
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
	}
}

	char buf[20] ={0};

//oled������
void oled_task(void *p_arg) {
	u8 i,size;
	char buf[3][20]={0};
//	float pitch,roll,yaw;  
	OS_ERR err;
	p_arg = p_arg;
	OLED_Clear();                                               //OLED����
	OLED_ShowPicture(0,0,128,8,LOGO);                           //��ʾLOGO
	OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);      //��ʱ500ms
	OLED_ShowString(0,0," MPU6050 [init]",16);
	OLED_Refresh();                                             //OLEDˢ��
	while(mpu_dmp_init()) {                                     //DMP��ʼ��
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);    //��ʱ5ms
	}
	OLED_ShowString(0,0," MPU6050 [OK]  ",16);
	OLED_Refresh();                                             //OLEDˢ��
	OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);      //��ʱ500ms
	OLED_Clear();                                               //����
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
		sprintf(buf[0],"Sports:%d step",(int)walk.step);         //��ʾ����
		sprintf(buf[1],"Distance:%.0f m",walk.distance);         //��ʾ����
		size=16;
		for(i=0;i<2;++i) {
			OLED_ShowString(0,0+(size/8+(size%8?1:0))*8*i,buf[i],size);
		}		
		OLED_Refresh(); 
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);     //��ʱ5ms
	}
}

//beep������
void beep_task(void *p_arg) {
	int i;
	OS_ERR err;
	p_arg = p_arg;
	while(1) {
		for(i=0;i<10;++i) {
			BEEP(1);
			OSTimeDlyHMSM(0,0,0,25,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ50ms
			BEEP(0);
			OSTimeDlyHMSM(0,0,0,25,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ50ms
		}
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);    //��ʱ500ms
	}
}



