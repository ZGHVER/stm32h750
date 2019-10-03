
#include <stdio.h>
#include <bsp.h>
#include <app_cfg.h>
#include <os_app_hooks.h>
#include <stm32h7xx_hal.h>
#include "lcd/bsp_lcd.h"
#include "./fonts/fonts.h"
#include "./mpu/bsp_mpu.h"
#include "./JDY40/JDY40.h"

UART_HandleTypeDef huart5;
UART_HandleTypeDef* Phuart5;

UART_HandleTypeDef huart1;
UART_HandleTypeDef* Phuart1;


void delay(uint32_t i){
    while(i--);
}


void uart5();
void uart1();
void SystemClock_Config(void);
int main(void){
    SystemClock_Config();
    HAL_Init();
    OS_CPU_SysTickInitFreq(400);
	uart1();
	uart5();

    __GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIOB_InitType;
    GPIOB_InitType.Pin = GPIO_PIN_11;
    GPIOB_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOB_InitType.Pull = GPIO_PULLUP;
    GPIOB_InitType.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIOB_InitType);

    __GPIOG_CLK_ENABLE();
    GPIO_InitTypeDef GPIOG_InitType;
    GPIOG_InitType.Pin = GPIO_PIN_13;
    GPIOG_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOG_InitType.Pull = GPIO_PULLUP;
    GPIOG_InitType.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIOG_InitType);

    __GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIOC_InitType;
    GPIOC_InitType.Pin = GPIO_PIN_5;
    GPIOC_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOC_InitType.Pull = GPIO_PULLUP;
    GPIOC_InitType.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIOC_InitType);
    delay(0x0fffff);
    GPIOB->ODR |= GPIO_PIN_11;
    GPIOG->ODR |= GPIO_PIN_13;
    while(1);
}

void uart1(){
    Phuart1 = &huart1;
    __GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIOA_InitType;
    GPIOA_InitType.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIOA_InitType.Mode = GPIO_MODE_AF_PP;
    GPIOA_InitType.Pull = GPIO_PULLUP;
    GPIOA_InitType.Speed = GPIO_SPEED_FREQ_LOW;
    GPIOA_InitType.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIOA_InitType);
    
    RCC_PeriphCLKInitTypeDef rcc_init;
    rcc_init.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    HAL_RCCEx_PeriphCLKConfig(&rcc_init);                                   //时钟源配置
    
    __USART1_CLK_ENABLE();
    huart1.Instance = USART1;
    huart1.Init.BaudRate               = 115200;                         //波特率
    huart1.Init.WordLength             = IRDA_WORDLENGTH_8B;           //数据位宽
    huart1.Init.StopBits               = UART_STOPBITS_1;              //停止位
    huart1.Init.Parity                 = UART_PARITY_NONE;             //奇偶校验
    huart1.Init.Mode                   = USART_MODE_TX_RX;             //uart模式
    huart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;          //硬件流
    huart1.Init.OverSampling           = UART_OVERSAMPLING_16;         //过采样选择
    huart1.Init.OneBitSampling         = UART_ONEBIT_SAMPLING_DISABLED;//采样方式
    huart1.Init.FIFOMode               = UART_FIFOMODE_DISABLE;        //FIFO模式
    huart1.Init.TXFIFOThreshold        = ETH_MAC_TXFIFO_FULL;          //发送FIFO阈值
    huart1.Init.RXFIFOThreshold        = ETH_MAC_RXFIFO_FULL;          //接收FIFO阈值
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;      //uart高级设置
    HAL_UART_Init(&huart1);
    
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    __HAL_UART_ENABLE_IT(Phuart1, USART_IT_RXNE);
}

void uart5(){
    Phuart5 = &huart5;
    __GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIOB_InitType;
    GPIOB_InitType.Pin = GPIO_PIN_12 | GPIO_PIN_13;
    GPIOB_InitType.Mode = GPIO_MODE_AF_PP;
    GPIOB_InitType.Pull = GPIO_PULLUP;
    GPIOB_InitType.Speed = GPIO_SPEED_FREQ_LOW;
    GPIOB_InitType.Alternate = GPIO_AF14_UART5;
    HAL_GPIO_Init(GPIOB, &GPIOB_InitType);

    RCC_PeriphCLKInitTypeDef rcc_init;
    rcc_init.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    HAL_RCCEx_PeriphCLKConfig(&rcc_init);                                   //时钟源配置
    
    __UART5_CLK_ENABLE();
    huart5.Instance = UART5;
    huart5.Init.BaudRate               = 9600;                         //波特率
    huart5.Init.WordLength             = IRDA_WORDLENGTH_8B;           //数据位宽
    huart5.Init.StopBits               = UART_STOPBITS_1;              //停止位
    huart5.Init.Parity                 = UART_PARITY_NONE;             //奇偶校验
    huart5.Init.Mode                   = USART_MODE_TX_RX;             //uart模式
    huart5.Init.HwFlowCtl              = UART_HWCONTROL_NONE;          //硬件流
    huart5.Init.OverSampling           = UART_OVERSAMPLING_16;         //过采样选择
    huart5.Init.OneBitSampling         = UART_ONEBIT_SAMPLING_DISABLED;//采样方式
    huart5.Init.FIFOMode               = UART_FIFOMODE_DISABLE;        //FIFO模式
    huart5.Init.TXFIFOThreshold        = ETH_MAC_TXFIFO_FULL;          //发送FIFO阈值
    huart5.Init.RXFIFOThreshold        = ETH_MAC_RXFIFO_FULL;          //接收FIFO阈值
    huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;      //uart高级设置
    HAL_UART_Init(&huart5);
    
    HAL_NVIC_SetPriority(UART5_IRQn, 1, 0);
    NVIC_EnableIRQ(UART5_IRQn);
    __HAL_UART_ENABLE_IT(Phuart5, UART_IT_RXNE);
}

void USART1_IRQHandler(){
    if(__HAL_USART_GET_IT(Phuart1, USART_IT_RXNE)){
        Phuart5->Instance->TDR = Phuart1->Instance->RDR;
        GPIOC->ODR ^= GPIO_PIN_5;

        while(!__HAL_UART_GET_FLAG(Phuart5, UART_FLAG_TC));
    }
    __HAL_USART_CLEAR_IT(Phuart1, USART_IT_RXNE);
}

void UART5_IRQHandler(){
    if(__HAL_UART_GET_IT(Phuart5, UART_IT_RXNE)){
        Phuart1->Instance->TDR = Phuart5->Instance->RDR;
        GPIOC->ODR ^= GPIO_PIN_5;

        while(!__HAL_UART_GET_FLAG(Phuart5, UART_FLAG_TC));
    }
    __HAL_UART_CLEAR_IT(Phuart1, UART_IT_RXNE);
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Supply configuration update enable 
  */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) 
  {
    
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
  PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
  }
}

