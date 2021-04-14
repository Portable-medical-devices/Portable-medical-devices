/****************************************************************************************
@file     mpu6050.c
@brief    �����Ǵ�����Դ�ļ�
@author   ʯ��ǿ
@version  V1.0
@introduction SCL->PB8 SDA->PB9 VCC->5V 
*****************************************************************************************/

#include "mpu6050.h"
#include "delay.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

void MPU6050_Init(void) {                       //MPU6050��ʼ��
	MPU_I2C_Init();                             //��ʼ��I2C����
	MPU6050_Write_Reg(PWR_MGMT_1,0x80);         //��λMPU6050
	delay_ms(50);
	MPU6050_Write_Reg(PWR_MGMT_1,0x00);         //�������״̬
	MPU6050_Write_Reg(SMPLRT_DIV, 0x07);        //�����ǲ����ʣ�����ֵ��0x07(125Hz)
	MPU6050_Write_Reg(CONFIG, 0x06);            //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
	MPU6050_Write_Reg(GYRO_CONFIG, 0x18);       //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
	MPU6050_Write_Reg(ACCEL_CONFIG, 0x01);      //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
}

void MPU6050_Write_Reg(uint8_t regAddr, uint8_t regData) {    //��Ĵ�����д����
	MPU_I2C_Start();                            //������ʼ�ź�
	//д�豸��ַ
	MPU_I2C_Write_Byte(DEV_ADDR); 
	if(MPU_I2C_Read_ACK()) goto stop;
	//д�Ĵ�����ַ
	MPU_I2C_Write_Byte(regAddr);
	if (MPU_I2C_Read_ACK())goto stop;
	//д���ݵ��Ĵ���
	MPU_I2C_Write_Byte(regData);
	if (MPU_I2C_Read_ACK())goto stop;
stop:
	MPU_I2C_Stop();
}

uint8_t MPU6050_Read_Reg(uint8_t regAddr) {         //�ӼĴ����ж�ȡ��ֵ
	uint8_t regData;
	//������ʼ�ź�
	MPU_I2C_Start();
	//�����豸��ַ
	MPU_I2C_Write_Byte(DEV_ADDR);
	if (MPU_I2C_Read_ACK()) goto stop;
	//���ͼĴ�����ַ
	MPU_I2C_Write_Byte(regAddr);
	if (MPU_I2C_Read_ACK()) goto stop;
	//�ظ���ʼ�ź�
	MPU_I2C_Start();
	//���Ͷ�ģʽ
	MPU_I2C_Write_Byte(DEV_ADDR | 0x01);
	if (MPU_I2C_Read_ACK()) goto stop;
	//���Ĵ�������
	regData=MPU_I2C_Read_Byte();
	MPU_I2C_Write_ACK(1);   //дһ����Ӧ���ź�
stop:
	MPU_I2C_Stop();
	return regData;
}

int16_t MPU6050_Get_Data(uint8_t regAddr) {         //�ϳ�16λ����
	uint8_t Data_H,Data_L;
	uint16_t data;
	Data_H = MPU6050_Read_Reg(regAddr);             //��ȡ��8λ
	Data_L = MPU6050_Read_Reg(regAddr + 1);
	data = (Data_H << 8) | Data_L;                  //��ȡ��8λ
	return data;
}

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf) {   //����д�������
	u8 i; 
    MPU_I2C_Start(); 
	MPU_I2C_Write_Byte((addr<<1)|0);                //����������ַ+д����	
	if(MPU_I2C_Read_ACK())	                        //�ȴ�Ӧ��
	{
		MPU_I2C_Stop();		 
		return 1;		
	}
    MPU_I2C_Write_Byte(reg);						//д�Ĵ�����ַ
    MPU_I2C_Read_ACK();								//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		MPU_I2C_Write_Byte(buf[i]);					//��������
		if(MPU_I2C_Read_ACK())						//�ȴ�ACK
		{
			MPU_I2C_Stop();	 
			return 1;		 
		}		
	}    
    MPU_I2C_Stop();	 
	return 0;	
}

u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf) {    //�������������
 	MPU_I2C_Start(); 
	MPU_I2C_Write_Byte((addr<<1)|0); 				//����������ַ+д����	
	if(MPU_I2C_Read_ACK())							//�ȴ�Ӧ��
	{
		MPU_I2C_Stop();		 
		return 1;		
	}
    MPU_I2C_Write_Byte(reg);						//д�Ĵ�����ַ
    MPU_I2C_Read_ACK();								//�ȴ�Ӧ��
    MPU_I2C_Start();
	MPU_I2C_Write_Byte((addr<<1)|1);				//����������ַ+������	
    MPU_I2C_Read_ACK();								//�ȴ�Ӧ�� 
	while(len)
	{
		if(len==1){
			*buf=MPU_I2C_Read_Byte();				//������,����nACK 
			MPU_I2C_Write_ACK(1);
		}
		else {
			*buf=MPU_I2C_Read_Byte();				//������,����ACK  
			MPU_I2C_Write_ACK(0);
		}
		len--;
		buf++; 
	}    
    MPU_I2C_Stop();									//����һ��ֹͣ���� 
	return 0;	
}

void MPU_Step_Count(void) {                          //��ȡ��������
	dmp_get_pedometer_step_count(&user.walk.step);       //��ȡ���߲���
	user.walk.distance=0.6*user.walk.step;                    //��ȡ����
}

