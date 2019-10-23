#ifndef __APP__H_
#define __APP__H_

#include "os.h"
#include "stm32h7xx_hal_conf.h"
#include "stm32h7xx.h"
#include "./BSP/sdram/bsp_sdram.h"
#include "./BSP/lcd/bsp_lcd.h"
#include "./BSP/sdram/bsp_sdram.h"

#define osError_hander(err) ((err) == OS_ERR_NONE ? (void)0 : ERR_H((uint8_t *)__FILE__, __LINE__, (OS_ERR)err))

void ERR_H(uint8_t* file, uint32_t line, OS_ERR err);
void SystemClock_Config(void);
void MEM_Init(void);
void LCD_Start(void);

#endif

