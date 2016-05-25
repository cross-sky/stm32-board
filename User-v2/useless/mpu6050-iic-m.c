#include "cominc.h"

int16_t mympu[7];
float tTemp[7];

char  test = 0; 				 //IIC用到 

static i2c1_gpio_config(void)
{
	GPIO_InitTypeDef gpio_init_structure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

	gpio_init_structure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5;
	gpio_init_structure.GPIO_Speed = GPIO_Speed_10MHz;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_OD;

	GPIO_Init(GPIOB,&gpio_init_structure);

}

static void i2c1_delay( void)
{
	u8 i=5;
	while ( i-- );
}

static void delay5ms(void)
{
	u16 i=10000;
	while (i--);
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
	SDA_L;
	i2c1_delay();
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
	SCL_L;
	i2c1_delay();
	SDA_H;
	i2c1_delay();
	SCL_H;
	i2c1_delay();
	if( SDA_READ )
	{
		SCL_L;
		i2c1_delay();
		return FALSE;
	}
	SCL_L;
	i2c1_delay();
	return TRUE;

}

void i2c_send_byte(u8 send_byte)
{
	u8 i=8;
	while ( i-- )
	{
		SCL_L;
		i2c1_delay();
		if(send_byte&0x80)
			SDA_H;
		else
			SDA_L;
		send_byte <<=1;
		i2c1_delay();
		SCL_H;
		i2c1_delay();

	}
	SCL_L;
}

u8 i2c_read_byte(void)
{
	u8 i=8;
	u8 receive_data = 0;

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
	return receive_data;
}

u8 i2c_read_byte_ack( u8 ack)
{
	u8 i=8;
	u8 receive_data = 0;

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

bool single_write(u8 slave_address, u8 reg_address, u8 reg_data)
{
	if( !i2c_start() ) return FALSE;

	i2c_send_byte( slave_address );
	if( !i2c_wait_ack() )
	{
		i2c_stop();
		return FALSE;

	}

	i2c_send_byte(reg_address);
	i2c_wait_ack();
	i2c_send_byte(reg_data);
	i2c_wait_ack();
	i2c_stop();
	//delay5ms();
	return TRUE;


}


u8 single_read(u8 slave_address, u8 reg_address)
{
	 u8 reg_data;
	 if(!i2c_start() ) return FALSE;
	 i2c_send_byte( slave_address );
	 if ( !i2c_wait_ack() )
	 {
		 i2c_stop();
		 test = 1;
		 return FALSE;
	 }
	 i2c_send_byte(reg_address);
	 i2c_wait_ack();
	 i2c_start();
	 i2c_send_byte( slave_address + 1 );
	 i2c_wait_ack();

	 reg_data = i2c_read_byte();
	 i2c_no_ack();
	 i2c_stop();

	 return reg_data;
}

bool mpu_read_xyz(u8 slave_address)
{
	u8 buf[14],i=0;
	
	if( !i2c_start() ) return FALSE;
	 i2c_send_byte( slave_address );
	 if ( !i2c_wait_ack() )
	 {
		 i2c_stop();
		 test = 1;
		 return FALSE;
	 }
	
	i2c_send_byte( 0x3b );
	i2c_wait_ack();

	i2c_start();
	i2c_send_byte( slave_address + 1 );
	i2c_wait_ack();

	for (i=0; i<14; i++ )
	{
		if( i == 13 ) buf[i] = i2c_read_byte_ack(0);
		else
			buf[i] = i2c_read_byte_ack(1);
	}

	i2c_stop();
	for (i=0; i<7; i++ )
	{
		mympu[i] = (int16_t)((u16)buf[2*i]<<8 | buf[2*i + 1]);  //加和与弄错了;

	}
	
//	Temperature = 35+ ((double) (Temperature + 13200)) / 280
  return TRUE;
//	i2c_stop();

}

void MpuRead(void)
{
		u8 i;
	mpu_read_xyz(0xd0);
		for(i=0;i<3;i++)
	{
		tTemp[i] = (float)mympu[i]/16384;
	}
	
		for(i=4;i<7;i++)
	{
		tTemp[i] = (float)mympu[i]/16.4;
	}
	
   tTemp[3] = 36.53+ (float)mympu[3] / 340;
// 		for(i=0;i<7;i++)
// 	printf("%d= %.2f,", i,tTemp[i]);
	
}

void mpu6050m_init(void )
{
	i2c1_gpio_config();
	delay5ms();
	
	single_write(MPU6050_Addr,PWR_MGMT_1,0x00);
	single_write(MPU6050_Addr,SMPLRT_DIV,0x07);
	single_write(MPU6050_Addr,CONFIG,0x06);
	single_write(MPU6050_Addr,GYRO_CONFIG,0x18);
	single_write(MPU6050_Addr,ACCEL_CONFIG,0x01);
}

void MpuDisPlay(void)
{

}
