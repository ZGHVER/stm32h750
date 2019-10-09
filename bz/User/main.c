
#include "main.h"
#include "./BSP/INMP441/INMP441.h"
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
int main(void){
  SystemClock_Config();	
  HAL_Init();
 uint8_t ss[32] = {
    0x01, 0x02, 0x04, 0x08,
    0x00, 0x00, 0x00, 0x01,
    0x01, 0x02, 0x04, 0x08,
    0x00, 0x00, 0x00, 0x02,
    0x10, 0x20, 0x40, 0x80,
    0x00, 0x00, 0x00, 0x03,
    0xaa, 0xaa, 0xaa, 0xaa,
    0x00, 0x00, 0x00, 0x04,
  };

  __HAL_RCC_USART1_CLK_ENABLE();
  __DMA1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  hdma_usart1_tx.Instance = DMA1_Stream1;
  hdma_usart1_tx.Init.Request = DMA_REQUEST_USART1_TX;
  hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart1_tx.Init.Mode = DMA_CIRCULAR;
  hdma_usart1_tx.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_DeInit(&hdma_usart1_tx);
  HAL_DMA_Init(&hdma_usart1_tx);

  __HAL_LINKDMA(&huart1,hdmatx,hdma_usart1_tx);

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.Prescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart1);

	HAL_UART_Transmit_DMA(&huart1, (uint8_t*)ss, 32);
  
	while(1){
  }
}

