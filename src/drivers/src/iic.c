/****************************************************************************************
@file     iic.c
@brief    软件模拟iic协议
@author   石国强
@version  V1.0
@introduction SCL->PB8 SDA->PB9
*****************************************************************************************/

#include "iic.h"

void delay(void) {                     
	uint16_t t=10;         //延时
	while(t--); 
}

void MPU_I2C_Init(void) {                                  //I2C初始化
	GPIO_InitTypeDef GPIO_InitStructure;                   //实例化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //使能GPIOB
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;         //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;        //50MHz
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;     //PB8\PB9
	GPIO_Init(GPIOB,&GPIO_InitStructure);                  //初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);             //PB8\PB9输出高
}

void MPU_I2C_Start(void) {                 //I2C协议起始信号
	SDA_OUT();                             //配置SDA为输出
	SDA=1;                                 //模拟开始信号
	SCL=1;
	delay();
	SDA=0;
	delay();
	SCL=0;
	delay();
}

void MPU_I2C_Stop(void) {                 //I2C协议终止信号
	SDA_OUT();                            //配置SDA为输出
	SDA=0;                                //模拟终止信号
	SCL=1;
	delay();
	SDA=1;
	delay();
}

void MPU_I2C_Write_Byte(uint8_t byte) {    //发送八位数据
	uint8_t i;
	SDA_OUT();                             //配置SDA为输出
	SCL=0;                                 //时钟线拉低
	for(i=0;i<8;++i) {
		if(byte&(0x80>>i)) SDA=1;          //从高往低写数据
		else               SDA=0;
		SCL=1;                             //时钟发生跳变
		delay();
		SCL=0;
		delay();
	}
}

uint8_t MPU_I2C_Read_Byte(void) {          //读取八位数据
	uint8_t i,t=0;
	for(i=0;i<8;++i) {
		SDA_OUT();  
		SDA=1;		
		SCL=1;
		delay();
		t<<=1;
		SDA_IN();
		if(RSDA) t++;                      //如果为高电平则最低位变为1
		SCL=0;
		delay();
	}
	return t;
}

//
uint8_t MPU_I2C_Read_ACK(void) {           //接受应答信号
	uint8_t ucErrTime=0;
	SDA_OUT();                             //SDA配置为输出
	SDA=1;
	SCL=1;
	delay();
	SDA_IN();                              //配置为输入
	while(RSDA) {                          //查看输入引脚电平
		ucErrTime++;
		if(ucErrTime>250) {
			return 1;                      //没有收到应答
		}
	}
	SCL=0;
	delay();
	return 0;                              //收到应答
}

//产生应答信号 0发送应答信号 1发送不应答信号 
void MPU_I2C_Write_ACK(uint8_t ack) {      //发送应答信号
	SDA_OUT();
	SCL=0;
	if(ack)  SDA=1;
	else     SDA=0;
	SCL=1;
	delay();
	SCL=0;
	delay();
}

