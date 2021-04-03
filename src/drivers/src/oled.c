/****************************************************************************************
@file     oled.c
@brief    oled驱动文件
@author   石国强
@version  V1.0
@introduction D0->PA5 D1->PA7 RES->PB0 DC->PB1 CS->GND
D0->时钟线(SCK) D1->数据线(MOSI) RES->命令/数据标志位(复位) DC->命令/数据标志位
常用功能函数
*****************************************************************************************/

#include "oled.h"
#include "oledfont.h"

u8 OLED_GRAM[128][8];       //显存

void OLED_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	//GPIO初始化
	//初始化PA5、PA7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //使能GPIOA
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;          //推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;      //PA5、PA7
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;         //速度50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);                   //初始化GPIOA
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7);              //PA(5)、PA(7)置高
	//初始化PB0、PB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);    //使能GPIOB
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;          //推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;      //PB0、PB1
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;         //速度50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   //初始化GPIOB
	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);			    //PB(0)、PB(1)置高
	//OLED复位
	OLED_Reset();      
	//OLED指令配置
	OLED_WR_Byte(0xAE,OLED_CMD);         //关闭屏幕
	OLED_WR_Byte(0x00,OLED_CMD);         //设置低列地址
	OLED_WR_Byte(0x10,OLED_CMD);         //设置高列地址
	OLED_WR_Byte(0x40,OLED_CMD);         //设置开始行
	OLED_WR_Byte(0x81,OLED_CMD);         //设置对比度
	OLED_WR_Byte(0xCF,OLED_CMD);         //设置亮度
	OLED_WR_Byte(0xA1,OLED_CMD);         //设置细分重映射(0xA1正常，0xA0左右反置)
	OLED_WR_Byte(0xC8,OLED_CMD);         //设置列输出扫描方向(0xC8正常,0xC0上下反置)
	OLED_WR_Byte(0xA6,OLED_CMD);         //设置正常/反向显示(0xA6正常,0xA7反向)
	OLED_WR_Byte(0xA8,OLED_CMD);         //设置多路服用率
	OLED_WR_Byte(0x3F,OLED_CMD);         
	OLED_WR_Byte(0xD3,OLED_CMD);         //设置显示偏移(垂直偏移)
	OLED_WR_Byte(0x00,OLED_CMD);         
	OLED_WR_Byte(0xD5,OLED_CMD);         //设置显示时钟分频
	OLED_WR_Byte(0x80,OLED_CMD);         
	OLED_WR_Byte(0xD9,OLED_CMD);         //设置预充电期
	OLED_WR_Byte(0xF1,OLED_CMD);         
	OLED_WR_Byte(0xDA,OLED_CMD);         
	OLED_WR_Byte(0x12,OLED_CMD);         
	OLED_WR_Byte(0xDB,OLED_CMD);         
	OLED_WR_Byte(0x40,OLED_CMD);         
	OLED_WR_Byte(0x20,OLED_CMD);         //内存寻址设置(最关键)
	OLED_WR_Byte(0x02,OLED_CMD);
	OLED_WR_Byte(0x8D,OLED_CMD);
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xA4,OLED_CMD);
	OLED_WR_Byte(0xA6,OLED_CMD);
	OLED_WR_Byte(0xAF,OLED_CMD);         //开启显示
	OLED_Clear();
}

void OLED_Clear(void) {                  //清屏函数
	u8 i,j;
	for(i=0;i<8;++i) {
		OLED_WR_Byte(0xB0+i,OLED_CMD);   //设置行起始地址
		OLED_WR_Byte(0x00,OLED_CMD);     //设置列起始地址
		OLED_WR_Byte(0x10,OLED_CMD);     //设置高列起始地址
		for(j=0;j<128;++j) {
			OLED_WR_Byte(0x00,OLED_DATA); //灭
		}
	}
}
 
void OLED_Full(void) {                   //全亮
	u8 i,j;
	for(i=0;i<8;++i) {
		OLED_WR_Byte(0xB0+i,OLED_CMD);   //设置行起始地址
		OLED_WR_Byte(0x00,OLED_CMD);     //设置低列起始地址
		OLED_WR_Byte(0x10,OLED_CMD);     //设置高列起始地址
		for(j=0;j<128;++j) {
			OLED_WR_Byte(0xFF,OLED_DATA); //亮
		}
	}
}

void OLED_Reset(void) {
	OLED_RESET(1); 
	delay_ms(100); 
	OLED_RESET(0);                        //RES低电平信号
	delay_ms(100);                        //延时
	OLED_RESET(1);                        //RES高电平信号 
}

void OLED_WR_Byte(u8 data,u8 cmd) {       //写数据
	u8 i;
	OLED_DC(OLED_CMD);                    //默认为命令模式
	if(cmd) OLED_DC(OLED_DATA);           //如果是数据模式,进行修正
	for(i=0;i<8;++i) {                    //从高向低位写
		OLED_SCK(0);                      //时钟线拉低
		if(data&0x80) { OLED_MOSI(1); }   //写高
		else { OLED_MOSI(0) }             //写低
		OLED_SCK(1);                      //时钟线拉高
		data<<=1;                         //左移
	}
}

void OLED_DisplayTurn(u8 i) {              //屏幕旋转180度
	if(i==0) {
		OLED_WR_Byte(0xC8,OLED_CMD);       //正常显示
		OLED_WR_Byte(0xA1,OLED_CMD);
	} else {
		OLED_WR_Byte(0xC0,OLED_CMD);       //反转显示
		OLED_WR_Byte(0xA0,OLED_CMD);
	}
}

void OLED_Display_On(void) {               //开启OLED显示
	OLED_WR_Byte(0x8D,OLED_CMD);           //电荷泵使能
	OLED_WR_Byte(0x14,OLED_CMD);           //开启电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);           //点亮屏幕	
}                                          

void OLED_Display_Off(void) {              //关闭OLED显示
	OLED_WR_Byte(0x8D,OLED_CMD);           //电荷泵使能
	OLED_WR_Byte(0x10,OLED_CMD);           //关闭电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);           //关闭屏幕
}                                                

void OLED_Refresh(void) {                  //根据显存更新
	u8 i,n;
	for(i=0;i<8;i++) {
	   OLED_WR_Byte(0xB0+i,OLED_CMD); //设置行起始地址
	   OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
	   OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
	   for(n=0;n<128;n++) OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
    }
}

void OLED_DrawPoint(u8 x,u8 y) {      //画点
	u8 page,col;
	page=y/8;                         //计算页数
	col=1<<(y%8);                     //计算数值
	OLED_GRAM[x][page]|=col;          //更新数值
}

void OLED_ClearPoint(u8 x,u8 y) {     //清除一个点
	u8 page;
	page=y/8;                         //计算页数
	OLED_GRAM[x][page]&=~(1<<(y%8));  
}

void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2) {             //画线
	int i,k,b;
	if(((int)x1<0)||(x2>128)||((int)y1<0)||(y2>64)||(x1>x2)||(y1>y2)) return;   //错误情况直接返回
	if(x1==x2) {
		for(i=0;i<y2-y1;++i) OLED_DrawPoint(x1,y1+i);     //画竖线
	} else if(y1==y2) {
		for(i=0;i<x2-x1;++i) OLED_DrawPoint(x1+i,y1);     //画横线
	} else {
		k=(y2-y1)/(x2-x1);
		b=y1-k*x1;                                        //画斜线,用线性方程
		for(i=0;i<(x2-x1);++i) OLED_DrawPoint(x1+i,k*(x1+i)+b);             
	}
}

void OLED_ColorTurn(u8 i) {                      //显示模式
	if(i==0) OLED_WR_Byte(0xA6,OLED_CMD);        //黑底白字
	else     OLED_WR_Byte(0xA7,OLED_CMD);        //白底黑字

}

//size:12 16 24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size) {        //显示字符
	u8 i,m,temp,size_temp,offset;                     
	u8 y0=y;                                          
	size_temp=(size/8+((size%8)?1:0))*(size/2);       //计算出一个字符的字节数
	offset=chr-' ';                                   //计算偏移量
	for(i=0;i<size_temp;++i) {
		if(size==12)      temp=asc2_1206[offset][i];  //调用12号字体
		else if(size==16) temp=asc2_1608[offset][i];  //调用16号字体
		else if(size==24) temp=asc2_2412[offset][i];  //调用24号字体
		else return;
		for(m=0;m<8;++m) {
			if(temp&0x80) OLED_DrawPoint(x,y);        //数值为1则亮
			else          OLED_ClearPoint(x,y);       //数值为0则灭
			temp<<=1;                                 //左移
			y++;
			if((y-y0)==size) {                        //y轴遍历完毕
				y=y0;                                 //更新y、x
				x++;
				break;
			}
		}
	}
}

void OLED_ShowString(u8 x,u8 y,char *chr,u8 size) {     //显示字符串
	while((*chr>=' ')&&(*chr<='~')) {                 //判断是否为有效字符
		OLED_ShowChar(x,y,*chr,size);                 //显示字符
		x+=size/2;                                    //修正x轴
		if(x>128-size) {                              //如果一行显示不下
			x=0;                                    
			y+=(size/8+((size%8)?1:0))*8;             //修正y轴
		}
		chr++;                                        //遍历字符
	}
}

void OLED_ShowNum(u8 x,u8 y,int num,u8 size) {        //显示数字
	char buf[100];                                    //存放数字字符
	sprintf(buf,"%d",num);                            //格式化数字转字符
	OLED_ShowString(x,y,buf,size);                    //显示字符
}

void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]) {   //显示图片
	u32 j=0;
	u8 x=0,y=0;
	for(y=y0;y<y1;++y) {
		OLED_WR_Byte(0xb0+y,OLED_CMD);//设置行起始地址
		OLED_WR_Byte(((x0&0xf0)>>4)|0x10,OLED_CMD);
		OLED_WR_Byte((x0&0x0f),OLED_CMD);
		for(x=x0;x<x1;++x) OLED_WR_Byte(BMP[j++],OLED_DATA);		
	}
	
}
