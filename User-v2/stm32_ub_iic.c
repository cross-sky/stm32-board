#include "cominc.h"

char iic[16];
//iic发送/接收，1字节15us
// pb5 pb6
//
void IIC_Init(void)
{
	uint8_t len;
	GPIO_InitTypeDef gpio_init_structure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

	gpio_init_structure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio_init_structure.GPIO_Speed = GPIO_Speed_10MHz;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_OD;

	GPIO_Init(GPIOB,&gpio_init_structure);
	gpio_init_structure.GPIO_Pin = GPIO_Pin_12;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&gpio_init_structure);

	GPIOB->BSRR = GPIO_Pin_10;
#ifdef Debug
	//printf("IIC done....");
	sprintf(iic, "%s","iic on...\r\n");
	len = strlen(iic);
	UartDMAQueue(qUartLink,(uint8_t*)iic,len);
#endif // Debug

}

static void i2c1_delay( void)
{
	uint8_t i=5;
	while ( i-- );
}


bool i2c_start(void)
{
	SDA_H;
	SCL_H;
	i2c1_delay();
	if( !SDA_READ ) return FALSE;
	SDA_L;
	i2c1_delay();
	if( SDA_READ ) return FALSE;
	SCL_L; //......................
	i2c1_delay();//when clk is high ,sda chang from high to low
	return TRUE;
}

void i2c_stop(void)
{
	SCL_L;
	i2c1_delay();
	SDA_L;
	i2c1_delay();
	SCL_H;
	i2c1_delay();
	SDA_H;
	i2c1_delay();

}

void i2c_ack(void)
{
	SCL_L;
	i2c1_delay();
	SDA_L;
	i2c1_delay();
	SCL_H;
	i2c1_delay();
	SCL_L;
	i2c1_delay();
}

void i2c_no_ack(void)
{
	SCL_L;
	i2c1_delay();
	SDA_H;
	i2c1_delay();
	SCL_H;
	i2c1_delay();
	SCL_L;
	i2c1_delay();
}

bool i2c_wait_ack( void )
{
	uint8_t ucErrTime = 0;
	SCL_L;
	i2c1_delay();
	SDA_H;
	i2c1_delay();
	SCL_H;
	i2c1_delay();
	while( SDA_READ )
	{
		ucErrTime++;
		if(ucErrTime > 50)
		{
			SCL_L;
			i2c_stop();
			return FALSE;
		}
		i2c1_delay();
	}
	SCL_L;
	i2c1_delay();
	return TRUE;

}

void IIC_SendByte(uint8_t txd)
{
	uint8_t i=8;
	while ( i-- )
	{
		SCL_L;
		i2c1_delay();
		if(txd&0x80)
			SDA_H;
		else
			SDA_L;
		txd <<=1;
		i2c1_delay();
		SCL_H;
		i2c1_delay();

	}
	SCL_L;
}

uint8_t IIC_ReadByteAck(uint8_t ack)
{
	uint8_t i=8;
	uint8_t receive_data = 0;

	SDA_H;
	while ( i-- )
	{
		receive_data <<= 1;
		SCL_L;
		i2c1_delay();
		SCL_H;
		i2c1_delay();
		if( SDA_READ)
			receive_data |= 0x01;

	}
	SCL_L;

	if( !ack)
		i2c_no_ack();
	else
		i2c_ack();

	return receive_data;
}

uint8_t I2C_ReadOneByte(uint8_t I2C_Addr, uint8_t reg_addr)
{
	uint8_t res=0;
	if(!i2c_start() ) return FALSE;
	IIC_SendByte(I2C_Addr);

	if ( !i2c_wait_ack() )
	{
		i2c_stop();
		return FALSE;
	}
	IIC_SendByte(reg_addr);
	i2c_wait_ack();
	i2c_start();
	IIC_SendByte(I2C_Addr + 1);
	i2c_wait_ack();
	res = IIC_ReadByteAck(0);
	i2c_stop();

	return res;
}

/**************************实现函数********************************************
*函数原型:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*功　　能:	    读取指定设备 指定寄存器的 length个值
输入	dev  目标设备地址
		reg	  寄存器地址
		length 要读的字节数
		*data  读出的数据将要存放的指针
返回   读出来的字节数量
*******************************************************************************/ 

uint8_t IICReadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
	uint8_t res=0, count;
	if(!i2c_start() ) return FALSE;
	IIC_SendByte(dev);

	if ( !i2c_wait_ack() )
	{
		i2c_stop();
		return FALSE;
	}
	IIC_SendByte(reg);
	i2c_wait_ack();
	i2c_start();
	IIC_SendByte(dev + 1);
	i2c_wait_ack();

	for (count=0; count<length; count++)
	{
		if(count != (length - 1))
			res = IIC_ReadByteAck(1);
		else
			res = IIC_ReadByteAck(0);
		data[count] = res;
	}
	i2c_stop();
	return count;
}



/**************************实现函数********************************************
*函数原型:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*功　　能:	    读取指定设备 指定寄存器的 length个值
输入	dev  目标设备地址
		reg	  寄存器地址
		length 要读的字节数
		*data  读出的数据将要存放的指针
返回   读出来的字节数量
*******************************************************************************/ 
/*
uint8_t IICReadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
	uint8_t res=0, count;
	i2c_start();
	IIC_SendByte(dev);

	i2c_wait_ack();

	IIC_SendByte(reg);
	i2c_wait_ack();
	i2c_start();
	IIC_SendByte(dev + 1);
	i2c_wait_ack();

	for (count=0; count<length; count++)
	{
		if(count != (length - 1))
			res = IIC_ReadByteAck(1);
		else
			res = IIC_ReadByteAck(0);
		data[count] = res;
	}
	i2c_stop();
	return count;
}
*/
/**************************实现函数********************************************
*函数原型:		u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*功　　能:	    将多个字节写入指定设备 指定寄存器
输入	dev  目标设备地址
		reg	  寄存器地址
		length 要写的字节数
		*data  将要写的数据的首地址
返回   返回是否成功
*******************************************************************************/ 

uint8_t IICWriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
	uint8_t count = 0;

	if(!i2c_start() ) return FALSE;
	IIC_SendByte(dev);

	if ( !i2c_wait_ack() )
	{
		i2c_stop();
		return FALSE;
	}
	IIC_SendByte(reg);
	i2c_wait_ack();
	for (count = 0; count < length; count++)
	{
		IIC_SendByte(data[count]);
		i2c_wait_ack();
	}
	i2c_stop();

	return 1;
	
}



/*
uint8_t IICWriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
	uint8_t count = 0;

	i2c_start();
	IIC_SendByte(dev);

	i2c_wait_ack();

	IIC_SendByte(reg);
	i2c_wait_ack();
	for (count = 0; count < length; count++)
	{
		IIC_SendByte(data[count]);
		i2c_wait_ack();
	}
	i2c_stop();

	return 1;
	
}
*/

uint8_t IICReadByte(uint8_t dev, uint8_t reg, uint8_t *dat)
{
	*dat = I2C_ReadOneByte(dev, reg);
	return 1;

}

uint8_t IICWriteByte(uint8_t dev, uint8_t reg, uint8_t data)
{
	return IICWriteBytes(dev, reg, 1, &data);
}

