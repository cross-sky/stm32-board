#ifndef  __STM32_UB_EEPROM_H
#define __STM32_UB_EEPROM_H

#include "stm32f10x_conf.h"
#include "stm32f10x.h"

void AT24CXX_WriteBuff(uint8_t dev, uint8_t reg, uint8_t *Buffer, uint16_t Length);

void AT24CXX_ReadBuffer(uint8_t dev, uint8_t reg, uint8_t *Buffer, uint16_t Length);

void AT24CxxTest(void);
uint8_t eepWRCoreParam(void);
void WRStartParam(uint8_t issend);
void TaskSendStartParm(void);
void CheckTimer(uint8_t isWrite);
void eepSaveTimer(void);
uint8_t RecevWartT(uint8_t len, uint8_t Rcdata[]);
void RequestTempe(void);
#endif
