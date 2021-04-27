#ifndef fdacoefs_h
#define fdacoefs_h

#include "stm32f10x.h"
#include "arm_math.h"

#define TEST_LENGTH_SAMPLES 320         //采样点数  (因为在实际使用的时候，是传输一个数据处理一个数据，所以不需要事先确定需要处理的总采样点的个数)
#define BLOCK_SIZE 1                   //调用一次arm_fir_f32处理采样点个数
#define NUM_TAPS 201                     //滤波器系数个数



void ecg_fir_tilter_init(void);
float ecg_fir_tilter(float input);

#endif

