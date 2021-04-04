/****************************************************************************************
@file     iic.h
@brief    ���ģ��iicЭ��
@author   ʯ��ǿ
@version  V1.0
@introduction SCL->PB8 SDA->PB9
*****************************************************************************************/

#ifndef __IIC_H__
#define __IIC_H__

#include "stm32f10x_gpio.h"
#include "sys.h"

#define SCL  PBout(8)                    //PB8λ������
#define SDA  PBout(9)                    //PB9���λ������
#define RSDA PBin(9)                     //PB9����λ������

#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}

void MPU_I2C_Init(void);                     //I2C��ʼ��
void MPU_I2C_Start(void);                    //I2CЭ����ʼ�ź�
void MPU_I2C_Stop(void);                     //I2CЭ����ֹ�ź�
void MPU_I2C_Write_Byte(uint8_t byte);       //���Ͱ�λ����
uint8_t MPU_I2C_Read_Byte(void);             //��ȡ��λ����
uint8_t MPU_I2C_Read_ACK(void);              //����Ӧ���ź�
void MPU_I2C_Write_ACK(uint8_t ack);         //����Ӧ���ź�

#endif
