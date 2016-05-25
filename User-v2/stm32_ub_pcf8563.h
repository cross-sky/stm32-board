#ifndef __STM32_PCF8563_H
#define __STM32_PCF8563_H

#include "stm32f10x_conf.h"
#include "stm32f10x.h"

void PCF8563Init(void);
void Pcf8536RT(void);
void Pcf8563SetTime(const uint8_t pcfst[3]);

#endif
