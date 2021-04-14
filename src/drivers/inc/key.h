/****************************************************************************************
@file     key.h
@brief    KEY驱动头文件
@author   石国强
@version  V1.0
@introduction KEY1->PA11 KEY2->PA12
*****************************************************************************************/

#ifndef __KEY_H__
#define __KEY_H__

#include "sys.h"

#define KEY1_PERS 1
#define KEY2_PERS 2

#define KEY1 PAin(11)
#define KEY2 PAin(12)

void KEY_Init(void);               //按键初始化

#endif
