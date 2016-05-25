

void IWDG_Init(uint8_t prer, uint8_t rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); /* 0x5555使能对寄存器IWDG_PR和IWDG_RLR的写操作*/
	IWDG_SetPrescaler(prer);    /*设置IWDG预分频值:设置IWDG预分频值*/
	IWDG_SetReload(rlr);     /*设置IWDG重装载值*/
	IWDG_ReloadCounter();    /*按照IWDG重装载寄存器的值重装载IWDG计数器*/
	IWDG_Enable();        /*0xcccc, 使能IWDG*/
}

/**
 * 喂独立看门狗
 */
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();    /*0xaaaa reload*/
}
