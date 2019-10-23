#include "app.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_i2c_touch.h"
#include "./touch/bsp_touch_gtxx.h"
#include "./touch/button.h"

#define Tuch_TSIZE 256
#define Tuch_Prio 10
CPU_STK Tuch_stk[Tuch_TSIZE];
OS_TCB  Tuck_TCB;
void    Tuck_Task(void* args);


void down(int32_t id,int32_t x,int32_t y,int32_t w){
	BUTTON_ActPush(id, x, y);
}

void up(int32_t id){
	BUTTON_ActUplift(id);
}

void LCD_Start(){
	OS_ERR err;
    LCD_Init();
    GTP_Init_Panel();

	LCD_SelectLayer(0);
	LCD_Clear(LCD_COLOR_WHITE);  
	LCD_SelectLayer(1);
	LCD_Clear(LCD_COLOR_LUCENCY);
	LCD_SetTransparency(0, 255);
	LCD_SetTransparency(1, 255);
	
	GTP_TouchRegisterCallBack(up, down);
	
	
	OSTaskCreate(
		(  OS_TCB*   )&Tuck_TCB,                                  //任务控制块
		( CPU_CHAR*  )"Tuck_TCB",                                 //任务名
		(OS_TASK_PTR )Tuck_Task,                                  //任务函数指针
		(   void*    )0,                                          //首次运行时传递的参数
		(  OS_PRIO   )Tuch_Prio,                                  //任务优先级
		(  CPU_STK*  )&Tuch_stk[0],                               //任务堆栈基地址
		(CPU_STK_SIZE)Tuch_TSIZE / 10,                            //可用最大堆栈空间
		(CPU_STK_SIZE)Tuch_TSIZE,                                 //任务堆栈大小
		( OS_MSG_QTY )0,                                          //任务可接收的最大消息数
		(  OS_TICK   )0,                                          //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
		(   void*    )0,                                          //TCB扩展指针
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
		(  OS_ERR*   )&err                                        //错误变量
	);

}

void  Tuck_Task(void* args){
	OS_ERR err;
	while (1)
	{
		OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, (CPU_TS *)0, &err);
		osError_hander(err);  
		CPU_IntDis(); 
		Goodix_TS_Work_Func();
		CPU_IntEn();
	}
}

void EXTI3_IRQHandler(void)
{
	OS_ERR err;
	OSIntEnter();
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET) //确保是否产生了EXTI Line中断
	{
		//Goodix_TS_Work_Func();
		OSTaskSemPost(&Tuck_TCB, OS_OPT_POST_NONE, &err);
	}  
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);     //清除中断标志位
	OSIntExit();
}
