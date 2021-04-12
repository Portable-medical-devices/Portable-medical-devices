/****************************************************************************************
@file     mymath.c
@brief    数学源文件
@author   石国强
@version  V1.0
@introduction 排序
*****************************************************************************************/

#include "mymath.h"

void Sort(u16 *a,int len) {
	int i,j;
	int flag=1;
	int t;
	for(i=0;i<len-1&&flag;++i) {
		flag=0;
		for(j=0;j<len-1-i;++j) {
			if(a[j]>a[j+1]) {
				flag=1;
				t=a[j];
				a[j]=a[j+1];
				a[j+1]=t;
			}
		}
	}
}

