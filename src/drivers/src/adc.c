/****************************************************************************************
@file     adc.c
@brief    ADCԴ�ļ�
@author   ʯ��ǿ
@version  V1.0
@function ADC1 1 -> PA1
*****************************************************************************************/

#include "adc.h"
#include "mymath.h"

void Adc_Init(void) {                            //ADC��ʼ��
	ADC_InitTypeDef ADC_InitStructure;           //ADCʵ����
	GPIO_InitTypeDef GPIO_InitStructure;         //GPIOʵ����
	//ʹ��GPIOA��ADC1ϵͳʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1,ENABLE);    
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);            //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	//��ʼ��GPIO
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;      //PA1��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;  //ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);       //��ʼ��GPIOA
	//��λADC1
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	                    //ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;             	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	            //ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                            //˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	                                //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
	//ADCУ׼
	ADC_Cmd(ADC1, ENABLE);                       //ʹ��ָ����ADC1
	ADC_ResetCalibration(ADC1);                  //ʹ�ܸ�λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);                  //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));       //�ȴ�У׼����
}

u16  Get_Adc(u8 ch) {                      //��ȡchͨ����ADCת����ֵ
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		                        //ʹ��ָ����ADC1�����ת����������	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));                     //�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	                            //�������һ��ADC1�������ת�����
}

u16  Get_Adc_Average(u8 ch,u8 times) {     //ƽ��times���ȡ�ĵ�ѹ
	u32 temp_val=0,t;
	for(t=0;t<times;++t) {
		temp_val+=Get_Adc(ch);
	}
	return temp_val/times;
}

u16  Get_Adc_Middle(u8 ch,u16 times) {     //��ֵ��ȡ�ĵ�ѹ
	u16 a[505]={0};
	int len=0;
	int i;
	for(i=0;i<times;++i) {
		a[len++]=Get_Adc_Average(ch,20);
	}
	Sort(a,len);
	
	return a[len/2];
}
