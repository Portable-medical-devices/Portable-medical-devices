/****************************************************************************************
@file     user.c
@brief    用户操作函数
@author   石国强
@version  V1.0
@introduction  
*****************************************************************************************/
#include "user.h"

User user;

s32	p_Temp[2];	//数据缓存

s32 get_volt(u32 num);//把采到的3个字节补码转成有符号32位数

__IO uint16_t ADCConvertedValue;	//ADC DMA  buffer


		u16 k;
		u32 cannle[2];	//存储两个通道的数据
	  u8 data_to_send[60];//串口发送缓存
	  u8 sum,i;

extern __IO uint32_t sys_tick;	//系统时基：每1ms自增1
uint32_t led_tick = 0;	//用来控制LED闪烁周期
uint32_t key_tick = 0;	//用来控制按键扫描周期
uint32_t imu_tick = 0;	//用来控制姿态传感器数据处理周期
uint32_t ecg_tick = 0;	//用来计算ECG信号的采样周期	
uint32_t server_data_tx_tick = 0;	//用来控制数据上传周期
uint32_t lmt70_tick = 0;	//LMT70采样周期
uint8_t key_res = 0;	//按键扫描结果

int32_t ecg_sum_temp = 0;	//ECG数据累加的临时变量
uint32_t ecg_sum_count = 0;	//ECG数据累加计数
int32_t ecg_avr = 0;		//n次ECG采样平均值
int32_t ecg_fir_res;		//fir滤波后的心电值
uint32_t ecg_x_value = 0;	//ecg示波器横轴变量
uint8_t ecg_x_ratio = 5 ;	//每次ECG数据都先经过ecg_x_ratio次求和求平均（相当于均值滤波），调整该值相当于调整采样频率
uint32_t ecg_count = 0;		//每次采集ECG数据时，该值计数，用来实测ECG的采样周期
uint8_t heart_rate = 0;		//心率



uint8_t filter_enable = 1;	//使能FIR滤波器


uint8_t menu_flag = 0;	//菜单标志




void Dirver_Init(void) {         //硬件初始化
	KEY_Init();                  //按键输出化
	LED_Init();                  //LED初始化
	BEEP_Init();                 //蜂鸣器初始化
	OLED_Init();                 //OLED初始化
	Adc_Init();		             //ADC初始化	
	uart_init(115200);           //串口初始化
	MPU6050_Init();              //MPU6050初始化
	HC05_Init(115200);           //蓝牙模块初始化
	//ecg_fir_tilter_init();       //fir滤波器初始化
}

void Show_Logo(void) {           //显示Logo
	OS_ERR err;
	OLED_Clear();                                               //OLED清屏
	OLED_ShowPicture(0,0,128,8,LOGO);                           //显示LOGO
	OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);        //延时1s
}

void Show_LMT70_Init(void) {                       //显示初始化LMT70
	OS_ERR err;
	u16 Errtimes=0;
	volatile float temp;
	OLED_ShowString(0,0," LMT70 [init]",16);       //显示LMT70初始化
	OLED_Refresh();                                //OLED刷屏
	while(temp<0||temp>50) {
		Errtimes++;
		temp=(float)Get_Adc_Middle(ADC_Channel_1,500)*(3.3/4096);   //ADC中位数滤波值获取
		temp=212.009-193*temp;
		if(Errtimes==5000) {
				OLED_ShowString(0,0," LMT70 [ERROR]",16);
				OLED_Refresh(); 
		}
	}
	OLED_ShowString(0,0," LMT70   [OK]",16);
	OLED_Refresh(); 
	OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);        //延时200ms
}

void Show_MPU6050_Init(void) {      //显示初始化MPU6050
	OS_ERR err;
	OLED_ShowString(0,16," MPU6050 [init]",16);
	OLED_Refresh(); 
	while(mpu_dmp_init());                                     //DMP初始化
	OLED_ShowString(0,16," MPU6050 [OK]  ",16);
	OLED_Refresh();
	OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
}

void Show_HC05_Init(void) {         //显示初始化HC05
	OS_ERR err;
	char buf[]="OK";
	int len=0;
	OLED_ShowString(0,32," HC-05   [init]",16);
	OLED_Refresh();
	//OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);    //延时5m
	while(1) {
		//HC05_Send_Data((u8 *)buf,3);
		if(USART2_RX_STA&0x8000) {
			len=USART2_RX_STA&0x3fff;     //获取长度
			USART2_RX_BUF[len]=0;
			USART2_RX_STA=0;
			if(!strcmp(buf,(char *)USART2_RX_BUF)) break;
			//////////////////////////////////////////
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);    //延时5ms
	}
	HC05_Send_Data((u8 *)buf,3);
	OLED_ShowString(0,32," HC-05   [OK]  ",16);
	OLED_Refresh();
}

void Show_ADS1292_Init(void) {      //显示初始化ADS1292
	OS_ERR err;
	OLED_ShowString(0,48," ADS1292 [init]",16);
	OLED_Refresh(); 
	ecg_fir_tilter_init();				//fir滤波器初始化
	PBout(10)=1;
	PBout(11)=0;
	ADS1292_Init();	//初始化ads1292
	while(Set_ADS1292_Collect(0))//0 正常采集  //1 1mV1Hz内部侧试信号 //2 内部短接噪声测试
	{	
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);    //延时5ms
	}
	OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);    //延时5ms			
	//TIM2_Init(10,7200);	//系统时基		
	OLED_ShowString(0,48," ADS1292 [OK]  ",16);
	OLED_Refresh();
	OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
}

u8   Scan_Key(void) {               //扫描按键
	static u8 flag=1;               //默认没有按键按下
	u8 res=0x03;                    //默认两个按键拉高 
	u8 key1=KEY1,key2=KEY2;
	OS_ERR err;
	 
	OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
	if(key1==KEY1&&key2==KEY2) {
		if(flag&&KEY1==0) {
			res&=~(1<<1);
			flag=0;
		}
		else if(flag&&KEY2==0) {
			res&=~(1<<0);
			flag=0;
		}
		else if(!flag&&KEY1==1&&KEY2==1) {
			flag=1;
		}
	}
	return res;
}

void Show_Menu(void) {              //显示菜单
	OLED_ShowString(32,0," Temputre",16);
	OLED_ShowString(32,20," Step",16);
	OLED_ShowString(32,40," Huart",16);
	OLED_Refresh();
}

void Change_Mode(u8 y) {               //模式切换
	if(y==0) user.mode=TEMPUTRE_MODE;
	else if(y==20) user.mode=STEP_MODE;
	else if(y==40) user.mode=HART_MODE;
}

void Applicaton(void) {             //应用
	u8 i;
	u8 sum=0;
	static u32 times=0;
	double volatile b=60;
	double volatile a=16;
	double volatile k;
	double volatile Max=25000;
	double volatile Min=1500;
	u32 res;
	data_to_send[0]=0xA5;	//25-32位

	k=(b-a)/(Max-Min);
	OLED_Clear();
	while(!KEY2);
	switch(user.mode) {
		case TEMPUTRE_MODE:
			while(1) {
				if(KEY2==0) return;
				Show_Temputre();
			}
			break;
		case STEP_MODE:
			while(1) {
				if(KEY2==0) return;
				Show_Step();
			}
			break;
		case HART_MODE:
			EXTI->IMR |= EXTI_Line8;	//开DRDY中断（这句在买来ADS1292里就有的，没动）
			while(1) {
				if(ads1292_recive_flag)	{	//ADS1292采样到数据
					cannle[0]=ads1292_Cache[3]<<16 | ads1292_Cache[4]<<8 | ads1292_Cache[5];//获取原始数据		
					cannle[1]=ads1292_Cache[6]<<16 | ads1292_Cache[7]<<8 | ads1292_Cache[8];
				
					p_Temp[0] = get_volt(cannle[0]);	//把采到的3个字节转成有符号32位数
					p_Temp[1] = get_volt(cannle[1]);	//把采到的3个字节转成有符号32位数
			
					ecg_sum_temp += p_Temp[1];			//累加
					ecg_sum_count++;					//计数
					if(ecg_sum_count >= ecg_x_ratio) {	//连续采样ecg_x_ratio次
						ecg_avr = ecg_sum_temp/ecg_x_ratio;					//计算平均值
						if(filter_enable) {
							//使能FIR带通滤波
							ecg_fir_res = (int32_t)ecg_fir_tilter(p_Temp[1]);		//FIR带通滤波
							ecg_fir_res+=7000;
							ecg_fir_res=a+k*(ecg_fir_res-Min);            //数据归一化
							data_to_send[1]=ecg_fir_res;
							data_to_send[2]=ecg_fir_res;
//							data_to_send[1]=(u8)ecg_fir_res&0x000000FF;
//							data_to_send[2]=(u8)(ecg_fir_res>>8)&0x000000FF;
//							data_to_send[3]=(u8)(ecg_fir_res>>16)&0x000000FF;
//							data_to_send[4]=(u8)(ecg_fir_res>>24)&0x000000FF;
//							sum=0;
							//for(i=1;i<5;++i) sum=(u8)(sum+data_to_send[i]);
//							data_to_send[5]=sum;
							data_to_send[3]=0x5A;
							 
							

							
							if(times>175) {
								//OLED_ShowFunction(res);
								HC05_Send_Data(data_to_send,4);
								OLED_ShowTimesFunction(2,ecg_fir_res);
							}
							times++;
						}
						ecg_sum_count = 0;	//计数清零
						ecg_sum_temp = 0;	//求和清零
					}
					ads1292_recive_flag = 0;
				}
				if(KEY2==0) return;
			}
		case DEFAULT:
			break;
	}
}

/*功能：把采到的3个字节转成有符号32位数 */
s32 get_volt(u32 num){		
			s32 temp;
			temp = num;
			temp <<= 8;
			temp >>= 8;
			return temp;
}


void Software_Init(void) {          //软件初始化
	user.mode=DEFAULT;
	user.temputre=0;
	user.walk.distance=0;
	user.walk.step=0;
}

void Show_Temputre(void) {          //显示温度
	char buf[30]={0};
	user.temputre=(float)Get_Adc_Middle(ADC_Channel_1,500)*(3.3/4096);  //ADC中位数滤波值获取
	user.temputre=212.009-193*user.temputre;
	sprintf(buf,"%.2f C   ",(float)user.temputre);                         
	printf("%.2f\r\n",user.temputre);
	OLED_ShowString(25,18,buf,24);
	OLED_Refresh();
}

void Show_Step(void) {              //显示移动信息
	char buf[2][100]={0};
	MPU_Step_Count();
	sprintf(buf[0],"%d step",(int)user.walk.step);
	OLED_ShowString(30,16,buf[0],16);
	sprintf(buf[1],"%.0f m",user.walk.distance);
	OLED_ShowString(30,32,buf[1],16);
	OLED_Refresh();
}


