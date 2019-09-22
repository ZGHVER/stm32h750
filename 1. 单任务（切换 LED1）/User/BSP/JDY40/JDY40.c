#include "./JDY40/JDY40.h"
#include "os.h"

UART_HandleTypeDef uart5_h;


__STATIC_INLINE void JDY40_UART_INIT(){
    __GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIOB_InitType;
    GPIOB_InitType.Pin = GPIO_PIN_13 | GPIO_PIN_12;
    GPIOB_InitType.Mode = GPIO_MODE_AF_PP;
    GPIOB_InitType.Pull = GPIO_PULLUP;
    GPIOB_InitType.Speed = GPIO_SPEED_FREQ_LOW;
    GPIOB_InitType.Alternate = GPIO_AF14_UART5;
    HAL_GPIO_Init(GPIOB, &GPIOB_InitType);

    RCC_PeriphCLKInitTypeDef rcc_init;
    rcc_init.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    HAL_RCCEx_PeriphCLKConfig(&rcc_init);                                   //时钟源配置

    __UART5_CLK_ENABLE();
    uart5_h.Instance = UART5;
    uart5_h.Init.BaudRate               = 9600;                         //波特率       
    GPIO_InitTypeDef GPIOG_InitType;
    GPIOG_InitType.Pin = GPIO_PIN_13;
    GPIOG_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOG_InitType.Pull = GPIO_PULLUP;
    GPIOG_InitType.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIOG_InitType);
}

void JDY40_Init(){
    JDY40_UART_INIT();
    JDY40_GPIO_INIT();
}

static void JDY_SendString(uint8_t s[], uint8_t s_size){
    HAL_UART_Transmit(&uart5_h, s, s_size, 0xffff);
}

void JDY40_SetBandRate(uint16_t bandrate){
    JDY40_CS_0;
    JDY40_SET_0;
    JDY_SendString((uint8_t*)BaundRate, C_SizeOf_BaundRate);
    uart5_h.Instance->TDR = (bandrate & (uint16_t)0x01FF);
    HAL_Delay(2);
    JDY_SendString((uint8_t*)"\r\n", 2);
    JDY40_CS_0;
    JDY40_SET_1;
}

void UART5_IRQHandler(){
    uint8_t s;
    static uint16_t posi = 0;
<<<<<<< HEAD
=======
    OSIntEnter();
>>>>>>> 266f895f39d96eafc90547aecd3f699145c8c3af
    GPIOC->ODR ^= GPIO_PIN_5;
    if(__HAL_UART_GET_IT(&uart5_h, UART_IT_RXNE)){
        HAL_UART_Receive(&uart5_h, &s, 1, 0xff);
        //LCD_DisplayChar(posi, 0, s);
        posi += 15;
    }
    __HAL_UART_CLEAR_IT(&uart5_h, UART_IT_RXNE);
<<<<<<< HEAD
}*/
=======
    OSIntExit();
}
>>>>>>> 266f895f39d96eafc90547aecd3f699145c8c3af

