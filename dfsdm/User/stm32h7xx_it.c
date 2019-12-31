/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx.h"
#include "stm32h7xx_it.h"
#include "os.h"


#define OS_USE 1

void NMI_Handler(void){
}

void HardFault_Handler(void){
  while (1);
}

void MemManage_Handler(void){
  while (1);
}

void BusFault_Handler(void){
  while (1);
}

void UsageFault_Handler(void){
  while (1);
}

void SVC_Handler(void){
}

void DebugMon_Handler(void){
}


//DFSDM PDM��˷�
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter1;
extern DMA_HandleTypeDef hdma_dfsdm1_flt0;
extern DMA_HandleTypeDef hdma_dfsdm1_flt1;

extern SAI_HandleTypeDef hsai_BlockA1;
extern DMA_HandleTypeDef hdma_sai1_a;

extern DMA_HandleTypeDef hdma_i2c1_tx;
extern I2C_HandleTypeDef hi2c1;

void I2C1_EV_IRQHandler(void){
  #if OS_USE == 1
  OSIntEnter();
  #endif

  HAL_I2C_EV_IRQHandler(&hi2c1);

  #if OS_USE == 1
  OSIntExit();
  #endif
}

void I2C1_ER_IRQHandler(void){
  #if OS_USE == 1
  OSIntEnter();
  #endif

  HAL_I2C_ER_IRQHandler(&hi2c1);

  #if OS_USE == 1
  OSIntExit();
  #endif
}

void DMA1_Stream0_IRQHandler(void){
  #if OS_USE == 1
  OSIntEnter();
  #endif

  HAL_DMA_IRQHandler(&hdma_i2c1_tx);

  #if OS_USE == 1
  OSIntExit();
  #endif
}

void DMA1_Stream1_IRQHandler(void){
  #if OS_USE == 1
  OSIntEnter();
  #endif


  #if OS_USE == 1
  OSIntExit();
  #endif
}

void DMA1_Stream2_IRQHandler(void){
  #if OS_USE == 1
  OSIntEnter();
  #endif


  #if OS_USE == 1
  OSIntExit();
  #endif
}

void DMA1_Stream4_IRQHandler(void){
  #if OS_USE == 1
  OSIntEnter();
  #endif

  HAL_DMA_IRQHandler(&hdma_dfsdm1_flt0);
	HAL_DMA_IRQHandler(&hdma_sai1_a);

  #if OS_USE == 1
  OSIntExit();
  #endif
}

void DMA1_Stream5_IRQHandler(void){
  #if OS_USE == 1
  OSIntEnter();
  #endif

  HAL_DMA_IRQHandler(&hdma_dfsdm1_flt1);

  #if OS_USE == 1
  OSIntExit();
  #endif

}

void DFSDM1_FLT0_IRQHandler(void){
  #if OS_USE == 1
  OSIntEnter();
  #endif

  HAL_DFSDM_IRQHandler(&hdfsdm1_filter0);

  #if OS_USE == 1
  OSIntExit();
  #endif
}

void DFSDM1_FLT1_IRQHandler(void){
  #if OS_USE == 1
  OSIntEnter();
  #endif

  HAL_DFSDM_IRQHandler(&hdfsdm1_filter1);

  #if OS_USE == 1
  OSIntExit();
  #endif
}

//������Ƶ
extern SAI_HandleTypeDef hsai_BlockB2;
extern DMA_HandleTypeDef hdma_sai2_b;
void SAI2_IRQHandler(void)
{
  #if OS_USE == 1
  OSIntEnter();
  #endif

  HAL_SAI_IRQHandler(&hsai_BlockB2);

  #if OS_USE == 1
  OSIntExit();
  #endif
  
}

void DMA1_Stream3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_sai2_b);
}

//I2S����
void SAI1_IRQHandler(void){
  #if OS_USE == 1
  OSIntEnter();
  #endif

  HAL_SAI_IRQHandler(&hsai_BlockA1);

  #if OS_USE == 1
  OSIntExit();
  #endif
}

void EXTI15_10_IRQHandler(){
  #if OS_USE == 1
  OSIntEnter();
  #endif

  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  
  #if OS_USE == 1
  OSIntEnter();
  #endif
}


//SD��
extern SD_HandleTypeDef uSdHandle;
void SDMMC1_IRQHandler(void){
  #if OS_USE == 1
  OSIntEnter();
  #endif

  HAL_SD_IRQHandler(&uSdHandle);
  
  #if OS_USE == 1
  OSIntEnter();
  #endif
}

void MDMA_IRQHandler(void){
  #if OS_USE == 1
  OSIntEnter();
  #endif

  #if OS_USE == 1
  OSIntEnter();
  #endif

}
