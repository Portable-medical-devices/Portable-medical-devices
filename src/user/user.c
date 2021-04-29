/****************************************************************************************
@file     user.c
@brief    �û���������
@author   ʯ��ǿ
@version  V1.0
@introduction  
*****************************************************************************************/
#include "user.h"

User user;

s32	p_Temp[2];	//���ݻ���

s32 get_volt(u32 num);//�Ѳɵ���3���ֽڲ���ת���з���32λ��

__IO uint16_t ADCConvertedValue;	//ADC DMA  buffer


		u16 k;
		u32 cannle[2];	//�洢����ͨ��������
	  u8 data_to_send[60];//���ڷ��ͻ���
	  u8 sum,i;

extern __IO uint32_t sys_tick;	//ϵͳʱ����ÿ1ms����1
uint32_t led_tick = 0;	//��������LED��˸����
uint32_t key_tick = 0;	//�������ư���ɨ������
uint32_t imu_tick = 0;	//����������̬���������ݴ�������
uint32_t ecg_tick = 0;	//��������ECG�źŵĲ�������	
uint32_t server_data_tx_tick = 0;	//�������������ϴ�����
uint32_t lmt70_tick = 0;	//LMT70��������
uint8_t key_res = 0;	//����ɨ����

int32_t ecg_sum_temp = 0;	//ECG�����ۼӵ���ʱ����
uint32_t ecg_sum_count = 0;	//ECG�����ۼӼ���
int32_t ecg_avr = 0;		//n��ECG����ƽ��ֵ
int32_t ecg_fir_res;		//fir�˲�����ĵ�ֵ
int32_t ecg_fir_res_pre=0;		//fir�˲�����ĵ�ֵ
u8 ecg_flag=0;              //�Ƿ��������ֵ
uint32_t ecg_x_value = 0;	//ecgʾ�����������
uint8_t ecg_x_ratio = 5 ;	//ÿ��ECG���ݶ��Ⱦ���ecg_x_ratio�������ƽ�����൱�ھ�ֵ�˲�����������ֵ�൱�ڵ�������Ƶ��
uint32_t ecg_count = 0;		//ÿ�βɼ�ECG����ʱ����ֵ����������ʵ��ECG�Ĳ�������
uint8_t heart_rate = 0;		//����
u8 data_oled_ok=0;
u8 data_send_ok=0;

int ecg_max=0x80000000;             //��ȡ��ֵ
int ecg_min=0x7FFFFFFF;             //��ȡ��ֵ
int ecg_max_next=0x80000000;             //��ȡ��ֵ
int ecg_min_next=0x7FFFFFFF;             //��ȡ��ֵ
u8 ecg_thresh=0;          //��ֵ

uint8_t filter_enable = 1;	//ʹ��FIR�˲���


uint8_t menu_flag = 0;	//�˵���־

u8 timer_times=0;

void Dirver_Init(void) {         //Ӳ����ʼ��
	KEY_Init();                  //���������
	LED_Init();                  //LED��ʼ��
	BEEP_Init();                 //��������ʼ��
	OLED_Init();                 //OLED��ʼ��
	Adc_Init();		             //ADC��ʼ��	
	uart_init(115200);           //���ڳ�ʼ��
	MPU6050_Init();              //MPU6050��ʼ��
	HC05_Init(115200);           //����ģ���ʼ��
	TIM3_Int_Init(9999,7200);    //1s
	//ecg_fir_tilter_init();       //fir�˲�����ʼ��
}

void Show_Logo(void) {           //��ʾLogo
	OS_ERR err;
	OLED_Clear();                                               //OLED����
	OLED_ShowPicture(0,0,128,8,LOGO);                           //��ʾLOGO
	OLED_Refresh();
	OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);        //��ʱ1s
	OLED_Clear();                                               //OLED����
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
	OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);    //��ʱ5m
//	while(1) {
//		//HC05_Send_Data((u8 *)buf,3);
//		if(USART2_RX_STA&0x8000) {
//			len=USART2_RX_STA&0x3fff;     //��ȡ����
//			USART2_RX_BUF[len]=0;
//			USART2_RX_STA=0;
//			if(!strcmp(buf,(char *)USART2_RX_BUF)) break;
//			//////////////////////////////////////////
//		}
//		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);    //��ʱ5ms
//	}
	HC05_Send_Data((u8 *)buf,3);
	OLED_ShowString(0,32," HC-05   [OK]  ",16);
	OLED_Refresh();
}

void Show_ADS1292_Init(void) {      //��ʾ��ʼ��ADS1292
	OS_ERR err;
	OLED_ShowString(0,48," ADS1292 [init]",16);
	OLED_Refresh(); 
	ecg_fir_tilter_init();				//fir�˲�����ʼ��
	PBout(10)=1;
	PBout(11)=0;
	ADS1292_Init();	//��ʼ��ads1292
	while(Set_ADS1292_Collect(0))//0 �����ɼ�  //1 1mV1Hz�ڲ������ź� //2 �ڲ��̽���������
	{	
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);    //��ʱ5ms
	}
	OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);    //��ʱ5ms			
	//TIM2_Init(10,7200);	//ϵͳʱ��		
	OLED_ShowString(0,48," ADS1292 [OK]  ",16);
	OLED_Refresh();
	OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
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
	OS_ERR err;
	u8 len;
	int times=0;
	u8 flag=1;
	char buf[50]={0};
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
			OLED_ShowPicture(0,4,36,8,HEART);
			OLED_ShowString(0,0,"bpm:...",24);
			OLED_Refresh();
			while(1) {
				if(KEY2==0) {
					TIM_Cmd(TIM3, DISABLE);    //ֹͣ��ʱ
					return;
				}
				Show_Ecg();
			}
		case DEFAULT:
			break;
	}
}

void Show_Ecg(void) {               //��ʾ�ĵ�����
	u8 len;
	static u8 flag=1;
	if((len=Get_Ecg())) { //��ȡ���ĵ�����
//		OSTaskResume((OS_TCB*)&EcgTaskTCB,&err);
		if(ecg_fir_res<=ecg_fir_res_pre) flag=1;
		if(flag&&ecg_fir_res>48&&ecg_fir_res>ecg_fir_res_pre) {
			TIM_Cmd(TIM3, ENABLE); 
			user.ecg_times++;
			flag=0;
		}
		HC05_Send_Data(data_to_send,len);
		//OSTimeDlyHMSM(0,0,0,3,OS_OPT_TIME_HMSM_STRICT,&err);    //��ʱ5ms 
	}
}

u8   Get_Ecg(void) {                //��ȡ�ĵ粨��
	u8 i;
	u8 sum=0;
	u8 ret=0;
	static u8 t=0;
	static u32 ecg_times=500;
	static u32 times=200;
	static double volatile b=80;
	static double volatile a=20;
	static double volatile k=0;
	data_to_send[0]=0xA5;	//25-32λ
	EXTI->IMR |= EXTI_Line8;	//��DRDY�жϣ����������ADS1292����еģ�û����
	if(ads1292_recive_flag)	{	//ADS1292����������
		cannle[0]=ads1292_Cache[3]<<16 | ads1292_Cache[4]<<8 | ads1292_Cache[5];//��ȡԭʼ����		
		cannle[1]=ads1292_Cache[6]<<16 | ads1292_Cache[7]<<8 | ads1292_Cache[8];
		
		p_Temp[0] = get_volt(cannle[0]);	//�Ѳɵ���3���ֽ�ת���з���32λ��
		p_Temp[1] = get_volt(cannle[1]);	//�Ѳɵ���3���ֽ�ת���з���32λ��
		
		ecg_sum_temp += p_Temp[1];			//�ۼ�
		ecg_sum_count++;					//����
		if(ecg_sum_count >= ecg_x_ratio) {	//��������ecg_x_ratio��
			ecg_avr = ecg_sum_temp/ecg_x_ratio;					//����ƽ��ֵ
			if(filter_enable) {
				//ʹ��FIR��ͨ�˲�
				ecg_fir_res = (int32_t)ecg_fir_tilter(p_Temp[1]);		//FIR��ͨ�˲�
				if(!times) {				
					if(ecg_times&&ecg_fir_res>ecg_max) ecg_max=ecg_fir_res;
					if(ecg_times&&ecg_fir_res<ecg_min) ecg_min=ecg_fir_res;
					//OLED_ShowTimesFunction(2,ecg_fir_res);
					if(!ecg_times) {
						k=(b-a)/(ecg_max-ecg_min);
						ecg_fir_res_pre=t;                       //������һ����ֵ
						ecg_fir_res=a+k*(ecg_fir_res-ecg_min);             //���ݹ�һ����[a:b]����
						t=ecg_fir_res;
	//					if(ecg_fir_res<20||ecg_fir_res>100) ecg_fir_res=0;
						ecg_thresh=b*0.6;                 //��ȡ��ֵ
						data_to_send[1]=ecg_fir_res;
						data_to_send[2]=ecg_thresh;
						sum=0;
						for(i=1;i<3;++i) sum+=data_to_send[i];
						data_to_send[3]=sum;
						data_to_send[4]=0x5A;				
						ret=5;
					}
					if(ecg_times) ecg_times--;
				}
				if(times) times--;
			}
			ecg_sum_count = 0;	//��������
			ecg_sum_temp = 0;	//�������
		}
		ads1292_recive_flag = 0;
	}
	return ret;
}

/*���ܣ��Ѳɵ���3���ֽ�ת���з���32λ�� */
s32 get_volt(u32 num){		
			s32 temp;
			temp = num;
			temp <<= 8;
			temp >>= 8;
			return temp;
}


void Software_Init(void) {          //�����ʼ��
	user.mode=DEFAULT;
	user.temputre=0;
	user.walk.distance=0;
	user.walk.step=0;
	user.ecg_times=0;
	timer_times=0;
}

void Show_Temputre(void) {          //��ʾ�¶�
	char buf[30]={0};
	OLED_ShowPicture(5,2,17,5,TEMPUTRE);
	user.temputre=(float)Get_Adc_Middle(ADC_Channel_1,500)*(3.3/4096);  //ADC��λ���˲�ֵ��ȡ
	user.temputre=212.009-193*user.temputre;
	sprintf(buf,"%.2f C   ",(float)user.temputre);                         
	printf("%.2f\r\n",user.temputre);
	OLED_ShowString(25,18,buf,24);
	OLED_Refresh();
}

void Show_Step(void) {              //��ʾ�ƶ���Ϣ
	char buf[2][100]={0};
	OLED_ShowPicture(0,1,29,7,WALK);
	MPU_Step_Count();
	sprintf(buf[0],"%d step",(int)user.walk.step);
	OLED_ShowString(40,6,buf[0],24);
	sprintf(buf[1],"%.0f m",user.walk.distance);
	OLED_ShowString(40,30,buf[1],24);
	OLED_Refresh();
}


