/****************************************************************************************
@file     mpu6050.h
@brief    �����Ǵ�����ͷ�ļ�
@author   ʯ��ǿ
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

#define DEV_ADDR	0xD0	// 6050 ������ַ

//MPU6050�ڴ�Ĵ�����ַ
#define SMPLRT_DIV   0x19   //�����ǲ�����,����ֵ:0x07(125Hz)
#define CONFIG       0x1A   //��ͨ�˲�Ƶ�ʣ�����ֵ:0x06(5Hz)
#define GYRO_CONFIG  0x1B   //�������Լ켰������Χ������ֵ:0x18(���Լ죬2000deg/s)
#define ACCEL_CONFIG 0x1C   //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ:0x01(���Լ죬2G��5Hz)

//���ٶ���ؼĴ���
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

//�¶���ؼĴ�����ַ
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

//��������ؼĴ���
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define PWR_MGMT_1 0x6B   //��Դ����,����ֵ:0x00(��������)
#define WHO_AM_I   0x75   //IIC��ַ�Ĵ���(Ĭ��0x68��ֻ��)
#define SlaveAddress 0xD0 //IICд��ʱ�ĵ�ַ�ֽ�����+1λ��ȡ

void MPU6050_Init(void);                                     //MPU6050��ʼ��
void MPU6050_Write_Reg(uint8_t regAddr, uint8_t regData);    //��Ĵ�����д����
uint8_t MPU6050_Read_Reg(uint8_t regAddr);                   //�ӼĴ����ж�ȡ��ֵ
int16_t MPU6050_Get_Data(uint8_t regAddr);                   //�ϳ�16λ����
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);             //����д�������
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);              //�������������
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);     //��ȡ���ٶ�ԭʼ����   
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);         //��ȡ������ԭʼ����
void MPU_Step_Count(void);                                   //��ȡ��������

#endif
