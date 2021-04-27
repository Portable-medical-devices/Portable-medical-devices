#ifndef fdacoefs_h
#define fdacoefs_h

#include "stm32f10x.h"
#include "arm_math.h"

#define TEST_LENGTH_SAMPLES 320         //��������  (��Ϊ��ʵ��ʹ�õ�ʱ���Ǵ���һ�����ݴ���һ�����ݣ����Բ���Ҫ����ȷ����Ҫ������ܲ�����ĸ���)
#define BLOCK_SIZE 1                   //����һ��arm_fir_f32������������
#define NUM_TAPS 201                     //�˲���ϵ������



void ecg_fir_tilter_init(void);
float ecg_fir_tilter(float input);

#endif

