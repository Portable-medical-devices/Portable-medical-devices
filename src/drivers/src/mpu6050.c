/****************************************************************************************
@file     mpu6050.c
@brief    陀螺仪传感器源文件
@author   石国强
@version  V1.0
@introduction SCL->PB8 SDA->PB9 VCC->5V 
*****************************************************************************************/

#include "mpu6050.h"
#include "delay.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

void MPU6050_Init(void) {                       //MPU6050初始化
	MPU_I2C_Init();                             //初始化I2C总线
	MPU6050_Write_Reg(PWR_MGMT_1,0x80);         //复位MPU6050
	delay_ms(50);
	MPU6050_Write_Reg(PWR_MGMT_1,0x00);         //解除休眠状态
	MPU6050_Write_Reg(SMPLRT_DIV, 0x07);        //陀螺仪采样率，典型值：0x07(125Hz)
	MPU6050_Write_Reg(CONFIG, 0x06);            //低通滤波频率，典型值：0x06(5Hz)
	MPU6050_Write_Reg(GYRO_CONFIG, 0x18);       //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	MPU6050_Write_Reg(ACCEL_CONFIG, 0x01);      //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
}

void MPU6050_Write_Reg(uint8_t regAddr, uint8_t regData) {    //向寄存器中写数据
	MPU_I2C_Start();                            //发送起始信号
	//写设备地址
	MPU_I2C_Write_Byte(DEV_ADDR); 
	if(MPU_I2C_Read_ACK()) goto stop;
	//写寄存器地址
	MPU_I2C_Write_Byte(regAddr);
	if (MPU_I2C_Read_ACK())goto stop;
	//写数据到寄存器
	MPU_I2C_Write_Byte(regData);
	if (MPU_I2C_Read_ACK())goto stop;
stop:
	MPU_I2C_Stop();
}

uint8_t MPU6050_Read_Reg(uint8_t regAddr) {         //从寄存器中读取数值
	uint8_t regData;
	//发送起始信号
	MPU_I2C_Start();
	//发送设备地址
	MPU_I2C_Write_Byte(DEV_ADDR);
	if (MPU_I2C_Read_ACK()) goto stop;
	//发送寄存器地址
	MPU_I2C_Write_Byte(regAddr);
	if (MPU_I2C_Read_ACK()) goto stop;
	//重复起始信号
	MPU_I2C_Start();
	//发送读模式
	MPU_I2C_Write_Byte(DEV_ADDR | 0x01);
	if (MPU_I2C_Read_ACK()) goto stop;
	//读寄存器数据
	regData=MPU_I2C_Read_Byte();
	MPU_I2C_Write_ACK(1);   //写一个非应答信号
stop:
	MPU_I2C_Stop();
	return regData;
}

int16_t MPU6050_Get_Data(uint8_t regAddr) {         //合成16位数据
	uint8_t Data_H,Data_L;
	uint16_t data;
	Data_H = MPU6050_Read_Reg(regAddr);             //读取低8位
	Data_L = MPU6050_Read_Reg(regAddr + 1);
	data = (Data_H << 8) | Data_L;                  //读取高8位
	return data;
}

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf) {   //连续写多个数据
	u8 i; 
    MPU_I2C_Start(); 
	MPU_I2C_Write_Byte((addr<<1)|0);                //发送器件地址+写命令	
	if(MPU_I2C_Read_ACK())	                        //等待应答
	{
		MPU_I2C_Stop();		 
		return 1;		
	}
    MPU_I2C_Write_Byte(reg);						//写寄存器地址
    MPU_I2C_Read_ACK();								//等待应答
	for(i=0;i<len;i++)
	{
		MPU_I2C_Write_Byte(buf[i]);					//发送数据
		if(MPU_I2C_Read_ACK())						//等待ACK
		{
			MPU_I2C_Stop();	 
			return 1;		 
		}		
	}    
    MPU_I2C_Stop();	 
	return 0;	
}

u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf) {    //连续读多个数据
 	MPU_I2C_Start(); 
	MPU_I2C_Write_Byte((addr<<1)|0); 				//发送器件地址+写命令	
	if(MPU_I2C_Read_ACK())							//等待应答
	{
		MPU_I2C_Stop();		 
		return 1;		
	}
    MPU_I2C_Write_Byte(reg);						//写寄存器地址
    MPU_I2C_Read_ACK();								//等待应答
    MPU_I2C_Start();
	MPU_I2C_Write_Byte((addr<<1)|1);				//发送器件地址+读命令	
    MPU_I2C_Read_ACK();								//等待应答 
	while(len)
	{
		if(len==1){
			*buf=MPU_I2C_Read_Byte();				//读数据,发送nACK 
			MPU_I2C_Write_ACK(1);
		}
		else {
			*buf=MPU_I2C_Read_Byte();				//读数据,发送ACK  
			MPU_I2C_Write_ACK(0);
		}
		len--;
		buf++; 
	}    
    MPU_I2C_Stop();									//产生一个停止条件 
	return 0;	
}

void MPU_Step_Count(void) {                          //获取行走属性
	dmp_get_pedometer_step_count(&user.walk.step);       //获取行走步数
	user.walk.distance=0.6*user.walk.step;                    //获取距离
}

