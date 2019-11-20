#ifndef __CSR8675_H_
#define __CSR8675_H_

#include "stm32h7xx.h"

#define CSR_DMA_BufferSize 1024

void CSR8675_Init(void);
void CSR8675_StartReceive(uint32_t* Buffer1, uint32_t* Buffer2, uint32_t bs, void (* M1F)(DMA_HandleTypeDef *), void (* M2F)(DMA_HandleTypeDef *));
#endif
