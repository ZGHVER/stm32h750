
#ifndef _APP_H_
#define _APP_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "stm32h7xx.h"

void LCD_Start(void);
void SystemClock_Config(void);
void ALL_Init(void);

#endif
