/****************************************************************************************
@file     oled.c
@brief    oled�����ļ�
@author   ʯ��ǿ
@version  V1.0
@introduction D0->PA5 D1->PA7 RES->PB0 DC->PB1 CS->GND
D0->ʱ����(SCK) D1->������(MOSI) RES->����/���ݱ�־λ(��λ) DC->����/���ݱ�־λ
���ù��ܺ���
*****************************************************************************************/

#include "oled.h"
#include "oledfont.h"

u8 OLED_GRAM[128][8];       //�Դ�

void OLED_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	//GPIO��ʼ��
	//��ʼ��PA5��PA7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //ʹ��GPIOA
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;          //�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;      //PA5��PA7
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;         //�ٶ�50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);                   //��ʼ��GPIOA
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7);              //PA(5)��PA(7)�ø�
	//��ʼ��PB0��PB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);    //ʹ��GPIOB
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;          //�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;      //PB0��PB1
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;         //�ٶ�50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   //��ʼ��GPIOB
	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);			    //PB(0)��PB(1)�ø�
	//OLED��λ
	OLED_Reset();      
	//OLEDָ������
	OLED_WR_Byte(0xAE,OLED_CMD);         //�ر���Ļ
	OLED_WR_Byte(0x00,OLED_CMD);         //���õ��е�ַ
	OLED_WR_Byte(0x10,OLED_CMD);         //���ø��е�ַ
	OLED_WR_Byte(0x40,OLED_CMD);         //���ÿ�ʼ��
	OLED_WR_Byte(0x81,OLED_CMD);         //���öԱȶ�
	OLED_WR_Byte(0xCF,OLED_CMD);         //��������
	OLED_WR_Byte(0xA1,OLED_CMD);         //����ϸ����ӳ��(0xA1������0xA0���ҷ���)
	OLED_WR_Byte(0xC8,OLED_CMD);         //���������ɨ�跽��(0xC8����,0xC0���·���)
	OLED_WR_Byte(0xA6,OLED_CMD);         //��������/������ʾ(0xA6����,0xA7����)
	OLED_WR_Byte(0xA8,OLED_CMD);         //���ö�·������
	OLED_WR_Byte(0x3F,OLED_CMD);         
	OLED_WR_Byte(0xD3,OLED_CMD);         //������ʾƫ��(��ֱƫ��)
	OLED_WR_Byte(0x00,OLED_CMD);         
	OLED_WR_Byte(0xD5,OLED_CMD);         //������ʾʱ�ӷ�Ƶ
	OLED_WR_Byte(0x80,OLED_CMD);         
	OLED_WR_Byte(0xD9,OLED_CMD);         //����Ԥ�����
	OLED_WR_Byte(0xF1,OLED_CMD);         
	OLED_WR_Byte(0xDA,OLED_CMD);         
	OLED_WR_Byte(0x12,OLED_CMD);         
	OLED_WR_Byte(0xDB,OLED_CMD);         
	OLED_WR_Byte(0x40,OLED_CMD);         
	OLED_WR_Byte(0x20,OLED_CMD);         //�ڴ�Ѱַ����(��ؼ�)
	OLED_WR_Byte(0x02,OLED_CMD);
	OLED_WR_Byte(0x8D,OLED_CMD);
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xA4,OLED_CMD);
	OLED_WR_Byte(0xA6,OLED_CMD);
	OLED_WR_Byte(0xAF,OLED_CMD);         //������ʾ
	OLED_Clear();
}

void OLED_Clear(void) {                  //��������
	u8 i,j;
	for(i=0;i<8;++i) {
		OLED_WR_Byte(0xB0+i,OLED_CMD);   //��������ʼ��ַ
		OLED_WR_Byte(0x00,OLED_CMD);     //��������ʼ��ַ
		OLED_WR_Byte(0x10,OLED_CMD);     //���ø�����ʼ��ַ
		for(j=0;j<128;++j) {
			OLED_WR_Byte(0x00,OLED_DATA); //��
		}
	}
}
 
void OLED_Full(void) {                   //ȫ��
	u8 i,j;
	for(i=0;i<8;++i) {
		OLED_WR_Byte(0xB0+i,OLED_CMD);   //��������ʼ��ַ
		OLED_WR_Byte(0x00,OLED_CMD);     //���õ�����ʼ��ַ
		OLED_WR_Byte(0x10,OLED_CMD);     //���ø�����ʼ��ַ
		for(j=0;j<128;++j) {
			OLED_WR_Byte(0xFF,OLED_DATA); //��
		}
	}
}

void OLED_Reset(void) {
	OLED_RESET(1); 
	delay_ms(100); 
	OLED_RESET(0);                        //RES�͵�ƽ�ź�
	delay_ms(100);                        //��ʱ
	OLED_RESET(1);                        //RES�ߵ�ƽ�ź� 
}

void OLED_WR_Byte(u8 data,u8 cmd) {       //д����
	u8 i;
	OLED_DC(OLED_CMD);                    //Ĭ��Ϊ����ģʽ
	if(cmd) OLED_DC(OLED_DATA);           //���������ģʽ,��������
	for(i=0;i<8;++i) {                    //�Ӹ����λд
		OLED_SCK(0);                      //ʱ��������
		if(data&0x80) { OLED_MOSI(1); }   //д��
		else { OLED_MOSI(0) }             //д��
		OLED_SCK(1);                      //ʱ��������
		data<<=1;                         //����
	}
}

void OLED_DisplayTurn(u8 i) {              //��Ļ��ת180��
	if(i==0) {
		OLED_WR_Byte(0xC8,OLED_CMD);       //������ʾ
		OLED_WR_Byte(0xA1,OLED_CMD);
	} else {
		OLED_WR_Byte(0xC0,OLED_CMD);       //��ת��ʾ
		OLED_WR_Byte(0xA0,OLED_CMD);
	}
}

void OLED_Display_On(void) {               //����OLED��ʾ
	OLED_WR_Byte(0x8D,OLED_CMD);           //��ɱ�ʹ��
	OLED_WR_Byte(0x14,OLED_CMD);           //������ɱ�
	OLED_WR_Byte(0xAF,OLED_CMD);           //������Ļ	
}                                          

void OLED_Display_Off(void) {              //�ر�OLED��ʾ
	OLED_WR_Byte(0x8D,OLED_CMD);           //��ɱ�ʹ��
	OLED_WR_Byte(0x10,OLED_CMD);           //�رյ�ɱ�
	OLED_WR_Byte(0xAF,OLED_CMD);           //�ر���Ļ
}                                                

void OLED_Refresh(void) {                  //�����Դ����
	u8 i,n;
	for(i=0;i<8;i++) {
	   OLED_WR_Byte(0xB0+i,OLED_CMD); //��������ʼ��ַ
	   OLED_WR_Byte(0x00,OLED_CMD);   //���õ�����ʼ��ַ
	   OLED_WR_Byte(0x10,OLED_CMD);   //���ø�����ʼ��ַ
	   for(n=0;n<128;n++) OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
    }
}

void OLED_DrawPoint(u8 x,u8 y) {      //����
	u8 page,col;
	page=y/8;                         //����ҳ��
	col=1<<(y%8);                     //������ֵ
	OLED_GRAM[x][page]|=col;          //������ֵ
}

void OLED_ClearPoint(u8 x,u8 y) {     //���һ����
	u8 page;
	page=y/8;                         //����ҳ��
	OLED_GRAM[x][page]&=~(1<<(y%8));  
}

void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2) {             //����
	int i,k,b;
	if(((int)x1<0)||(x2>128)||((int)y1<0)||(y2>64)||(x1>x2)||(y1>y2)) return;   //�������ֱ�ӷ���
	if(x1==x2) {
		for(i=0;i<y2-y1;++i) OLED_DrawPoint(x1,y1+i);     //������
	} else if(y1==y2) {
		for(i=0;i<x2-x1;++i) OLED_DrawPoint(x1+i,y1);     //������
	} else {
		k=(y2-y1)/(x2-x1);
		b=y1-k*x1;                                        //��б��,�����Է���
		for(i=0;i<(x2-x1);++i) OLED_DrawPoint(x1+i,k*(x1+i)+b);             
	}
}

void OLED_ColorTurn(u8 i) {                      //��ʾģʽ
	if(i==0) OLED_WR_Byte(0xA6,OLED_CMD);        //�ڵװ���
	else     OLED_WR_Byte(0xA7,OLED_CMD);        //�׵׺���

}

//size:12 16 24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size) {        //��ʾ�ַ�
	u8 i,m,temp,size_temp,offset;                     
	u8 y0=y;                                          
	size_temp=(size/8+((size%8)?1:0))*(size/2);       //�����һ���ַ����ֽ���
	offset=chr-' ';                                   //����ƫ����
	for(i=0;i<size_temp;++i) {
		if(size==12)      temp=asc2_1206[offset][i];  //����12������
		else if(size==16) temp=asc2_1608[offset][i];  //����16������
		else if(size==24) temp=asc2_2412[offset][i];  //����24������
		else return;
		for(m=0;m<8;++m) {
			if(temp&0x80) OLED_DrawPoint(x,y);        //��ֵΪ1����
			else          OLED_ClearPoint(x,y);       //��ֵΪ0����
			temp<<=1;                                 //����
			y++;
			if((y-y0)==size) {                        //y��������
				y=y0;                                 //����y��x
				x++;
				break;
			}
		}
	}
}

void OLED_ShowString(u8 x,u8 y,char *chr,u8 size) {     //��ʾ�ַ���
	while((*chr>=' ')&&(*chr<='~')) {                 //�ж��Ƿ�Ϊ��Ч�ַ�
		OLED_ShowChar(x,y,*chr,size);                 //��ʾ�ַ�
		x+=size/2;                                    //����x��
		if(x>128-size) {                              //���һ����ʾ����
			x=0;                                    
			y+=(size/8+((size%8)?1:0))*8;             //����y��
		}
		chr++;                                        //�����ַ�
	}
}

void OLED_ShowNum(u8 x,u8 y,int num,u8 size) {        //��ʾ����
	char buf[100];                                    //��������ַ�
	sprintf(buf,"%d",num);                            //��ʽ������ת�ַ�
	OLED_ShowString(x,y,buf,size);                    //��ʾ�ַ�
}

void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]) {   //��ʾͼƬ
	u32 j=0;
	u8 x=0,y=0;
	for(y=y0;y<y1;++y) {
		OLED_WR_Byte(0xb0+y,OLED_CMD);//��������ʼ��ַ
		OLED_WR_Byte(((x0&0xf0)>>4)|0x10,OLED_CMD);
		OLED_WR_Byte((x0&0x0f),OLED_CMD);
		for(x=x0;x<x1;++x) OLED_WR_Byte(BMP[j++],OLED_DATA);		
	}
	
}
