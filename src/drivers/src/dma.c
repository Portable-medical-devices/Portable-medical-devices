#include "dma.h"
#include "delay.h"       //延时




 
//DMA的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:内存地址
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar)
{
	
		RCC->AHBENR |= 1<<0;				//开启DMA1时钟		
		delay_ms(10);
	
		//DMA_CHx -> CNDTR = len;	//数据传输量最大65535
		DMA_CHx -> CPAR = (u32)cpar;    //设定外设寄存器地址 发送地址
		DMA_CHx -> CMAR = (u32)cmar;    //设定数据存储器地址 接收地址
		
		//CCR	寄存器 0-14为设置位		
		DMA_CHx -> CCR  = 0;  							//复位，清零寄存器
		
		//被屏蔽的中断，在中断函数中不要判断其标志位，不然会触发，不清楚原因
		DMA_CHx -> CCR  	|= 0<<1;         //传输完成中断				1允许 0禁止
		DMA_CHx -> CCR  	|= 0<<2;         //半传输中断 				1允许 0禁止
		DMA_CHx -> CCR		|= 0<<3;         //允许传输错误中断		1允许 0禁止	  读写一个保留的地址区域,将会产生DMA传输错误 
	
		//设定数据传输方向
		DMA_CHx -> CCR  |= 1<<4;           //设定数据传输方向   0：从外设传输至内存 1：从内存传输至外设
		DMA_CHx -> CCR  |= 0<<5;           //0：不执行循环操作 1：执行循环操作           
		//设定外设和存储器地址增量
		DMA_CHx -> CCR  |= 0<<6;           //0：外设地址不自增 1：外设地址自增
		DMA_CHx -> CCR  |= 1<<7;           //0：内存地址不自增 1：内存地址自增
		//设定外设数据宽度
		DMA_CHx -> CCR  |= 0<<8;           //外设数据宽度,由[9:8]两位控制
		DMA_CHx -> CCR  |= 0<<9;           //00：8位 01：16位 10：32位 11：保留  
		//设定存储数据宽度
		DMA_CHx -> CCR  |= 0<<10;          //存储器数据宽度,由[11:10]两位控制
		DMA_CHx -> CCR  |= 0<<11;          //00：8位 01：16位 10：32位 11：保留  			
		//设定为中等优先级
		DMA_CHx -> CCR  |= 0<<12;          //通道优先级,由[13:12]两位控制
		DMA_CHx -> CCR  |= 0<<13;          //00：低 01：中 10：高 11：最高 
		//存储方式
		DMA_CHx -> CCR  |= 0<<14;          //0：非内存到内存； 1：内存传输到内存。     

} 


//开启一次DMA传输
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len)
{
	DMA_CHx -> CCR &= ~(1<<0);			//关闭当前的DMA传输（不管有没有传输完成）
    DMA_CHx -> CNDTR = len;				//设置数据传输量
//	DMA_CHx -> CMAR = 
    DMA_CHx -> CCR |= 1<<0;				//开始DMA传输  
}
void u3_dma_init(void)
{
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE); //DMA发送使能
	DMA1_Channel2 -> CCR &= ~(1<<0);			//关闭当前的DMA传输（不管有没有传输完成）
    DMA1_Channel2->CPAR = (uint32_t) & (USART3->DR);
}
//开启一次DMA传输
void UART3_TX_DMA_Enable(uint8_t *data,u16 len)
{
	DMA1_Channel2 -> CCR &= ~(1<<0);			//关闭当前的DMA传输（不管有没有传输完成）	
	DMA1_Channel2 -> CMAR = (uint32_t)data;		//设置数据地址
    DMA1_Channel2 -> CNDTR = len;				//设置数据传输量
    DMA1_Channel2 -> CCR |= 1<<0;				//开始DMA传输
}

/********LCD SPI1 DMA********/
DMA_InitTypeDef DMA_InitStructure;

u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 	    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值
	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	  	
} 
void MYDMA_Config1(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
  DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值
	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //内存地址寄存器不变
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
} 
 //开启一次DMA传输
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );
 	DMA_SetCurrDataCounter(DMA1_Channel3,DMA1_MEM_LEN);
 	DMA_Cmd(DMA_CHx, ENABLE);
}	








