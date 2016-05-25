#ifndef  __STm32_UB_LCD_DRIVE_H
#define  __STm32_UB_LCD_DRIVE_H

#include "stm32f10x.h"
//#include "stm32f10x_spi.h"


#define  USE_HARDWARE_SPI 1  //1:Enable Hardware SPI;0:USE Soft SPI
#define LCD_X_SIZE	        240
#define LCD_Y_SIZE	        320

//支持横竖屏快速定义切换;
#define USE_HORIZONTAL  		1	//定义是否使用横屏 		0,不使用.1,使用.;


/* 字体属性结构, 用于LCD_DispStr() */
typedef struct
{
	uint16_t usFontCode;	/* 字体代码 0 表示16点阵 */
	uint16_t usTextColor;	/* 字体颜色 */
	uint16_t usBackColor;	/* 文字背景颜色，透明 */
	uint16_t usSpace;		/* 文字间距，单位 = 像素 */
}FONT_T;

enum
{
	CL_WHITE        = 0xFFFF,	/* 白色 */
	CL_BLACK        = 0x0000,	/* 黑色 */
	CL_GREY         = 0xF7DE,	/* 灰色 */
	CL_BLUE         = 0x001F,	/* 蓝色 */
	CL_BLUE2        = 0x051F,	/* 浅蓝色 */
	CL_RED          = 0xF800,	/* 红色 */
	CL_MAGENTA      = 0xF81F,	/* 红紫色，洋红色 */
	CL_GREEN        = 0x07E0,	/* 绿色 */
	CL_CYAN         = 0x7FFF,	/* 蓝绿色，青色 */
	CL_YELLOW       = 0xFFE0,	/* 黄色 */
	CL_MASK					= 0x9999,	/* 颜色掩码，用于文字背景透明 */
	CL_GRAY1   				=	0x8410,      	//灰色1      00000 000000 00000
	CL_GRAY2   				=	0x4208      	//灰色2  1111111111011111
};

enum
{
	FC_ST_16X16 = 0,		/* 宋体15x16点阵 （宽x高） */
	FC_ST_24X24 = 1			/* 宋体24x24点阵 （宽x高） */
};


#ifdef USE_HORIZONTAL//如果定义了横屏 ;
#define X_MAX_PIXEL	        LCD_Y_SIZE
#define Y_MAX_PIXEL	        LCD_X_SIZE
#else
#define X_MAX_PIXEL	        LCD_X_SIZE
#define Y_MAX_PIXEL	        LCD_Y_SIZE
#endif


#define LCD_CTRL   	  	GPIOB		//定义TFT数据端口
#define LCD_LED        	GPIO_Pin_1  //MCU_PB9--->>TFT --BL-pb1
#define LCD_RS         	GPIO_Pin_11	//PB10--->>TFT --RS/DC-pb11
//#define LCD_CS        	GPIO_Pin_11 //MCU_PB11--->>TFT --CS/CE  已接地；
#define LCD_RST     	GPIO_Pin_12	//Pb12--->>TFT --RST
#define LCD_SCL        	GPIO_Pin_13	//Pb13--->>TFT --SCL/SCK
#define LCD_SDA        	GPIO_Pin_15	//Pb15 MOSI--->>TFT --SDA/DIN

//液晶控制口置1操作语句宏定义；
//#define	LCD_CS_SET  	LCD_CTRL->BSRR=LCD_CS    
#define	LCD_RS_SET  	GPIOB->BSRR=LCD_RS    
#define	LCD_SDA_SET  	GPIOB->BSRR=LCD_SDA    
#define	LCD_SCL_SET  	GPIOB->BSRR=LCD_SCL    
#define	LCD_RST_SET  	GPIOB->BSRR=LCD_RST    
#define	LCD_LED_SET  	GPIOB->BSRR=LCD_LED 

//液晶控制口置0操作语句宏定义；
//#define	LCD_CS_CLR  	LCD_CTRL->BRR=LCD_CS    
#define	LCD_RS_CLR  	GPIOB->BRR=LCD_RS    
#define	LCD_SDA_CLR  	GPIOB->BRR=LCD_SDA    
#define	LCD_SCL_CLR  	GPIOB->BRR=LCD_SCL    
#define	LCD_RST_CLR  	GPIOB->BRR=LCD_RST    
#define	LCD_LED_CLR  	GPIOB->BRR=LCD_LED 

// #define LCD_DATAOUT(x) LCD_DATA->ODR=x; //数据输出;
// #define LCD_DATAIN     LCD_DATA->IDR;   //数据输入;


extern void Lcd_Init(void);  //lcd初始化函数
extern void Lcd_SetXY(u16 Xpos, u16 Ypos);
extern void Gui_DrawPoint(u16 x,u16 y,u16 Data);
extern void Lcd_Clear(u16 color);

/*
#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
LCD_DATAOUT(data);\
LCD_WR_CLR;\

}
*/

/*
#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111


#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define MAGENTA       	 0xF81F
#define CYAN          	 0x7FFF
#define BROWN 			 0XBC40 //棕色;
#define BRRED 			 0XFC07 //棕红色;
#define GRAY  			 0X8430 //灰色;
//GUI颜色;

#define DARKBLUE      	 0X01CF	//深蓝色;
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  ;
#define GRAYBLUE       	 0X5458 //灰蓝色;
//以上三色为PANEL的颜色 ;

#define LIGHTGREEN     	 0X841F //浅绿色;
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL);
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色;

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色);
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色);

*/


#endif


