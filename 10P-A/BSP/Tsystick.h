#ifndef __TSYSTICK_H
#define __TSYSTICK_H

#define TestEx 1
extern u8 G_TestExcut;

void SystickInit( void );
//void TestExcuteTimeGpioConfig(void);
//void TestExcuteTime(u8 state);
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);
#endif

/*
#if TestEx
	TestExcuteTime(1);
#endif

#if TestEx
	TestExcuteTime(0);
#endif
*/

