#include"./INMP441/INMP441.h"
I2S_HandleTypeDef hi2s1;
DMA_HandleTypeDef hdma_spi1_rx;
uint16_t d1[100];
uint16_t d2[100];

void callb(DMA_HandleTypeDef* hdma);
__STATIC_INLINE void I_I2S_Init(){
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**I2S1 GPIO Configuration    
    PA6     ------> I2S1_SDI
    PA7     ------> I2S1_SDO
    PA5     ------> I2S1_CK
    PA4     ------> I2S1_WS 
    */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    hi2s1.Instance =                     SPI1;
    hi2s1.Init.Mode =                    I2S_MODE_MASTER_RX;
    hi2s1.Init.Standard =                I2S_STANDARD_PHILIPS;
    hi2s1.Init.DataFormat =              I2S_DATAFORMAT_24B;
    hi2s1.Init.MCLKOutput =              I2S_MCLKOUTPUT_DISABLE;
    hi2s1.Init.AudioFreq =               I2S_AUDIOFREQ_44K;
    hi2s1.Init.CPOL =                    I2S_CPOL_LOW;
    hi2s1.Init.FirstBit =                I2S_FIRSTBIT_MSB;
    hi2s1.Init.WSInversion =             I2S_WS_INVERSION_DISABLE;
    hi2s1.Init.IOSwap =                  I2S_IO_SWAP_DISABLE;
    hi2s1.Init.Data24BitAlignment =      I2S_DATA_24BIT_ALIGNMENT_RIGHT;
    hi2s1.Init.FifoThreshold =           I2S_FIFO_THRESHOLD_01DATA;
    hi2s1.Init.MasterKeepIOState =       I2S_MASTER_KEEP_IO_STATE_DISABLE;
    hi2s1.Init.SlaveExtendFREDetection = I2S_SLAVE_EXTEND_FRE_DETECTION_DISABLE;
    if (HAL_I2S_Init(&hi2s1) != HAL_OK){    
      osError_hander(12);
    }

    hdma_spi1_rx.Instance =                 DMA1_Stream0;
    hdma_spi1_rx.Init.Request =             DMA_REQUEST_SPI1_RX;
    hdma_spi1_rx.Init.Direction =           DMA_PERIPH_TO_MEMORY;
    hdma_spi1_rx.Init.PeriphInc =           DMA_PINC_DISABLE;
    hdma_spi1_rx.Init.MemInc =              DMA_MINC_ENABLE;
    hdma_spi1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_spi1_rx.Init.MemDataAlignment =    DMA_MDATAALIGN_HALFWORD;
    hdma_spi1_rx.Init.Mode =                DMA_CIRCULAR;
    hdma_spi1_rx.Init.Priority =            DMA_PRIORITY_LOW;
    hdma_spi1_rx.Init.FIFOMode =            DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi1_rx) != HAL_OK){
      osError_hander(12);
    }
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 2, 0);
    HAL_DMA_RegisterCallback(&hdma_spi1_rx, HAL_DMA_XFER_CPLT_CB_ID, callb);
    __HAL_LINKDMA(&hi2s1, hdmarx, hdma_spi1_rx);
    HAL_DMAEx_MultiBufferStart_IT(  &hdma_spi1_rx, 
                                    (uint32_t)&hi2s1.Instance->RXDR, 
                                    (uint32_t)&d1[0], (uint32_t)&d2[0], 
                                    100);
}

void INMP441_Init(){
    I_I2S_Init();
}

void DMA1_Stream0_IRQHandler(){
    HAL_DMA_IRQHandler(&hdma_spi1_rx);
}

void callb(DMA_HandleTypeDef* hdma){

}
