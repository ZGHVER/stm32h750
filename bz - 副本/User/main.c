
#include "main.h"
#include "./BSP/INMP441/INMP441.h"
#include "arm_math.h"
int main(void){
  SystemClock_Config();	
 __GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIOC_InitType;
    GPIOC_InitType.Pin = GPIO_PIN_5;
    GPIOC_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOC_InitType.Pull = GPIO_PULLUP;
    GPIOC_InitType.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIOC_InitType);

    GPIOC->ODR &= ~GPIO_PIN_5;
}

