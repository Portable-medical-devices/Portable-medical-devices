#include "dma.h"
#include "delay.h"       //��ʱ




 
//DMA�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�ڴ��ַ
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar)
{
	
		RCC->AHBENR |= 1<<0;				//����DMA1ʱ��		
		delay_ms(10);
	
		//DMA_CHx -> CNDTR = len;	//���ݴ��������65535
		DMA_CHx -> CPAR = (u32)cpar;    //�趨����Ĵ�����ַ ���͵�ַ
		DMA_CHx -> CMAR = (u32)cmar;    //�趨���ݴ洢����ַ ���յ�ַ
		
		//CCR	�Ĵ��� 0-14Ϊ����λ		
		DMA_CHx -> CCR  = 0;  							//��λ������Ĵ���
		
		//�����ε��жϣ����жϺ����в�Ҫ�ж����־λ����Ȼ�ᴥ���������ԭ��
		DMA_CHx -> CCR  	|= 0<<1;         //��������ж�				1���� 0��ֹ
		DMA_CHx -> CCR  	|= 0<<2;         //�봫���ж� 				1���� 0��ֹ
		DMA_CHx -> CCR		|= 0<<3;         //����������ж�		1���� 0��ֹ	  ��дһ�������ĵ�ַ����,�������DMA������� 
	
		//�趨���ݴ��䷽��
		DMA_CHx -> CCR  |= 1<<4;           //�趨���ݴ��䷽��   0�������贫�����ڴ� 1�����ڴ洫��������
		DMA_CHx -> CCR  |= 0<<5;           //0����ִ��ѭ������ 1��ִ��ѭ������           
		//�趨����ʹ洢����ַ����
		DMA_CHx -> CCR  |= 0<<6;           //0�������ַ������ 1�������ַ����
		DMA_CHx -> CCR  |= 1<<7;           //0���ڴ��ַ������ 1���ڴ��ַ����
		//�趨�������ݿ��
		DMA_CHx -> CCR  |= 0<<8;           //�������ݿ��,��[9:8]��λ����
		DMA_CHx -> CCR  |= 0<<9;           //00��8λ 01��16λ 10��32λ 11������  
		//�趨�洢���ݿ��
		DMA_CHx -> CCR  |= 0<<10;          //�洢�����ݿ��,��[11:10]��λ����
		DMA_CHx -> CCR  |= 0<<11;          //00��8λ 01��16λ 10��32λ 11������  			
		//�趨Ϊ�е����ȼ�
		DMA_CHx -> CCR  |= 0<<12;          //ͨ�����ȼ�,��[13:12]��λ����
		DMA_CHx -> CCR  |= 0<<13;          //00���� 01���� 10���� 11����� 
		//�洢��ʽ
		DMA_CHx -> CCR  |= 0<<14;          //0�����ڴ浽�ڴ棻 1���ڴ洫�䵽�ڴ档     

} 


//����һ��DMA����
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len)
{
	DMA_CHx -> CCR &= ~(1<<0);			//�رյ�ǰ��DMA���䣨������û�д�����ɣ�
    DMA_CHx -> CNDTR = len;				//�������ݴ�����
//	DMA_CHx -> CMAR = 
    DMA_CHx -> CCR |= 1<<0;				//��ʼDMA����  
}
void u3_dma_init(void)
{
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE); //DMA����ʹ��
	DMA1_Channel2 -> CCR &= ~(1<<0);			//�رյ�ǰ��DMA���䣨������û�д�����ɣ�
    DMA1_Channel2->CPAR = (uint32_t) & (USART3->DR);
}
//����һ��DMA����
void UART3_TX_DMA_Enable(uint8_t *data,u16 len)
{
	DMA1_Channel2 -> CCR &= ~(1<<0);			//�رյ�ǰ��DMA���䣨������û�д�����ɣ�	
	DMA1_Channel2 -> CMAR = (uint32_t)data;		//�������ݵ�ַ
    DMA1_Channel2 -> CNDTR = len;				//�������ݴ�����
    DMA1_Channel2 -> CCR |= 1<<0;				//��ʼDMA����
}

/********LCD SPI1 DMA********/
DMA_InitTypeDef DMA_InitStructure;

u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 	    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
	DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	  	
} 
void MYDMA_Config1(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
  DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
} 
 //����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );
 	DMA_SetCurrDataCounter(DMA1_Channel3,DMA1_MEM_LEN);
 	DMA_Cmd(DMA_CHx, ENABLE);
}	








