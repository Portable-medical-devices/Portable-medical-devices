/****************************************************************************************
@file     user.c
@brief    �û���������
@author   ʯ��ǿ
@version  V1.0
@introduction  
*****************************************************************************************/
#include "user.h"

User user;

void Dirver_Init(void) {         //Ӳ����ʼ��
	KEY_Init();                  //���������
	LED_Init();                  //LED��ʼ��
	BEEP_Init();                 //��������ʼ��
	OLED_Init();                 //OLED��ʼ��
	Adc_Init();		             //ADC��ʼ��	
	uart_init(115200);           //���ڳ�ʼ��
	MPU6050_Init();              //MPU6050��ʼ��
	HC05_Init(115200);           //����ģ���ʼ��
}

void Show_Logo(void) {           //��ʾLogo
	OS_ERR err;
	OLED_Clear();                                               //OLED����
	OLED_ShowPicture(0,0,128,8,LOGO);                           //��ʾLOGO
	OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);        //��ʱ1s
}

void Show_LMT70_Init(void) {                       //��ʾ��ʼ��LMT70
	OS_ERR err;
	u16 Errtimes=0;
	volatile float temp;
	OLED_ShowString(0,0," LMT70 [init]",16);       //��ʾLMT70��ʼ��
	OLED_Refresh();                                //OLEDˢ��
	while(temp<0||temp>50) {
		Errtimes++;
		temp=(float)Get_Adc_Middle(ADC_Channel_1,500)*(3.3/4096);   //ADC��λ���˲�ֵ��ȡ
		temp=212.009-193*temp;
		if(Errtimes==5000) {
				OLED_ShowString(0,0," LMT70 [ERROR]",16);
				OLED_Refresh(); 
		}
	}
	OLED_ShowString(0,0," LMT70   [OK]",16);
	OLED_Refresh(); 
	OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);        //��ʱ200ms
}

void Show_MPU6050_Init(void) {      //��ʾ��ʼ��MPU6050
	OS_ERR err;
	OLED_ShowString(0,16," MPU6050 [init]",16);
	OLED_Refresh(); 
	while(mpu_dmp_init());                                     //DMP��ʼ��
	OLED_ShowString(0,16," MPU6050 [OK]  ",16);
	OLED_Refresh();
	OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
}

void Show_HC05_Init(void) {         //��ʾ��ʼ��HC05
	OS_ERR err;
	char buf[]="OK";
	int len=0;
	OLED_ShowString(0,32," HC-05   [init]",16);
	OLED_Refresh();
	//OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);    //��ʱ5m
	while(1) {
		//HC05_Send_Data((u8 *)buf,3);
		if(USART2_RX_STA&0x8000) {
			len=USART2_RX_STA&0x3fff;     //��ȡ����
			USART2_RX_BUF[len]=0;
			USART2_RX_STA=0;
			if(!strcmp(buf,(char *)USART2_RX_BUF)) break;
			//////////////////////////////////////////
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);    //��ʱ5ms
	}
	HC05_Send_Data((u8 *)buf,3);
	OLED_ShowString(0,32," HC-05   [OK]  ",16);
	OLED_Refresh();
}

u8   Scan_Key(void) {               //ɨ�谴��
	static u8 flag=1;               //Ĭ��û�а�������
	u8 res=0x03;                    //Ĭ�������������� 
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

void Show_Menu(void) {              //��ʾ�˵�
	OLED_ShowString(32,0," Temputre",16);
	OLED_ShowString(32,20," Step",16);
	OLED_ShowString(32,40," Heart",16);
	OLED_Refresh();
}

void Change_Mode(u8 y) {               //ģʽ�л�
	if(y==0) user.mode=TEMPUTRE_MODE;
	else if(y==20) user.mode=STEP_MODE;
	else if(y==40) user.mode=HART_MODE;
}

void Applicaton(void) {             //Ӧ��
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

void Software_Init(void) {          //�����ʼ��
	user.mode=DEFAULT;
	user.temputre=0;
	user.walk.distance=0;
	user.walk.step=0;
}

void Show_Temputre(void) {          //��ʾ�¶�
	char buf[30]={0};
	user.temputre=(float)Get_Adc_Middle(ADC_Channel_1,500)*(3.3/4096);  //ADC��λ���˲�ֵ��ȡ
	user.temputre=212.009-193*user.temputre;
	sprintf(buf,"%.2f C   ",(float)user.temputre);                         
	printf("%.2f\r\n",user.temputre);
	OLED_ShowString(25,18,buf,24);
	OLED_Refresh();
}

void Show_Step(void) {              //��ʾ�ƶ���Ϣ
	char buf[2][100]={0};
	MPU_Step_Count();
	sprintf(buf[0],"%d step",(int)user.walk.step);
	OLED_ShowString(0,0,buf[0],16);
	sprintf(buf[1],"%.0f m",user.walk.distance);
	OLED_ShowString(0,16,buf[1],16);
	OLED_Refresh();
}
