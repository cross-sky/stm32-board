#ifndef __STM32_UB_RELAY_H
#define __STM32_UB_RELAY_H



typedef enum{
	Relay01Compressor=0x00,
	Relay02Valve4way,
	Relay03Pump,
	Relay04ElecHeater,
	Relay05,
	Relay06,
	Relay07,
	Relay08,
	Relay09,
	Relay10Motor,
	Relay11,
	RelayMax
}RelayName_t;

void vRelaySet(RelayName_t relay, State_t state);
void vRelaySetAll(void);
void vRelayClearAll(void);
void vTask4RelayOutProcess(void);
void vRelayInit(void);


#endif

