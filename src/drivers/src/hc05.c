/****************************************************************************************
@file     HC05.c
@brief    ��������Դ�ļ�
@author   ʯ��ǿ
@version  V1.0
@introduction HC05(RX)->PA2(TX) HC05(TX)->PA3(RX) USART2
*****************************************************************************************/

#include "hc05.h"

u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA=0;       //����״̬���	 

void HC05_Init(u32 bound) {                           //����ģ���ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//ʹ��USART2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE);	
	USART_DeInit(USART2);                              //��λ����2
	//USART_TX PA2                                     
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;            //Pin2
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //50MHz
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;      //�������
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA2
	//USART_RX PA3
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;            //Pin3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;        //��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA3
	//USART2 NVIC����
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);                    //NVICʵ����
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;                   //һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;        //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;           //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2,&USART_InitStructure);                      //��ʼ������
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 	
}

void USART2_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#ifdef SYSTEM_SUPPORT_OS	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//��ȡ���յ�������
		
		if((USART2_RX_STA&0x8000)==0)//����δ���
			{
			if(USART2_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
				else USART2_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
					{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#ifdef SYSTEM_SUPPORT_OS	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 

void HC05_Send_Data(u8 *buf,u8 len) {        //HC05��������
	u8 i=0;
	for(i=0;i<len;++i) {
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET);
		USART_SendData(USART2,buf[i]);
	}
}
