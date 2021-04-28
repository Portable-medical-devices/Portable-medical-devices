//#include "sys.h"         //系统配置
//#include "delay.h"       //延时
//#include "usart.h"       //串口  		
//#include "led.h"       
//#include "ADS1292.h"
//#include "math.h"
//#include "arm_math.h"
//#include "stm32f10x_exti.h"
//#include "fdacoefs.h"
//#include "dma.h"
//#include "oled.h"

///***********************************************************************

//***作者：doee
//***主页：https://blog.csdn.net/weixin_44578655
//***关联：https://blog.csdn.net/weixin_44578655/article/details/109020022

//************************************************************************/



//s32	p_Temp[2];	//数据缓存

//s32 get_volt(u32 num);//把采到的3个字节补码转成有符号32位数

//__IO uint16_t ADCConvertedValue;	//ADC DMA  buffer


//extern __IO uint32_t sys_tick;	//系统时基：每1ms自增1
//uint32_t led_tick = 0;	//用来控制LED闪烁周期
//uint32_t key_tick = 0;	//用来控制按键扫描周期
//uint32_t imu_tick = 0;	//用来控制姿态传感器数据处理周期
//uint32_t ecg_tick = 0;	//用来计算ECG信号的采样周期	
//uint32_t server_data_tx_tick = 0;	//用来控制数据上传周期
//uint32_t lmt70_tick = 0;	//LMT70采样周期
//uint8_t key_res = 0;	//按键扫描结果

//int32_t ecg_sum_temp = 0;	//ECG数据累加的临时变量
//uint32_t ecg_sum_count = 0;	//ECG数据累加计数
//int32_t ecg_avr = 0;		//n次ECG采样平均值
//int32_t ecg_fir_res;		//fir滤波后的心电值
//uint32_t ecg_x_value = 0;	//ecg示波器横轴变量
//uint8_t ecg_x_ratio = 5 ;	//每次ECG数据都先经过ecg_x_ratio次求和求平均（相当于均值滤波），调整该值相当于调整采样频率
//uint32_t ecg_count = 0;		//每次采集ECG数据时，该值计数，用来实测ECG的采样周期
//uint8_t heart_rate = 0;		//心率



//uint8_t filter_enable = 1;	//使能FIR滤波器


//uint8_t menu_flag = 0;	//菜单标志

//s32 get_volt(u32 num);
////main
//int main(void)
//{		
//	u16 sss=0;
//		u16 k;
//	u32 times=0;
//		u32 cannle[2];	//存储两个通道的数据
//	  u8 data_to_send[60];//串口发送缓存
//	  u8 sum,i;
//		data_to_send[0]=0xAA;
//		data_to_send[1]=0xAA;
//		data_to_send[2]=0xF1;	
//		data_to_send[3]=8;
//		//初始化系统时钟	 72M
//		SystemInit();	
//		delay_init();
//		OLED_Init();
//		uart_init(115200);//串口初始化为115200	
//	  DMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)data_to_send);//串口1DMA设置
//		USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //DMA	
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
//		LED_Init();			//LED初始化
//		ecg_fir_tilter_init();				//fir滤波器初始化
//		
//	  PBout(10)=1;
//	  PBout(11)=0;
//		
//	
//	  ADS1292_Init();	//初始化ads1292
//		
//		while(Set_ADS1292_Collect(0))//0 正常采集  //1 1mV1Hz内部侧试信号 //2 内部短接噪声测试
//		{	
//				printf("1292寄存器设置失败\r\n");
//				delay_s(1);
//		}
//		
//		delay_s(1);
//		
//		//TIM2_Init(10,7200);	//系统时基
//		
//		EXTI->IMR |= EXTI_Line8;	//开DRDY中断（这句在买来ADS1292里就有的，没动）

//		while(1)//循环发送数据
//		{
//			if(ads1292_recive_flag)		//ADS1292采样到数据
//			{
//				cannle[0]=ads1292_Cache[3]<<16 | ads1292_Cache[4]<<8 | ads1292_Cache[5];//获取原始数据		
//				cannle[1]=ads1292_Cache[6]<<16 | ads1292_Cache[7]<<8 | ads1292_Cache[8];
//			
//				p_Temp[0] = get_volt(cannle[0]);	//把采到的3个字节转成有符号32位数
//				p_Temp[1] = get_volt(cannle[1]);	//把采到的3个字节转成有符号32位数
//				//这里是显示原始数据  将原始的信号发到上位机
//				//有符号数为再转为无符号，无符号数为逻辑右移
////							data_to_send[4]=p_Temp[0] >>24;		//25-32位
////							data_to_send[5]=p_Temp[0] >>16;  	//17-24
////							data_to_send[6]=p_Temp[0] >>8;		//9-16
////							data_to_send[7]=p_Temp[0];  			//1-8

////							data_to_send[8]=p_Temp[1]>>24;		//25-32位
////							data_to_send[9]=p_Temp[1]>>16;  	//17-24
////							data_to_send[10]=p_Temp[1]>>8;		//9-16
////							data_to_send[11]=p_Temp[1];			 //1-8
////							
////							for(i=0;i<12;i++)
////									sum += data_to_send[i];							
////							data_to_send[12] = sum;	//校验和																		
////							DMA_Enable(DMA1_Channel4,13);//串口1DMA 
////							sum=0;
//		
//				ecg_sum_temp += p_Temp[1];			//累加
//				ecg_sum_count++;					//计数
//				if(ecg_sum_count >= ecg_x_ratio)	//连续采样ecg_x_ratio次
//				{

//					ecg_avr = ecg_sum_temp/ecg_x_ratio;					//计算平均值
//											
//					if(filter_enable)
//					{
//						//使能FIR带通滤波
//						ecg_fir_res = (int32_t)ecg_fir_tilter(p_Temp[1]);		//FIR带通滤波
////						printf("%d\t",ecg_fir_res);
////							data_to_send[4]=p_Temp[1]>>24;		//25-32位
////							data_to_send[5]=p_Temp[1]>>16;  	//17-24
////							data_to_send[6]=p_Temp[1]>>8;		//9-16
////							data_to_send[7]=p_Temp[1]; 			//1-8
//						k=1500;
////						data_to_send[4]=(ecg_fir_res>>24)/k;		//25-32位
////					  data_to_send[5]=(ecg_fir_res>>16)/k;  	//17-24
////					  data_to_send[6]=(ecg_fir_res>>8)/k;		//9-16
//					  data_to_send[4]=(ecg_fir_res)/k;			 //1-8
//						for(i=0;i<5;i++)									
//						    sum += data_to_send[i];	
//					  data_to_send[5] = sum;	//校验和	
//							
//						if(times>175) {
//							DMA_Enable(DMA1_Channel4,6);//串口1DMA 
//							OLED_ShowFunction((data_to_send[4]-50)*2.5);
//						}
//						times++;
//						sum=0;
//					}
//					else
//					{
//						//失能FIR带通滤波
//						//osc_refresh(&ecg_win,ecg_x_value++,ecg_avr);	//刷新示波器串口（以屏幕像素点为单位）
//					}
//					ecg_sum_count = 0;	//计数清零
//					ecg_sum_temp = 0;	//求和清零
//				}
//				/***测量采样周期***/
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

///*功能：把采到的3个字节转成有符号32位数 */
//s32 get_volt(u32 num){		
//			s32 temp;
//			temp = num;
//			temp <<= 8;
//			temp >>= 8;
//			return temp;
//}

///**********************************************************************
//编译结果里面的几个数据的意义：
//Code：表示程序所占用 FLASH 的大小（FLASH）
//RO-data：即 Read Only-data， 表示程序定义的常量，如 const 类型（FLASH）
//RW-data：即 Read Write-data， 表示已被初始化的全局变量（SRAM）
//ZI-data：即 Zero Init-data， 表示未被初始化的全局变量(SRAM)
//***********************************************************************/



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
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];

//OLED任务
//任务控制块
OS_TCB OledTaskTCB;
//任务堆栈	
CPU_STK OLED_TASK_STK[OLED_STK_SIZE];

//按键任务
//任务控制块
OS_TCB Keyprocess_TaskTCB;
//任务堆栈	
CPU_STK KEYPROCESS_TASK_STK[KEYPROCESS_STK_SIZE];

////////////////////////消息队列//////////////////////////////
OS_Q KEY_Msg;				//定义一个消息队列，用于按键消息传递，模拟消息邮箱

int main() {
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组
	Software_Init();    //软件初始化
	Dirver_Init();      //硬件初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
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
	Show_ADS1292_Init();
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


