#include "ES9038.h"
#include "arm_math.h"
#include "./OLED/oled.h"

SAI_HandleTypeDef hsai_BlockA1;
DMA_HandleTypeDef hdma_sai1_a;

void MX_SAI4_Init(void);
void HAL_SAI1A_MspInit();

void ES9038_StartPlay(uint32_t* Buffer1, uint32_t* Buffer2, uint32_t bs, void (* M1F)(DMA_HandleTypeDef *), void (* M2F)(DMA_HandleTypeDef *)){
    HAL_DMA_RegisterCallback(&hdma_sai1_a, HAL_DMA_XFER_CPLT_CB_ID, M1F);
    HAL_DMA_RegisterCallback(&hdma_sai1_a, HAL_DMA_XFER_M1CPLT_CB_ID, M2F);
    HAL_DMAEx_MultiBufferStart_IT(&hdma_sai1_a,
                                 (uint32_t)Buffer1, 
                                 (uint32_t)&hsai_BlockA1.Instance->DR, 
                                 (uint32_t)Buffer2, 
                                 bs
                                 );
    __HAL_SAI_ENABLE(&hsai_BlockA1);
    hsai_BlockA1.Instance->CR1 |= SAI_xCR1_DMAEN;
}

void ES9038_StopPlay(){
  hsai_BlockA1.Instance->CR1 &= ~SAI_xCR1_DMAEN;
  __HAL_SAI_DISABLE(&hsai_BlockA1);
  __HAL_DMA_DISABLE(hsai_BlockA1.hdmatx);
}

void ES9038_Continue(){
  __HAL_DMA_ENABLE(hsai_BlockA1.hdmatx);
  __HAL_SAI_ENABLE(&hsai_BlockA1);
  hsai_BlockA1.Instance->CR1 |= SAI_xCR1_DMAEN;
}

static void MX_SAI1_Init(void){
  hsai_BlockA1.Instance = SAI1_Block_A;
  hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_TX;
  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_48K;
  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol(&hsai_BlockA1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_32BIT, 2) != HAL_OK)
  {
    while(1);
  }
  HAL_SAI1A_MspInit();
  HAL_SAI_Init(&hsai_BlockA1);

}

void HAL_SAI1A_MspInit(){
  GPIO_InitTypeDef GPIO_InitStruct;
  /* SAI1 */
    __HAL_RCC_SAI1_CLK_ENABLE();
    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(SAI1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(SAI1_IRQn);
    /**SAI1_A_Block_A GPIO Configuration    
    PE5     ------> SAI1_SCK_A
    PE4     ------> SAI1_FS_A
    PC1     ------> SAI1_SD_A 
    */
    __GPIOE_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    __GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Peripheral DMA init*/
    
    hdma_sai1_a.Instance = DMA1_Stream4;
    hdma_sai1_a.Init.Request = DMA_REQUEST_SAI1_A;
    hdma_sai1_a.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sai1_a.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sai1_a.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sai1_a.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sai1_a.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sai1_a.Init.Mode = DMA_CIRCULAR;
    hdma_sai1_a.Init.Priority = DMA_PRIORITY_LOW;
    hdma_sai1_a.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_sai1_a) != HAL_OK)
    {
      while(1);
    }
    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one channel to perform all the requested DMAs. */
    __HAL_LINKDMA(&hsai_BlockA1,hdmarx,hdma_sai1_a);
    __HAL_LINKDMA(&hsai_BlockA1,hdmatx,hdma_sai1_a);
}

void ES9038_Init(){
  __HAL_RCC_DMA1_CLK_ENABLE();
  /* DMA interrupt init */
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
/*
  __GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef GPIOB_InitType;
  GPIOB_InitType.Pin = GPIO_PIN_11;
  GPIOB_InitType.Mode = GPIO_MODE_IT_FALLING;
  GPIOB_InitType.Pull = GPIO_NOPULL;
  GPIOB_InitType.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIOB_InitType);

  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 4, 0);
*/
  MX_SAI1_Init();
  
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if(GPIO_Pin == GPIO_PIN_11){
    OLED_ShowString(10, 10, "123", 12);
    OLED_Refresh_Gram();
  }
}
