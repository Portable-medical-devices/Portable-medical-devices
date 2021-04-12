/****************************************************************************************
@file     adc.c
@brief    ADC源文件
@author   石国强
@version  V1.0
@function ADC1 1 -> PA1
*****************************************************************************************/

#include "adc.h"
#include "mymath.h"

void Adc_Init(void) {                            //ADC初始化
	ADC_InitTypeDef ADC_InitStructure;           //ADC实例化
	GPIO_InitTypeDef GPIO_InitStructure;         //GPIO实例化
	//使能GPIOA和ADC1系统时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1,ENABLE);    
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);            //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	//初始化GPIO
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;      //PA1口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;  //模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);       //初始化GPIOA
	//复位ADC1
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                //ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	                    //模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;             	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	            //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                            //顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	                                //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
	//ADC校准
	ADC_Cmd(ADC1, ENABLE);                       //使能指定的ADC1
	ADC_ResetCalibration(ADC1);                  //使能复位校准
	while(ADC_GetResetCalibrationStatus(ADC1));  //等待复位校准结束
	ADC_StartCalibration(ADC1);                  //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));       //等待校准结束
}

u16  Get_Adc(u8 ch) {                      //获取ch通道的ADC转换的值
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		                        //使能指定的ADC1的软件转换启动功能	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));                     //等待转换结束
	return ADC_GetConversionValue(ADC1);	                            //返回最近一次ADC1规则组的转换结果
}

u16  Get_Adc_Average(u8 ch,u8 times) {     //平均times后获取的电压
	u32 temp_val=0,t;
	for(t=0;t<times;++t) {
		temp_val+=Get_Adc(ch);
	}
	return temp_val/times;
}

u16  Get_Adc_Middle(u8 ch,u16 times) {     //中值获取的电压
	u16 a[505]={0};
	int len=0;
	int i;
	for(i=0;i<times;++i) {
		a[len++]=Get_Adc_Average(ch,20);
	}
	Sort(a,len);
	
	return a[len/2];
}
