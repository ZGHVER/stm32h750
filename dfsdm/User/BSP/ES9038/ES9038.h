#ifndef __ES9038_H_
#define __ES9038_H_

#include "stm32h7xx.h"
void ES9038_Init(void);
void ES9038_StartPlay(uint32_t* Buffer1, uint32_t* Buffer2, uint32_t Bs, void (* M1F)(DMA_HandleTypeDef *), void (* M2F)(DMA_HandleTypeDef *));
void ES9038_StopPlay(void);
void ES9038_Continue(void);
#endif
