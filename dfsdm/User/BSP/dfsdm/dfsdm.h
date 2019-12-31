/*
 * @Author: your name
 * @Date: 2019-10-27 13:08:29
 * @LastEditTime : 2019-12-29 21:16:00
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \dfsdm\User\BSP\dfsdm\dfsdm.h
 */
#ifndef __DFSDM_H_
#define __DFSDM_H_

#include "stm32h7xx.h"

void DFSDM_Init(void);
void HAL_DFSDM_FilterMspInit(DFSDM_Filter_HandleTypeDef* hdfsdm_filter);
void HAL_DFSDM_ChannelMspInit(DFSDM_Channel_HandleTypeDef* hdfsdm_channel);
void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter);
void DFSDM_StartConv(uint32_t* F0B0,
                     uint32_t* F0B1,
                     uint32_t* F1B0,
                     uint32_t* F1B1,
                     uint32_t  BufferLength,
                     void (* F1F)(DMA_HandleTypeDef *), 
                     void (* F2F)(DMA_HandleTypeDef *));
#endif
