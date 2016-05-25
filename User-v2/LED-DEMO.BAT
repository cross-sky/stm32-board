SET PATH=E:\Program Files\keil\ARM\BIN40;E:\Python34\Lib\site-packages\PyQt4;E:\Python34\;E:\Python34\Scripts;c:\windows;c:\windows\system32;C:\Program Files\TortoiseGit\bin;e:\CooCox\CoIDE\gcc\bin;e:\Program Files\doxygen\bin;E:\Python34;C:\Program Files (x86)\GtkSharp\2.12\bin;C:\Program Files\Common Files\Autodesk Shared\;E:\Program Files (x86)\QuickTime\QTSystem\;C:\Program Files (x86)\Microsoft SQL Server\90\Tools\binn\;e:\Cocos\frameworks\cocos2d-x\tools\cocos2d-console\bin;F:\opencv\build\x64\vc10\bin;F:\opencv\tbb40\bin\intel64\vc10;e:\Cocos\tools\Python27;%JAVA_HOME%\jre\bin;%JAVA_HOME%\bin
SET ARMCC41INC=E:\Program Files\keil\ARM\RV31\Inc
SET ARMCC41LIB=E:\Program Files\keil\ARM\RV31\Lib
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=LED-DEMO
SET CPU_CLOCK=0x007A1200
"E:\Program Files\keil\ARM\BIN40\ArmAsm" --Via "..\output\startup_stm32f10x_md._ia"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\main.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32_ub_led.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32f10x_it.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\task.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\tsystick.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32_ub_button.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32_ub_eeprom.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32_ub_iic.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32_ub_pcf8563.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32_ub_lcd.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32_ub_menu.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32_ub_params.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32_ub_offstatus.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32_ub_dma_usart1.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32f10x_rcc.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32f10x_gpio.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\misc.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32f10x_exti.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32f10x_adc.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32f10x_dma.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32f10x_usart.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32f10x_i2c.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32f10x_spi.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\stm32f10x_tim.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\core_cm3.__i"
"E:\Program Files\keil\ARM\BIN40\ArmCC" --Via "..\output\system_stm32f10x.__i"
"E:\Program Files\keil\ARM\BIN40\ArmLink" --Via "..\Output\STM32-DEMO.lnp"
E:\Program Files\keil\ARM\BIN40\fromelf.exe "..\Output\STM32-DEMO.axf" --i32combined --output "..\Output\STM32-DEMO.hex"
