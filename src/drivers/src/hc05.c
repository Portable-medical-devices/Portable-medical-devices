/****************************************************************************************
@file     HC05.c
@brief    蓝牙驱动源文件
@author   石国强
@version  V1.0
@introduction HC05(RX)->PA2(TX) HC05(TX)->PA3(RX) USART2
*****************************************************************************************/

#include "hc05.h"

u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	 

void HC05_Init(u32 bound) {                           //蓝牙模块初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//使能USART2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE);	
	USART_DeInit(USART2);                              //复位串口2
	//USART_TX PA2                                     
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;            //Pin2
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //50MHz
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;      //推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA2
	//USART_RX PA3
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;            //Pin3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;        //输入上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA3
	//USART2 NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);                    //NVIC实例化
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;                   //一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;        //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;           //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2,&USART_InitStructure);                      //初始化串口
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART2, ENABLE);                    //使能串口 	
}

void USART2_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#ifdef SYSTEM_SUPPORT_OS	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
		
		if((USART2_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART2_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
				else USART2_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
					{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#ifdef SYSTEM_SUPPORT_OS	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 

void HC05_Send_Data(u8 *buf,u8 len) {        //HC05发送数据
	u8 i=0;
	for(i=0;i<len;++i) {
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET);
		USART_SendData(USART2,buf[i]);
	}
}
