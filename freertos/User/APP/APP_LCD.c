
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_i2c_touch.h"
#include "./touch/bsp_touch_gtxx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "./touch/button.h"

SemaphoreHandle_t LCD_sem;
void  Tuck_Task(void* args);

#define Tuch_Prio  3
TaskHandle_t Tuch_TCB = NULL;
#define ButtonUp_Prio 10
TaskHandle_t ButtonUp_TCB = NULL;
#define ButtonDown_Prio 10
TaskHandle_t ButtonDown_TCB = NULL;

QueueHandle_t UpQueue;
QueueHandle_t DownQueue;

static void up(int32_t id);
static void down(int32_t id,int32_t x,int32_t y,int32_t w);
void ButtonUp_Task(void* args);
void ButtonDown_Task(void* args);

void LCD_Start(){
    LCD_Init();

    GTP_Init_Panel();
	GTP_TouchRegisterCallBack(up, down);

	LCD_SelectLayer(0);
	LCD_Clear(LCD_COLOR_WHITE);  
	LCD_SelectLayer(1);
	LCD_Clear(LCD_COLOR_LUCENCY);
	LCD_SetTransparency(0, 255);
	LCD_SetTransparency(1, 255);

	
	LCD_sem = xSemaphoreCreateBinary();

	xTaskCreate(Tuck_Task,
				"task",
				512,
				0,
				Tuch_Prio,
				&Tuch_TCB
	);

	xTaskCreate(ButtonUp_Task,
				"task",
				256,
				0,
				ButtonUp_Prio,
				&ButtonUp_TCB
	);

	xTaskCreate(ButtonDown_Task,
				"task",
				256,
				0,
				ButtonDown_Prio,
				&ButtonDown_TCB
	);

	UpQueue = xQueueCreate(10, sizeof(int32_t));
	DownQueue = xQueueCreate(10, sizeof(int32_t) * 3);

}


void ButtonUp_Task(void* args){
	uint32_t id;
	while (1){
		xQueueReceive(UpQueue ,&id, portMAX_DELAY);
		BUTTON_ActUplift(id);
	}
	
}

void ButtonDown_Task(void* args){
	uint32_t* mes;
	while (1){
		xQueueReceive( DownQueue, mes, portMAX_DELAY);
		BUTTON_ActPush(mes[0], mes[1], mes[2]);
	}
	vPortFree(mes);
}

void  Tuck_Task(void* args){
	BaseType_t ret;
	while (1)
	{
		ret = xSemaphoreTake(LCD_sem,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
		Goodix_TS_Work_Func();
	}

}

static void up(int32_t id){
	//xQueueSend(UpQueue, &id, 0);
	BUTTON_ActUplift(id);
}

static void down(int32_t id,int32_t x,int32_t y,int32_t w){
/*	uint32_t* pd = (uint32_t*)pvPortMalloc(3 * sizeof(uint32_t));
	uint32_t* pr = pd;
	*pd++ =id;
	*pd++ = x;
	*pd++ = y;
	xQueueSend(DownQueue, pr, 0);*/
	BUTTON_ActPush(id, x, y);
}

void EXTI3_IRQHandler(void)
{
	BaseType_t dd;
	uint32_t k = taskENTER_CRITICAL_FROM_ISR();
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET) //确保是否产生了EXTI Line中断
	{
 		dd = xSemaphoreGive( LCD_sem );//给出二值信号量
		
	}  
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);     //清除中断标志位
	taskEXIT_CRITICAL_FROM_ISR(k);
}
