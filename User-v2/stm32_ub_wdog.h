#ifndef __STM32_UB_IWDG_H
#define __STM32_UB_IWDG_H

void IWDG_Init(uint8_t prer, uint8_t rlr);
void IWDG_Feed(void);
#endif