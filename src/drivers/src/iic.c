/****************************************************************************************
@file     iic.c
@brief    ���ģ��iicЭ��
@author   ʯ��ǿ
@version  V1.0
@introduction SCL->PB8 SDA->PB9
*****************************************************************************************/

#include "iic.h"

void delay(void) {                     
	uint16_t t=10;         //��ʱ
	while(t--); 
}

void MPU_I2C_Init(void) {                                  //I2C��ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;                   //ʵ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //ʹ��GPIOB
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;         //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;        //50MHz
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;     //PB8\PB9
	GPIO_Init(GPIOB,&GPIO_InitStructure);                  //��ʼ��GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);             //PB8\PB9�����
}

void MPU_I2C_Start(void) {                 //I2CЭ����ʼ�ź�
	SDA_OUT();                             //����SDAΪ���
	SDA=1;                                 //ģ�⿪ʼ�ź�
	SCL=1;
	delay();
	SDA=0;
	delay();
	SCL=0;
	delay();
}

void MPU_I2C_Stop(void) {                 //I2CЭ����ֹ�ź�
	SDA_OUT();                            //����SDAΪ���
	SDA=0;                                //ģ����ֹ�ź�
	SCL=1;
	delay();
	SDA=1;
	delay();
}

void MPU_I2C_Write_Byte(uint8_t byte) {    //���Ͱ�λ����
	uint8_t i;
	SDA_OUT();                             //����SDAΪ���
	SCL=0;                                 //ʱ��������
	for(i=0;i<8;++i) {
		if(byte&(0x80>>i)) SDA=1;          //�Ӹ�����д����
		else               SDA=0;
		SCL=1;                             //ʱ�ӷ�������
		delay();
		SCL=0;
		delay();
	}
}

uint8_t MPU_I2C_Read_Byte(void) {          //��ȡ��λ����
	uint8_t i,t=0;
	for(i=0;i<8;++i) {
		SDA_OUT();  
		SDA=1;		
		SCL=1;
		delay();
		t<<=1;
		SDA_IN();
		if(RSDA) t++;                      //���Ϊ�ߵ�ƽ�����λ��Ϊ1
		SCL=0;
		delay();
	}
	return t;
}

//
uint8_t MPU_I2C_Read_ACK(void) {           //����Ӧ���ź�
	uint8_t ucErrTime=0;
	SDA_OUT();                             //SDA����Ϊ���
	SDA=1;
	SCL=1;
	delay();
	SDA_IN();                              //����Ϊ����
	while(RSDA) {                          //�鿴�������ŵ�ƽ
		ucErrTime++;
		if(ucErrTime>250) {
			return 1;                      //û���յ�Ӧ��
		}
	}
	SCL=0;
	delay();
	return 0;                              //�յ�Ӧ��
}

//����Ӧ���ź� 0����Ӧ���ź� 1���Ͳ�Ӧ���ź� 
void MPU_I2C_Write_ACK(uint8_t ack) {      //����Ӧ���ź�
	SDA_OUT();
	SCL=0;
	if(ack)  SDA=1;
	else     SDA=0;
	SCL=1;
	delay();
	SCL=0;
	delay();
}

