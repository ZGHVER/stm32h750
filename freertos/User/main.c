

#include "core_delay.h"   
#include "bsp_debug_usart.h"

#include "./lcd/bsp_lcd.h"
#include "./APP/APP.h"
#include "./touch/button.h"
#include "./touch/bsp_touch_gtxx.h"
#include "queue.h"

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
int32_t lastid = -1;
BUTTON but[4];

TaskHandle_t Test_tcb;
void Test_Task(void* Args);

void dd(BUTTON* but){
 char s[10];
    sprintf(s, "%d", but->id);
    LCD_DisplayStringAtABSPos(100, 50, s);
}

int main(void){
  	ALL_Init();
	SDRAM_Init();
	LCD_Start();
	
	BUTTON_SetBorderColor(LCD_COLOR_BROWN);
	Button_KeyBoard_Init(300, 120, 80);
	
	BUTTON d;
	BUTTON_Init(&d, 20, 22, 100, 300, 80 ,80,LCD_COLOR_LIGHTBLUE, LCD_COLOR_BLACK,"asd", &Font16, dd);
	BUTTON_Drow(&d);

  	xTaskCreate(Test_Task,
				"task",
				512,
				0,
				10,
				&Test_tcb
	);

	vTaskStartScheduler(); 
  	while(1){};  
}

void Test_Task(void* Args){
  while(1){
    LCD_SetTextColor(LCD_COLOR_BROWN);
    LCD_FillRect(100, 100, 50, 50);
    vTaskDelay(500);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(100, 100, 50, 50);
    vTaskDelay(500);
  }
}


