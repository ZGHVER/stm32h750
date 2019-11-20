/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : stm32h7xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */
 
/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

extern DMA_HandleTypeDef hdma_sai4_a;

static uint32_t SAI4_client =0;

void HAL_SAI_MspInit(SAI_HandleTypeDef* hsai)
{

  GPIO_InitTypeDef GPIO_InitStruct;
/* SAI4 */
    if(hsai->Instance==SAI4_Block_A)
    {
    /* Peripheral clock enable */
    if (SAI4_client == 0)
    {
       __HAL_RCC_SAI4_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(SAI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SAI4_IRQn);
    }
    SAI4_client ++;
    
    /**SAI4_A_Block_A GPIO Configuration    
    PE5     ------> SAI4_SCK_A
    PE4     ------> SAI4_FS_A
    PE6     ------> SAI4_SD_A 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_SAI4;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* Peripheral DMA init*/
    
    hdma_sai4_a.Instance = BDMA_Channel0;
    hdma_sai4_a.Init.Request = BDMA_REQUEST_SAI4_A;
    hdma_sai4_a.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sai4_a.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sai4_a.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sai4_a.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_sai4_a.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_sai4_a.Init.Mode = DMA_CIRCULAR;
    hdma_sai4_a.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_sai4_a) != HAL_OK)
    {
      Error_Handler();
    }

    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one channel to perform all the requested DMAs. */
    __HAL_LINKDMA(hsai,hdmarx,hdma_sai4_a);

    __HAL_LINKDMA(hsai,hdmatx,hdma_sai4_a);

    }
}

void HAL_SAI_MspDeInit(SAI_HandleTypeDef* hsai)
{
/* SAI4 */
    if(hsai->Instance==SAI4_Block_A)
    {
    SAI4_client --;
    if (SAI4_client == 0)
      {
      /* Peripheral clock disable */ 
       __HAL_RCC_SAI4_CLK_DISABLE();
      HAL_NVIC_DisableIRQ(SAI4_IRQn);
      }
    
    /**SAI4_A_Block_A GPIO Configuration    
    PE5     ------> SAI4_SCK_A
    PE4     ------> SAI4_FS_A
    PE6     ------> SAI4_SD_A 
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_6);

    HAL_DMA_DeInit(hsai->hdmarx);
    HAL_DMA_DeInit(hsai->hdmatx);
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
