#include "CSR8675.h"
DMA_HandleTypeDef hdma_sai2_b;
SAI_HandleTypeDef hsai_BlockB2;


static void MX_SAI2_Init(void);
__STATIC_INLINE void HAL_SAI2B_MspInit();

void CSR8675_Init(){
   __HAL_RCC_DMA1_CLK_ENABLE();
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  MX_SAI2_Init();
}

void CSR8675_StartReceive(uint32_t* Buffer1, uint32_t* Buffer2, uint32_t bs, void (* M1F)(DMA_HandleTypeDef *), void (* M2F)(DMA_HandleTypeDef *)){
    HAL_DMA_RegisterCallback(&hdma_sai2_b, HAL_DMA_XFER_CPLT_CB_ID, M1F);
    HAL_DMA_RegisterCallback(&hdma_sai2_b, HAL_DMA_XFER_M1CPLT_CB_ID, M2F);
    HAL_DMAEx_MultiBufferStart_IT(&hdma_sai2_b, 
                                 (uint32_t)&hsai_BlockB2.Instance->DR, 
                                 (uint32_t)Buffer1, 
                                 (uint32_t)Buffer2, 
                                 bs
                                 );
    __HAL_SAI_ENABLE(&hsai_BlockB2);
    hsai_BlockB2.Instance->CR1 |= SAI_xCR1_DMAEN;
}

static void MX_SAI2_Init(void){
  hsai_BlockB2.Instance =             SAI2_Block_B;
  hsai_BlockB2.Init.AudioMode =       SAI_MODESLAVE_RX;
  hsai_BlockB2.Init.Synchro =         SAI_ASYNCHRONOUS;
  hsai_BlockB2.Init.OutputDrive =     SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockB2.Init.FIFOThreshold =   SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockB2.Init.SynchroExt =      SAI_SYNCEXT_DISABLE;
  hsai_BlockB2.Init.MonoStereoMode =  SAI_STEREOMODE;
  hsai_BlockB2.Init.CompandingMode =  SAI_NOCOMPANDING;
  hsai_BlockB2.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol(&hsai_BlockB2, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_32BIT, 2) != HAL_OK)
  {
  }
  HAL_SAI2B_MspInit();
	HAL_SAI_Init(&hsai_BlockB2);
}

__STATIC_INLINE void HAL_SAI2B_MspInit(){
  GPIO_InitTypeDef GPIO_InitStruct;
    /* SAI2 */
     __HAL_RCC_SAI2_CLK_ENABLE();
    HAL_NVIC_SetPriority(SAI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SAI2_IRQn);
    /**SAI2_B_Block_B GPIO Configuration    
    PG9     ------> SAI2_FS_B
    PA2     ------> SAI2_SCK_B
    PA0     ------> SAI2_SD_B 
    */
    __GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_SAI2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_SAI2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF10_SAI2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral DMA init*/
    
    hdma_sai2_b.Instance = DMA1_Stream3;
    hdma_sai2_b.Init.Request = DMA_REQUEST_SAI2_B;
    hdma_sai2_b.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sai2_b.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sai2_b.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sai2_b.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sai2_b.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sai2_b.Init.Mode = DMA_CIRCULAR;
    hdma_sai2_b.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_sai2_b.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_sai2_b);

    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one channel to perform all the requested DMAs. */
    __HAL_LINKDMA(&hsai_BlockB2,hdmarx,hdma_sai2_b);
    __HAL_LINKDMA(&hsai_BlockB2,hdmatx,hdma_sai2_b);
}

