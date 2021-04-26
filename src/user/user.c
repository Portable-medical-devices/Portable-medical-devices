/****************************************************************************************
@file     user.c
@brief    用户操作函数
@author   石国强
@version  V1.0
@introduction  
*****************************************************************************************/
#include "user.h"

User user;

void Dirver_Init(void) {         //硬件初始化
	KEY_Init();                  //按键输出化
	LED_Init();                  //LED初始化
	BEEP_Init();                 //蜂鸣器初始化
	OLED_Init();                 //OLED初始化
	Adc_Init();		             //ADC初始化	
	uart_init(115200);           //串口初始化
	MPU6050_Init();              //MPU6050初始化
	HC05_Init(115200);           //蓝牙模块初始化
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
	OLED_ShowString(32,40," Heart",16);
	OLED_Refresh();
}

void Change_Mode(u8 y) {               //模式切换
	if(y==0) user.mode=TEMPUTRE_MODE;
	else if(y==20) user.mode=STEP_MODE;
	else if(y==40) user.mode=HART_MODE;
}

void Applicaton(void) {             //应用
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
			OLED_ShowString(0,0,"HART",24);
			OLED_Refresh();
			while(1) {
				if(KEY2==0) return;
			}
			break;
		case DEFAULT:
			break;
	}
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
	OLED_ShowString(0,0,buf[0],16);
	sprintf(buf[1],"%.0f m",user.walk.distance);
	OLED_ShowString(0,16,buf[1],16);
	OLED_Refresh();
}
