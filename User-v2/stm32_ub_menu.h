#ifndef __STM32_UB_MENU_H
#define __STM32_UB_MENU_H



//#define BgledOn GPIOA->BSRR = GPIO_Pin_7
//#define BgledOff GPIOA->BRR = GPIO_Pin_7

void MenuSetClock(void);
void MenuOnStatus(void);
void MenuParaInit(void);

void MenuLcd(void);
void MenuSetTime(void);

void TBeepOn(void);
void TBgledOn(void);

void fAddCoreParam(uint8_t id);
void fSubCoreParam(uint8_t id);

void LcdSetWater(uint8_t id);
void fClockOn(uint8_t hour, uint8_t min);

#endif

