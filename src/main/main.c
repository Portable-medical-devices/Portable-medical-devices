//#include "sys.h"         //ϵͳ����
//#include "delay.h"       //��ʱ
//#include "usart.h"       //����  		
//#include "led.h"       
//#include "ADS1292.h"
//#include "math.h"
//#include "arm_math.h"
//#include "stm32f10x_exti.h"
//#include "fdacoefs.h"
//#include "dma.h"
//#include "oled.h"

///***********************************************************************

//***���ߣ�doee
//***��ҳ��https://blog.csdn.net/weixin_44578655
//***������https://blog.csdn.net/weixin_44578655/article/details/109020022

//************************************************************************/



//s32	p_Temp[2];	//���ݻ���

//s32 get_volt(u32 num);//�Ѳɵ���3���ֽڲ���ת���з���32λ��

//__IO uint16_t ADCConvertedValue;	//ADC DMA  buffer


//extern __IO uint32_t sys_tick;	//ϵͳʱ����ÿ1ms����1
//uint32_t led_tick = 0;	//��������LED��˸����
//uint32_t key_tick = 0;	//�������ư���ɨ������
//uint32_t imu_tick = 0;	//����������̬���������ݴ�������
//uint32_t ecg_tick = 0;	//��������ECG�źŵĲ�������	
//uint32_t server_data_tx_tick = 0;	//�������������ϴ�����
//uint32_t lmt70_tick = 0;	//LMT70��������
//uint8_t key_res = 0;	//����ɨ����

//int32_t ecg_sum_temp = 0;	//ECG�����ۼӵ���ʱ����
//uint32_t ecg_sum_count = 0;	//ECG�����ۼӼ���
//int32_t ecg_avr = 0;		//n��ECG����ƽ��ֵ
//int32_t ecg_fir_res;		//fir�˲�����ĵ�ֵ
//uint32_t ecg_x_value = 0;	//ecgʾ�����������
//uint8_t ecg_x_ratio = 5 ;	//ÿ��ECG���ݶ��Ⱦ���ecg_x_ratio�������ƽ�����൱�ھ�ֵ�˲�����������ֵ�൱�ڵ�������Ƶ��
//uint32_t ecg_count = 0;		//ÿ�βɼ�ECG����ʱ����ֵ����������ʵ��ECG�Ĳ�������
//uint8_t heart_rate = 0;		//����



//uint8_t filter_enable = 1;	//ʹ��FIR�˲���


//uint8_t menu_flag = 0;	//�˵���־

//s32 get_volt(u32 num);
////main
//int main(void)
//{		
//	u16 sss=0;
//		u16 k;
//	u32 times=0;
//		u32 cannle[2];	//�洢����ͨ��������
//	  u8 data_to_send[60];//���ڷ��ͻ���
//	  u8 sum,i;
//		data_to_send[0]=0xAA;
//		data_to_send[1]=0xAA;
//		data_to_send[2]=0xF1;	
//		data_to_send[3]=8;
//		//��ʼ��ϵͳʱ��	 72M
//		SystemInit();	
//		delay_init();
//		OLED_Init();
//		uart_init(115200);//���ڳ�ʼ��Ϊ115200	
//	  DMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)data_to_send);//����1DMA����
//		USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //DMA	
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//		LED_Init();			//LED��ʼ��
//		ecg_fir_tilter_init();				//fir�˲�����ʼ��
//		
//	  PBout(10)=1;
//	  PBout(11)=0;
//		
//	
//	  ADS1292_Init();	//��ʼ��ads1292
//		
//		while(Set_ADS1292_Collect(0))//0 �����ɼ�  //1 1mV1Hz�ڲ������ź� //2 �ڲ��̽���������
//		{	
//				printf("1292�Ĵ�������ʧ��\r\n");
//				delay_s(1);
//		}
//		
//		delay_s(1);
//		
//		//TIM2_Init(10,7200);	//ϵͳʱ��
//		
//		EXTI->IMR |= EXTI_Line8;	//��DRDY�жϣ����������ADS1292����еģ�û����

//		while(1)//ѭ����������
//		{
//			if(ads1292_recive_flag)		//ADS1292����������
//			{
//				cannle[0]=ads1292_Cache[3]<<16 | ads1292_Cache[4]<<8 | ads1292_Cache[5];//��ȡԭʼ����		
//				cannle[1]=ads1292_Cache[6]<<16 | ads1292_Cache[7]<<8 | ads1292_Cache[8];
//			
//				p_Temp[0] = get_volt(cannle[0]);	//�Ѳɵ���3���ֽ�ת���з���32λ��
//				p_Temp[1] = get_volt(cannle[1]);	//�Ѳɵ���3���ֽ�ת���з���32λ��
//				//��������ʾԭʼ����  ��ԭʼ���źŷ�����λ��
//				//�з�����Ϊ��תΪ�޷��ţ��޷�����Ϊ�߼�����
////							data_to_send[4]=p_Temp[0] >>24;		//25-32λ
////							data_to_send[5]=p_Temp[0] >>16;  	//17-24
////							data_to_send[6]=p_Temp[0] >>8;		//9-16
////							data_to_send[7]=p_Temp[0];  			//1-8

////							data_to_send[8]=p_Temp[1]>>24;		//25-32λ
////							data_to_send[9]=p_Temp[1]>>16;  	//17-24
////							data_to_send[10]=p_Temp[1]>>8;		//9-16
////							data_to_send[11]=p_Temp[1];			 //1-8
////							
////							for(i=0;i<12;i++)
////									sum += data_to_send[i];							
////							data_to_send[12] = sum;	//У���																		
////							DMA_Enable(DMA1_Channel4,13);//����1DMA 
////							sum=0;
//		
//				ecg_sum_temp += p_Temp[1];			//�ۼ�
//				ecg_sum_count++;					//����
//				if(ecg_sum_count >= ecg_x_ratio)	//��������ecg_x_ratio��
//				{

//					ecg_avr = ecg_sum_temp/ecg_x_ratio;					//����ƽ��ֵ
//											
//					if(filter_enable)
//					{
//						//ʹ��FIR��ͨ�˲�
//						ecg_fir_res = (int32_t)ecg_fir_tilter(p_Temp[1]);		//FIR��ͨ�˲�
////						printf("%d\t",ecg_fir_res);
////							data_to_send[4]=p_Temp[1]>>24;		//25-32λ
////							data_to_send[5]=p_Temp[1]>>16;  	//17-24
////							data_to_send[6]=p_Temp[1]>>8;		//9-16
////							data_to_send[7]=p_Temp[1]; 			//1-8
//						k=1500;
////						data_to_send[4]=(ecg_fir_res>>24)/k;		//25-32λ
////					  data_to_send[5]=(ecg_fir_res>>16)/k;  	//17-24
////					  data_to_send[6]=(ecg_fir_res>>8)/k;		//9-16
//					  data_to_send[4]=(ecg_fir_res)/k;			 //1-8
//						for(i=0;i<5;i++)									
//						    sum += data_to_send[i];	
//					  data_to_send[5] = sum;	//У���	
//							
//						if(times>175) {
//							DMA_Enable(DMA1_Channel4,6);//����1DMA 
//							OLED_ShowFunction((data_to_send[4]-50)*2.5);
//						}
//						times++;
//						sum=0;
//					}
//					else
//					{
//						//ʧ��FIR��ͨ�˲�
//						//osc_refresh(&ecg_win,ecg_x_value++,ecg_avr);	//ˢ��ʾ�������ڣ�����Ļ���ص�Ϊ��λ��
//					}
//					ecg_sum_count = 0;	//��������
//					ecg_sum_temp = 0;	//�������
//				}
//				/***������������***/
////				ecg_count++;
////				if(ecg_count >=10000)
////				{
////					ecg_count = 0;
////					LCD_ShowIntNum(0,200,sys_tick-ecg_tick,6,GREEN,BLACK,16);
////					ecg_tick = sys_tick;
////				}
//				/*****************/
//				
//				ads1292_recive_flag = 0;
//			}
//			}
//}

///*���ܣ��Ѳɵ���3���ֽ�ת���з���32λ�� */
//s32 get_volt(u32 num){		
//			s32 temp;
//			temp = num;
//			temp <<= 8;
//			temp >>= 8;
//			return temp;
//}

///**********************************************************************
//����������ļ������ݵ����壺
//Code����ʾ������ռ�� FLASH �Ĵ�С��FLASH��
//RO-data���� Read Only-data�� ��ʾ������ĳ������� const ���ͣ�FLASH��
//RW-data���� Read Write-data�� ��ʾ�ѱ���ʼ����ȫ�ֱ�����SRAM��
//ZI-data���� Zero Init-data�� ��ʾδ����ʼ����ȫ�ֱ���(SRAM)
//***********************************************************************/



/****************************************************************************************
@file     main.c
@brief    �����������ļ�
@author   ʯ��ǿ
@version  V1.0
@introduction ��������
*****************************************************************************************/

//UCOSIII���������ȼ��û�������ʹ�ã�ALIENTEK
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
#include "user.h"

__align(8)

//start����
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];

//OLED����
//������ƿ�
OS_TCB OledTaskTCB;
//�����ջ	
CPU_STK OLED_TASK_STK[OLED_STK_SIZE];

//��������
//������ƿ�
OS_TCB Keyprocess_TaskTCB;
//�����ջ	
CPU_STK KEYPROCESS_TASK_STK[KEYPROCESS_STK_SIZE];

////////////////////////��Ϣ����//////////////////////////////
OS_Q KEY_Msg;				//����һ����Ϣ���У����ڰ�����Ϣ���ݣ�ģ����Ϣ����

int main() {
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�жϷ���
	Software_Init();    //�����ʼ��
	Dirver_Init();      //Ӳ����ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
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
	//������Ϣ����KEY_Msg
	OSQCreate ((OS_Q*		)&KEY_Msg,	//��Ϣ����
                (CPU_CHAR*	)"KEY Msg",	//��Ϣ��������
                (OS_MSG_QTY	)KEYMSG_Q_NUM,	//��Ϣ���г��ȣ���������Ϊ1
                (OS_ERR*	)&err);		//������
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
	//������������
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
//	double temp;
//	char buf[20]={0};
	u8 *key,menu_y=0;                                            //�жϰ���״̬
	OS_MSG_SIZE size;
	OS_ERR err;
	p_arg = p_arg;
	Show_Logo();                                                //��ʾLOGO
	Show_LMT70_Init();                                          //LMT70��ʼ��
	Show_MPU6050_Init();                                        //MPU6050��ʼ��
	Show_HC05_Init();                                           //������ʼ��
	Show_ADS1292_Init();
	OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);        //��ʱ1s
	OLED_Clear();                                               //����
	Show_Menu();                                                //��ʾ�˵�
	while(1) {
		
		//������ϢKEY_Msg
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
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);    //��ʱ5ms 
	}
}

void Keyprocess_task(void *p_arg) {	
	u8 key;
	OS_ERR err;
	while(1){
		//������Ϣ
		key=Scan_Key();
		OSQPost((OS_Q*		)&KEY_Msg,		
				(void*		)&key,
				(OS_MSG_SIZE)1,
				(OS_OPT		)OS_OPT_POST_FIFO,
				(OS_ERR*	)&err);
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);   //��ʱ5ms
	}
}


