#ifndef __MPU6050M_H
#define __MPU6050M_H

#include "stm32f10x.h"

//#define FALSE 0
//#define TRUE 1

#define SDA_H GPIOB->BSRR = GPIO_Pin_5
#define SDA_L GPIOB->BRR = GPIO_Pin_5

#define SCL_H GPIOB->BSRR = GPIO_Pin_6
#define SCL_L GPIOB->BRR = GPIO_Pin_6

#define SDA_READ (GPIOB->IDR & GPIO_Pin_5)

// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz);
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz);
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s);
#define	ACCEL_CONFIG	0x1C	//加速计自检测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz);

#define INT_PIN_CFG     0x37    //设置旁路有效 打开值：0x42 AUX_DA的辅助I2C;
#define USER_CTRL       0x6A    //用户配置寄存器 打开值：0x40  AUX_DA的辅助I2C;

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用);
//#define	WHO_AM_I		0x75	//IIC地址寄存器(默认读出数值0x68，只读);
//IIC写入时的地址字节数据，+1为读取;
#define	MPU6050_Addr   0xD0	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改;

typedef enum {FALSE = 0, TRUE = 1} bool;

extern char  test; 				 //IIC用到
extern float tTemp[7];
extern bool mpu_read_xyz(u8 slave_address);
extern void mpu6050m_init(void );
//extern void i2c1_gpio_config(void);
extern u8 single_read(u8 slave_address, u8 reg_address);
void MpuRead(void);
#endif



