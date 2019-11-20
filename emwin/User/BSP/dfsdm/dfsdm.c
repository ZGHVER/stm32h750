#include "dfsdm.h"
#include "./lcd/bsp_lcd.h"

DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
DFSDM_Filter_HandleTypeDef hdfsdm1_filter1;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel0;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel1;
DMA_HandleTypeDef hdma_dfsdm1_flt0;
DMA_HandleTypeDef hdma_dfsdm1_flt1;

#define BufferSize 1000
/*
int32_t F0_B0[BufferSize]__attribute__((section(".ARM.__at_0x24000000")));
int32_t F0_B1[BufferSize];
int32_t F1_B0[BufferSize];
int32_t F1_B1[BufferSize];
*/
static uint32_t DFSDM1_Init = 0;
static uint32_t HAL_RCC_DFSDM1_CLK_ENABLED = 0;

__STATIC_INLINE void Channel0_Filter0_Init(){

  hdfsdm1_filter0.Instance =                         DFSDM1_Filter0;
  hdfsdm1_filter0.Init.RegularParam.Trigger =        DFSDM_FILTER_SW_TRIGGER;
  hdfsdm1_filter0.Init.RegularParam.FastMode =       ENABLE;
  hdfsdm1_filter0.Init.RegularParam.DmaMode =        ENABLE;
  hdfsdm1_filter0.Init.FilterParam.SincOrder =       DFSDM_FILTER_SINC4_ORDER;
  hdfsdm1_filter0.Init.FilterParam.Oversampling =    64;
  hdfsdm1_filter0.Init.FilterParam.IntOversampling = 1;
  HAL_DFSDM_FilterInit(&hdfsdm1_filter0);

  hdfsdm1_channel0.Instance = DFSDM1_Channel0;
  hdfsdm1_channel0.Init.OutputClock.Activation =      ENABLE;
  hdfsdm1_channel0.Init.OutputClock.Selection =       DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  hdfsdm1_channel0.Init.OutputClock.Divider =         20;
  hdfsdm1_channel0.Init.Input.Multiplexer =           DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel0.Init.Input.DataPacking =           DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel0.Init.Input.Pins =                  DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  hdfsdm1_channel0.Init.SerialInterface.Type =        DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel0.Init.SerialInterface.SpiClock =    DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel0.Init.Awd.FilterOrder =             DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel0.Init.Awd.Oversampling =            1;
  hdfsdm1_channel0.Init.Offset =                      0;
  hdfsdm1_channel0.Init.RightBitShift =               0x02;
  HAL_DFSDM_ChannelInit(&hdfsdm1_channel0);
  HAL_DFSDM_FilterConfigRegChannel(&hdfsdm1_filter0, DFSDM_CHANNEL_0, DFSDM_CONTINUOUS_CONV_ON);

}

__STATIC_INLINE void Channel1_Filter1_Init(){

  hdfsdm1_filter1.Instance =                         DFSDM1_Filter1;
  hdfsdm1_filter1.Init.RegularParam.Trigger =        DFSDM_FILTER_SW_TRIGGER;
  hdfsdm1_filter1.Init.RegularParam.FastMode =       ENABLE;
  hdfsdm1_filter1.Init.RegularParam.DmaMode =        ENABLE;
  hdfsdm1_filter1.Init.FilterParam.SincOrder =       DFSDM_FILTER_SINC4_ORDER;
  hdfsdm1_filter1.Init.FilterParam.Oversampling =    64;
  hdfsdm1_filter1.Init.FilterParam.IntOversampling = 1;
  HAL_DFSDM_FilterInit(&hdfsdm1_filter1);

  hdfsdm1_channel1.Instance = DFSDM1_Channel1;
  hdfsdm1_channel1.Init.OutputClock.Activation =     ENABLE;
  hdfsdm1_channel1.Init.OutputClock.Selection =      DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  hdfsdm1_channel1.Init.OutputClock.Divider =        20;
  hdfsdm1_channel1.Init.Input.Multiplexer =          DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel1.Init.Input.DataPacking =          DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel1.Init.Input.Pins =                 DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  hdfsdm1_channel1.Init.SerialInterface.Type =       DFSDM_CHANNEL_SPI_FALLING;
  hdfsdm1_channel1.Init.SerialInterface.SpiClock =   DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel1.Init.Awd.FilterOrder =            DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel1.Init.Awd.Oversampling =           1;
  hdfsdm1_channel1.Init.Offset =                     0;
  hdfsdm1_channel1.Init.RightBitShift =              0x02;
  HAL_DFSDM_ChannelInit(&hdfsdm1_channel1);
  HAL_DFSDM_FilterConfigRegChannel(&hdfsdm1_filter1, DFSDM_CHANNEL_1, DFSDM_CONTINUOUS_CONV_ON);

}

void DFSDM_Init(){

  __HAL_RCC_DMA1_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

  Channel0_Filter0_Init();
  Channel1_Filter1_Init();
  /*
  HAL_DMA_RegisterCallback(&hdma_dfsdm1_flt0, HAL_DMA_XFER_CPLT_CB_ID, cab);
	HAL_DMA_RegisterCallback(&hdma_dfsdm1_flt0, HAL_DMA_XFER_M1CPLT_CB_ID, cac);
	HAL_DMA_RegisterCallback(&hdma_dfsdm1_flt1, HAL_DMA_XFER_M1CPLT_CB_ID, cad);
	HAL_DMAEx_MultiBufferStart_IT(&hdma_dfsdm1_flt0, (uint32_t)&hdfsdm1_filter0.Instance->FLTRDATAR,
         (uint32_t)&dd[0], (uint32_t)&dd1[0], 500);       
	HAL_DMAEx_MultiBufferStart_IT(&hdma_dfsdm1_flt1, (uint32_t)&hdfsdm1_filter1.Instance->FLTRDATAR,
         (uint32_t)&dd2[0], (uint32_t)&dd3[0], 500); 
  HAL_DFSDM_FilterRegularStart(&hdfsdm1_filter0);
  HAL_DFSDM_FilterRegularStart(&hdfsdm1_filter1);*/
  /*HAL_DMAEx_MultiBufferStart_IT(&hdma_dfsdm1_flt0, 
                                (uint32_t)&hdfsdm1_filter0.Instance->FLTRDATAR,
                                (uint32_t)&F0_B0[0], 
                                (uint32_t)&F0_B1[0], 
                                BufferSize
                                );

	HAL_DMAEx_MultiBufferStart_IT(&hdma_dfsdm1_flt1, 
                                (uint32_t)&hdfsdm1_filter1.Instance->FLTRDATAR,
                                (uint32_t)&F1_B0[0], 
                                (uint32_t)&F1_B1[0], 
                                BufferSize
                                );  */                            
}

void DFSDM_StartConv(){
}

__STATIC_INLINE void Filter0_DMAInit(){
  
  hdma_dfsdm1_flt0.Instance =                 DMA1_Stream4;
  hdma_dfsdm1_flt0.Init.Request =             DMA_REQUEST_DFSDM1_FLT0;
  hdma_dfsdm1_flt0.Init.Direction =           DMA_PERIPH_TO_MEMORY;
  hdma_dfsdm1_flt0.Init.PeriphInc =           DMA_PINC_DISABLE;
  hdma_dfsdm1_flt0.Init.MemInc =              DMA_MINC_ENABLE;
  hdma_dfsdm1_flt0.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_dfsdm1_flt0.Init.MemDataAlignment =    DMA_MDATAALIGN_WORD;
  hdma_dfsdm1_flt0.Init.Mode =                DMA_CIRCULAR;
  hdma_dfsdm1_flt0.Init.Priority =            DMA_PRIORITY_LOW;
  hdma_dfsdm1_flt0.Init.FIFOMode =            DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_dfsdm1_flt0);
  __HAL_LINKDMA(&hdfsdm1_filter0,hdmaReg,hdma_dfsdm1_flt0);
  __HAL_LINKDMA(&hdfsdm1_filter0,hdmaInj,hdma_dfsdm1_flt0);
}

__STATIC_INLINE void Filter1_DMAInit(){

  hdma_dfsdm1_flt1.Instance =                 DMA1_Stream5;
  hdma_dfsdm1_flt1.Init.Request =             DMA_REQUEST_DFSDM1_FLT1;
  hdma_dfsdm1_flt1.Init.Direction =           DMA_PERIPH_TO_MEMORY;
  hdma_dfsdm1_flt1.Init.PeriphInc =           DMA_PINC_DISABLE;
  hdma_dfsdm1_flt1.Init.MemInc =              DMA_MINC_ENABLE;
  hdma_dfsdm1_flt1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_dfsdm1_flt1.Init.MemDataAlignment =    DMA_MDATAALIGN_WORD;
  hdma_dfsdm1_flt1.Init.Mode =                DMA_CIRCULAR;
  hdma_dfsdm1_flt1.Init.Priority =            DMA_PRIORITY_LOW;
  hdma_dfsdm1_flt1.Init.FIFOMode =            DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_dfsdm1_flt1);
  __HAL_LINKDMA(&hdfsdm1_filter1,hdmaInj,hdma_dfsdm1_flt1);
  __HAL_LINKDMA(&hdfsdm1_filter1,hdmaReg,hdma_dfsdm1_flt1);

}

void HAL_DFSDM_FilterMspInit(DFSDM_Filter_HandleTypeDef* hdfsdm_filter){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(DFSDM1_Init == 0)
  {
    HAL_RCC_DFSDM1_CLK_ENABLED++;
    if(HAL_RCC_DFSDM1_CLK_ENABLED==1){
      __HAL_RCC_DFSDM1_CLK_ENABLE();
    }
  
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_DFSDM1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_DFSDM1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(DFSDM1_FLT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DFSDM1_FLT0_IRQn);
    HAL_NVIC_SetPriority(DFSDM1_FLT1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DFSDM1_FLT1_IRQn);

  }
  
  if(hdfsdm_filter->Instance == DFSDM1_Filter0){
    Filter0_DMAInit();
  }

   if(hdfsdm_filter->Instance == DFSDM1_Filter1){
    Filter1_DMAInit();
  }

}

void HAL_DFSDM_ChannelMspInit(DFSDM_Channel_HandleTypeDef* hdfsdm_channel){
   GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(DFSDM1_Init == 0)
  {
    HAL_RCC_DFSDM1_CLK_ENABLED++;
    if(HAL_RCC_DFSDM1_CLK_ENABLED==1){
      __HAL_RCC_DFSDM1_CLK_ENABLE();
    }
  
    __HAL_RCC_GPIOD_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_DFSDM1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_DFSDM1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  }
}
