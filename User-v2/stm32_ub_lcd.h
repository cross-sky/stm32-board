#ifndef __STM32_UB_LCD_H
#define __STM32_UB_LCD_H

#include "stm32f10x_conf.h"
#include "stm32f10x.h"

static void lcd_delay(void);

#define LCD_CS_H {GPIOB->BSRR = GPIO_Pin_5; lcd_delay();}
#define LCD_CS_L {GPIOB->BRR = GPIO_Pin_5; lcd_delay();}
#define LCD_WR_H {GPIOB->BSRR = GPIO_Pin_4; lcd_delay();}
#define LCD_WR_L {GPIOB->BRR = GPIO_Pin_4; lcd_delay();}
#define LCD_DA_H {GPIOB->BSRR = GPIO_Pin_3; lcd_delay();}
#define LCD_DA_L {GPIOB->BRR = GPIO_Pin_3; lcd_delay();}

#define LCD_LED_ON GPIOA->BSRR = GPIO_Pin_11
#define LCD_LED_OFF GPIOA->BRR = GPIO_Pin_11// bgled

#define BeepOn GPIOA->BSRR = GPIO_Pin_6// buzz
#define BeepOff GPIOA->BRR = GPIO_Pin_6

void Lcd1621Init(void);
void lcd_wr_num(uint8_t number, uint8_t position, uint8_t display);
void lcd_clear(void);
void lcd_flood_data(void);
void lcd1621test(void);
void lcd_wr_char(uint8_t name, uint8_t display);
#endif
