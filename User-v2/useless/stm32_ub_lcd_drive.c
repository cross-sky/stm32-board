#include "cominc.h"

//VCC:可以接5V也可以接3.3V;
//LED:可以接5V也可以接3.3V或者使用任意空闲IO控制(高电平使能);
//GND：接电源地;
//说明：如需要尽可能少占用IO，可以将LCD_CS接地，LCD_LED接3.3V，LCD_RST接至单片机复位端，;
//将可以释放3个可用IO;
//接口定义在Lcd_Driver.h内定义，;
//如需变更IO接法，请根据您的实际接线修改相应IO初始化LCD_GPIO_Init();

void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LCD_LED| LCD_RS| LCD_RST | LCD_SCL| LCD_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void SPIv_WriteData(u8 Data)
{
	u8 i;
	for (i=8; i>0; i--)
	{
		if (Data&0x80)
			LCD_SDA_SET;
		else
			LCD_SDA_CLR;

		LCD_SCL_CLR;
		LCD_SCL_SET;
		Data <<= 1;
		
	}
}


//STM32_硬件SPI读写一个字节数据底层函数;
u8 SPI_WriteByte(SPI_TypeDef* SPIx, u8 Byte)
{
	while( (SPIx->SR & SPI_I2S_FLAG_TXE) == RESET);
	SPIx->DR = Byte;
	while( (SPIx->SR & SPI_I2S_FLAG_RXNE) == RESET);
	return SPIx->DR;

}

//设置SPI的速度;
void SPI_SetSpeed( SPI_TypeDef* SPIx, u8 SpeedSet)
{
	SPIx->CR1 &= 0xffc7;
	if ( SpeedSet == 1 )
		SPIx->CR1 |= SPI_BaudRatePrescaler_2;  //Fsck=Fpclk/2;
	else
		SPIx->CR1 |= SPI_BaudRatePrescaler_32;

	SPIx->CR1 |= 1<<6; //SPI设备使能;
}

//STM32_SPI2硬件配置初始化;
void SPI2_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_SDA|LCD_SCL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCD_RST;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCD_LED| LCD_RS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	SPI_Cmd(SPI2, ENABLE);

}

//写一个8位指令;
void Lcd_WriteIndex( u8 Index)
{
//	LCD_CS_CLR;
	LCD_RS_CLR;
#if USE_HARDWARE_SPI
	SPI_WriteByte(SPI2, Index);
#else
	SPIv_WriteData(Index);
#endif

//	LCD_CS_SET;
}

//写一个8位数据;
void Lcd_WriteData(u8 Data)
{
//	LCD_CS_CLR;
	LCD_RS_SET;
#if USE_HARDWARE_SPI
	SPI_WriteByte(SPI2,Data);
#else
	SPIv_WriteData(Data);
#endif

//	LCD_CS_SET;

}

void Lcd_WriteReg(u8 Index, u16 Data)
{
	Lcd_WriteIndex(Index);
	Lcd_WriteData( Data>>8 );
	Lcd_WriteData(Data);
}

void Lcd_WriteData_16Bit(u16 Data)
{
	Lcd_WriteData(Data>>8);
	Lcd_WriteData(Data);
}


static void DelayMs(u16 tim)
{
	u16 i;
	while(tim--)
	{
		i=7200;
		while( i-- );
	}

}

void Lcd_Reset(void)
{
	LCD_RST_CLR;
	DelayMs(100);
	LCD_RST_SET;
	DelayMs(100);
}

void Lcd_Init(void)
{
#if USE_HARDWARE_SPI
	SPI2_Init();
#else
	LCD_GPIO_Init();
#endif
	LCD_LED_SET;
	Lcd_Reset();
	Lcd_WriteIndex(0xCB);
	Lcd_WriteData(0x39); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x34); 
	Lcd_WriteData(0x02); 

	Lcd_WriteIndex(0xCF);  
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0XC1); 
	Lcd_WriteData(0X30); 

	Lcd_WriteIndex(0xE8);  
	Lcd_WriteData(0x85); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x78);

	Lcd_WriteIndex(0xEA);  
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x00); 

	Lcd_WriteIndex(0xED);  
	Lcd_WriteData(0x64); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0X12); 
	Lcd_WriteData(0X81); 

	Lcd_WriteIndex(0xF7);  
	Lcd_WriteData(0x20); 

	Lcd_WriteIndex(0xC0);    //Power control 
	Lcd_WriteData(0x23);   //VRH[5:0] 

	Lcd_WriteIndex(0xC1);    //Power control 
	Lcd_WriteData(0x10);   //SAP[2:0];BT[3:0] 

	Lcd_WriteIndex(0xC5);    //VCM control 
	Lcd_WriteData(0x3e); //对比度调节;
	Lcd_WriteData(0x28); 

	Lcd_WriteIndex(0xC7);    //VCM control2 
	Lcd_WriteData(0x86);  //--

	Lcd_WriteIndex(0x36);    // Memory Access Control 
#ifdef USE_HORIZONTAL
	Lcd_WriteData(0x38); //C8	   //48 68竖屏//28 E8 横屏;
#else
	Lcd_WriteData(0xC8); 
#endif

	Lcd_WriteIndex(0x3A);    
	Lcd_WriteData(0x55); 

	Lcd_WriteIndex(0xB1);    
	Lcd_WriteData(0x00);  
	Lcd_WriteData(0x18); 

	Lcd_WriteIndex(0xB6);    // Display Function Control 
	Lcd_WriteData(0x08); 
	Lcd_WriteData(0x82);
	Lcd_WriteData(0x27);  

	Lcd_WriteIndex(0xF2);    // 3Gamma Function Disable 
	Lcd_WriteData(0x00); 

	Lcd_WriteIndex(0x26);    //Gamma curve selected 
	Lcd_WriteData(0x01); 

	Lcd_WriteIndex(0xE0);    //Set Gamma 
	Lcd_WriteData(0x0F); 
	Lcd_WriteData(0x31); 
	Lcd_WriteData(0x2B); 
	Lcd_WriteData(0x0C); 
	Lcd_WriteData(0x0E); 
	Lcd_WriteData(0x08); 
	Lcd_WriteData(0x4E); 
	Lcd_WriteData(0xF1); 
	Lcd_WriteData(0x37); 
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x10); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0x0E); 
	Lcd_WriteData(0x09); 
	Lcd_WriteData(0x00); 

	Lcd_WriteIndex(0XE1);    //Set Gamma 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x0E); 
	Lcd_WriteData(0x14); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0x11); 
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x31); 
	Lcd_WriteData(0xC1); 
	Lcd_WriteData(0x48); 
	Lcd_WriteData(0x08); 
	Lcd_WriteData(0x0F); 
	Lcd_WriteData(0x0C); 
	Lcd_WriteData(0x31); 
	Lcd_WriteData(0x36); 
	Lcd_WriteData(0x0F); 

	Lcd_WriteIndex(0x11);    //Exit Sleep 

	DelayMs(1000);

	Lcd_WriteIndex(0x29);    //Display on 
	Lcd_WriteIndex(0x2c); 
	
	Lcd_Clear(CL_BLUE);
}

void Lcd_SetXY(u16 Xpos, u16 Ypos)
{
	Lcd_WriteReg(0x2a, Xpos);
	Lcd_WriteReg(0x2b, Ypos);
	Lcd_WriteIndex(0x2c);
	
}

/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行;
入口参数：xy起点和终点;
返回值：无;
*************************************************/
void Lcd_setRegion(u16 xStar,u16 yStar, u16 xEnd, u16 yEnd)
{
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData_16Bit(xStar);
	Lcd_WriteData_16Bit(xEnd);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData_16Bit(yStar);
	Lcd_WriteData_16Bit(yEnd);
	Lcd_WriteIndex(0x2c);
}

//函数名：LCD_DrawPoint
///	功能：画一个点;
//	入口参数：xy坐标和颜色数据;
//	返回值：无;
void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
	Lcd_SetXY(x,y);
	Lcd_WriteData_16Bit(Data);
}

void Lcd_Clear(u16 color)
{
	u16 i,j;
	
	Lcd_setRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
	for (i=0; i<X_MAX_PIXEL; i++)
	{
		for (j=0;j<Y_MAX_PIXEL; j++)
		{
			Lcd_WriteData_16Bit(color);
		}
	}
}


