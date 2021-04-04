/****************************************************************************************
@file     iic.h
@brief    软件模拟iic协议
@author   石国强
@version  V1.0
@introduction SCL->PB8 SDA->PB9
*****************************************************************************************/

#ifndef __IIC_H__
#define __IIC_H__

#include "stm32f10x_gpio.h"
#include "sys.h"

#define SCL  PBout(8)                    //PB8位带操作
#define SDA  PBout(9)                    //PB9输出位带操作
#define RSDA PBin(9)                     //PB9输入位带操作

#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}

void MPU_I2C_Init(void);                     //I2C初始化
void MPU_I2C_Start(void);                    //I2C协议起始信号
void MPU_I2C_Stop(void);                     //I2C协议终止信号
void MPU_I2C_Write_Byte(uint8_t byte);       //发送八位数据
uint8_t MPU_I2C_Read_Byte(void);             //读取八位数据
uint8_t MPU_I2C_Read_ACK(void);              //接受应答信号
void MPU_I2C_Write_ACK(uint8_t ack);         //发送应答信号

#endif
