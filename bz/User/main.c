
#include "main.h"
#include "./bsp/lcd/bsp_lcd.h"
#include "./touch/button.h"
#include "./BSP/lcd/photo1.h"
#include "./touch/bsp_touch_gtxx.h"
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
extern OS_MEM SDRAM_MEM;
int32_t lastid = -1;
BUTTON but[4];

#define Test_TSIZE 128
#define Test_Prio 15
CPU_STK Test_STK[Test_TSIZE];
OS_TCB  Test_TCB;
void    Test_Task(void* args);

#define Test1_TSIZE 128
#define Test1_Prio 15
CPU_STK Test1_STK[Test_TSIZE];
OS_TCB  Test1_TCB;
void    Test1_Task(void* args);



void dd(BUTTON* but){
 char s[10];
    sprintf(s, "%d", but->id);
    LCD_DisplayStringAtABSPos(100, 50, (unsigned char*)s);
}

int main(void){
	OS_ERR err;
	
  	SystemClock_Config();	
	//BSP_SystemClkCfg();
  	HAL_Init();
	BSP_Init();
	CPU_Init();
	SDRAM_Init();
	BUTTON_SetBorderColor(LCD_COLOR_BROWN);
	OSInit(&err);
	LCD_Start();
	BUTTON but[16];
	osError_hander(err);
	
	Button_KeyBoard_Init(but, 400, 100, 80);

	OSTaskCreate(
		(  OS_TCB*   )&Test_TCB,                                   //任务控制块
		( CPU_CHAR*  )"test",                                      //任务名
		(OS_TASK_PTR )Test_Task,                                  //任务函数指针
		(   void*    )0,                                          //首次运行时传递的参数
		(  OS_PRIO   )Test_Prio,                                  //任务优先级
		(  CPU_STK*  )&Test_STK[0],                               //任务堆栈基地址
		(CPU_STK_SIZE)Test_TSIZE / 10,                            //可用最大堆栈空间
		(CPU_STK_SIZE)Test_TSIZE,                                 //任务堆栈大小
		( OS_MSG_QTY )10,                                         //任务可接收的最大消息数
		(  OS_TICK   )0,                                          //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
		(   void*    )0,                                          //TCB扩展指针
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
		(  OS_ERR*   )&err                                        //错误变量
	);

	OSTaskCreate(
		(  OS_TCB*   )&Test1_TCB,                                   //任务控制块
		( CPU_CHAR*  )"Test1_Task",                                 //任务名
		(OS_TASK_PTR )Test1_Task,                                   //任务函数指针
		(   void*    )0,                                            //首次运行时传递的参数
		(  OS_PRIO   )Test1_Prio,                                   //任务优先级
		(  CPU_STK*  )&Test1_STK[0],                                //任务堆栈基地址
		(CPU_STK_SIZE)Test1_TSIZE / 10,                             //可用最大堆栈空间
		(CPU_STK_SIZE)Test1_TSIZE,                                  //任务堆栈大小
		( OS_MSG_QTY )12,                                           //任务可接收的最大消息数
		(  OS_TICK   )0,                                            //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
		(   void*    )0,                                            //TCB扩展指针
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,    //包含有关任务行为的其他信息（或选项）
		(  OS_ERR*   )&err                                          //错误变量
	);

		OSStart(&err);
	uint32_t k;
	while(1){	
		k ++;
  };
}

void Test_Task(void* args){
	OS_ERR err;
	while(1){
		LCD_SetTextColor(LCD_COLOR_YELLOW);
		LCD_FillRect(100, 100, 50, 50);
		OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
		LCD_SetTextColor(LCD_COLOR_BLUE);
		LCD_FillRect(100, 100, 50, 50);
		OSTimeDly(400, OS_OPT_TIME_DLY, &err);
	}
}

void Test1_Task(void* args){
	OS_ERR err;
	while(1){
		LCD_SetTextColor(LCD_COLOR_RED);
		LCD_FillRect(150, 100, 50, 50);
		OSTimeDly(400, OS_OPT_TIME_DLY, &err);
		LCD_SetTextColor(LCD_COLOR_GREEN);
		LCD_FillRect(150, 100, 50, 50);
		OSTimeDly(200, OS_OPT_TIME_DLY, &err);
	}
}



