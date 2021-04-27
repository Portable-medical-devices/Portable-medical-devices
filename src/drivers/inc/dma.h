#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"
#include "stm32f10x_dma.h"

void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar);
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len);

void u3_dma_init(void);

void UART3_TX_DMA_Enable(uint8_t *data,u16 len);

/****LCD****/
void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//≈‰÷√DMA1_CHx
void MYDMA_Config1(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);// πƒ‹DMA1_CHx

#endif




