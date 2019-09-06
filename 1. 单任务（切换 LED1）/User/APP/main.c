
#include <stdio.h>
#include <bsp.h>
#include <app_cfg.h>
#include <os_app_hooks.h>
#include <stm32h7xx_hal.h>
#include "lcd/bsp_lcd.h"
#include "./fonts/fonts.h"
#include "./mpu/bsp_mpu.h"
#include "./JDY40/JDY40.h"


#define BaseTaskSTRSize 512
OS_TCB BaseTaskTCB;
CPU_STK BaseTaskSTK[BaseTaskSTRSize];
static void BaseTask(void  *p_arg);

int main(void){
    OS_ERR   err;
    HAL_Init();                                                             //HAL???,??? 1
    BSP_SystemClkCfg();                                                     //???CPU??? 400Mhz
    CPU_Init();                                                             //??? CPU ??(???????????????)
    Mem_Init();                                                             //?????????(?????????)
    OSInit(&err);                                                           //???uC/OS-III??
    App_OS_SetAllHooks();
    SCB_EnableDCache();
    SCB_EnableICache();

    OSTaskCreate(
        (  OS_TCB*   )&BaseTaskTCB,                                  
        ( CPU_CHAR*  )"BaseTask",                                 
        (OS_TASK_PTR )BaseTask,                                      
        (   void*    )0,                                        
        (  OS_PRIO   )5,                                         
        (  CPU_STK*  )&BaseTaskSTK[0],                               
        (CPU_STK_SIZE)BaseTaskSTRSize / 10,                          
        (CPU_STK_SIZE)BaseTaskSTRSize,                               
        ( OS_MSG_QTY )10u,                                        
        (  OS_TICK   )0,                                        
        (   void*    )0,                                          
        (  OS_OPT    )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
        (  OS_ERR*   )&err                                     
    );

    

    OSStart(&err);                                                        

}

static void BaseTask(void *p_arg){
    OS_ERR err;
    int i = 0;
    BSP_Init(); 
    
  //Board_MPU_Config(0, MPU_Normal_WT, 0xD0000000, MPU_32MB);
  //Board_MPU_Config(1, MPU_Normal_WT, 0x24000000, MPU_512KB);


    LCD_Init();
    LCD_LayerInit(0, LCD_FB_START_ADDRESS,ARGB8888);
	LCD_LayerInit(1, LCD_FB_START_ADDRESS+(LCD_GetXSize()*LCD_GetYSize()*4),ARGB8888);
    LCD_DisplayOn(); 
	LCD_SelectLayer(0);
	LCD_Clear(LCD_COLOR_BLACK);  
	LCD_SelectLayer(1);
	LCD_Clear(LCD_COLOR_TRANSPARENT);
	LCD_SetTransparency(0, 255);
	LCD_SetTransparency(1, 0);

    __GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIOC_InitType;
    GPIOC_InitType.Pin = GPIO_PIN_5;
    GPIOC_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOC_InitType.Pull = GPIO_PULLUP;
    GPIOC_InitType.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIOC_InitType);
    GPIOC->ODR |= GPIO_PIN_5;
    LCD_SelectLayer(0);
    LCD_Clear(LCD_COLOR_BLACK);
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_SetFont(&LCD_DEFAULT_FONT);
     GPIOC->ODR &= ~GPIO_PIN_5;
    JDY40_Init();
    HAL_Delay(1000);
    JDY40_SetBandRate('4');
    char s[20];
    while(1){
      /*  sprintf(s,"%d",i);
        LCD_DisplayStringLine(1, s);
        if(i > 1000)
            i = 0;
        OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
		i++;*/
    }
}
