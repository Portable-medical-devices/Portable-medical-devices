/****************************************************************************************
@file     mpu6050.h
@brief    陀螺仪传感器头文件
@author   石国强
@version  V1.0
@introduction SCL->PB8 SDA->PB9 VCC->5V 
*****************************************************************************************/

#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "iic.h"
#include "sys.h"
#include "user.h"

#define X_ACCEL_OFFSET  930
#define Y_ACCEL_OFFSET  60
#define Z_ACCEL_OFFSET  70

#define X_GYRO_OFFSET   41
#define Y_GYRO_OFFSET   -12
#define Z_GYRO_OFFSET   18

#define DEV_ADDR	0xD0	// 6050 器件地址

//MPU6050内存寄存器地址
#define SMPLRT_DIV   0x19   //陀螺仪采样率,典型值:0x07(125Hz)
#define CONFIG       0x1A   //低通滤波频率，典型值:0x06(5Hz)
#define GYRO_CONFIG  0x1B   //陀螺仪自检及测量范围，典型值:0x18(不自检，2000deg/s)
#define ACCEL_CONFIG 0x1C   //加速计自检、测量范围及高通滤波频率，典型值:0x01(不自检，2G，5Hz)

//加速度相关寄存器
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

//温度相关寄存器地址
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

//陀螺仪相关寄存器
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define PWR_MGMT_1 0x6B   //电源管理,典型值:0x00(正常启用)
#define WHO_AM_I   0x75   //IIC地址寄存器(默认0x68，只读)
#define SlaveAddress 0xD0 //IIC写入时的地址字节数据+1位读取

void MPU6050_Init(void);                                     //MPU6050初始化
void MPU6050_Write_Reg(uint8_t regAddr, uint8_t regData);    //向寄存器中写数据
uint8_t MPU6050_Read_Reg(uint8_t regAddr);                   //从寄存器中读取数值
int16_t MPU6050_Get_Data(uint8_t regAddr);                   //合成16位数据
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);             //连续写多个数据
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);              //连续读多个数据
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);     //获取加速度原始数据   
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);         //获取陀螺仪原始数据
void MPU_Step_Count(void);                                   //获取行走属性

#endif
