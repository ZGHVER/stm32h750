#include "app.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_i2c_touch.h"
#include "./touch/bsp_touch_gtxx.h"
#include "./touch/button.h"

#define Touch_TSIZE 256
#define Touch_Prio 10
CPU_STK Touch_stk[Touch_TSIZE];
OS_TCB  Touck_TCB;
void    Touck_Task(void* args);

#define TouchUp_TSIZE 128
#define TouchUp_Prio 12
CPU_STK TouchUp_STK[TouchUp_TSIZE];
OS_TCB  TouchUp_TCB;
void    TouchUp_Task(void* args);

#define TouchDown_TSIZE 128
#define TouchDown_Prio 12
CPU_STK TouchDown_STK[TouchDown_TSIZE];
OS_TCB  TouchDown_TCB;
void    TouchDown_Task(void* args);

OS_MEM 	MEM_SI128;

#define SI128_BlockNum 100
#define SI128_BlockSize 16
uint8_t mem[SI128_BlockNum][SI128_BlockSize];

void down(int32_t id,int32_t x,int32_t y,int32_t w);
void up(int32_t id);

void LCD_Start(){
	OS_ERR err;
    LCD_Init();

	LCD_SelectLayer(0);
	LCD_Clear(LCD_COLOR_WHITE);  
	LCD_SelectLayer(1);
	LCD_Clear(LCD_COLOR_LUCENCY);
	LCD_SetTransparency(0, 255);
	LCD_SetTransparency(1, 255);
	
    GTP_Init_Panel();
	GTP_TouchRegisterCallBack(up, down);
	
	//触摸处理任务
	OSTaskCreate( 
		(  OS_TCB*   )&Touck_TCB,                                 //任务控制块
		( CPU_CHAR*  )"Touck_TCB",                                //任务名
		(OS_TASK_PTR )Touck_Task,                                 //任务函数指针
		(   void*    )0,                                          //首次运行时传递的参数
		(  OS_PRIO   )Touch_Prio,                                 //任务优先级
		(  CPU_STK*  )&Touch_stk[0],                              //任务堆栈基地址
		(CPU_STK_SIZE)Touch_TSIZE / 10,                           //可用最大堆栈空间
		(CPU_STK_SIZE)Touch_TSIZE,                                //任务堆栈大小
		( OS_MSG_QTY )0,                                          //任务可接收的最大消息数
		(  OS_TICK   )10,                                         //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
		(   void*    )0,                                          //TCB扩展指针
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
		(  OS_ERR*   )&err                                        //错误变量
	);
	//触摸抬起处理
	OSTaskCreate(
		(  OS_TCB*   )&TouchUp_TCB,                               //任务控制块
		( CPU_CHAR*  )"TouchUp_Task",                             //任务名
		(OS_TASK_PTR )TouchUp_Task,                               //任务函数指针
		(   void*    )0,                                          //首次运行时传递的参数
		(  OS_PRIO   )TouchUp_Prio,                               //任务优先级
		(  CPU_STK*  )&TouchUp_STK[0],                            //任务堆栈基地址
		(CPU_STK_SIZE)TouchUp_TSIZE / 10,                         //可用最大堆栈空间
		(CPU_STK_SIZE)TouchUp_TSIZE,                              //任务堆栈大小
		( OS_MSG_QTY )10,                                         //任务可接收的最大消息数
		(  OS_TICK   )0,                                          //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
		(   void*    )0,                                          //TCB扩展指针
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
		(  OS_ERR*   )&err                                        //错误变量
	);
	//触摸按下处理
	OSTaskCreate(
		(  OS_TCB*   )&TouchDown_TCB,                             //任务控制块
		( CPU_CHAR*  )"TouchDown_Task",                           //任务名
		(OS_TASK_PTR )TouchDown_Task,                             //任务函数指针
		(   void*    )0,                                          //首次运行时传递的参数
		(  OS_PRIO   )TouchDown_Prio,                             //任务优先级
		(  CPU_STK*  )&TouchDown_STK[0],                          //任务堆栈基地址
		(CPU_STK_SIZE)TouchDown_TSIZE / 10,                       //可用最大堆栈空间
		(CPU_STK_SIZE)TouchDown_TSIZE,                            //任务堆栈大小
		( OS_MSG_QTY )10,                                         //任务可接收的最大消息数
		(  OS_TICK   )0,                                          //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
		(   void*    )0,                                          //TCB扩展指针
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
		(  OS_ERR*   )&err                                        //错误变量
	);

	OSMemCreate(&MEM_SI128, 
				"block size 128",
				(void* )mem,
				SI128_BlockNum,
				SI128_BlockSize,
				&err
	);				 
}

void TouchUp_Task(void* args){
	OS_ERR       err;
	int32_t*     get;
	OS_MSG_SIZE  size;

	while(1){
		get = (int32_t*)OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &size, (void*)0, &err);
		osError_hander(err);
		BUTTON_ActUplift(*get);
		OSMemPut(&MEM_SI128, get, &err);
		osError_hander(err);
	}
}

void TouchDown_Task(void* args){
	OS_ERR       err;
	int32_t*     get;
	OS_MSG_SIZE  size;

	while(1){
		get = (int32_t*)OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &size, (void*)0, &err);
		osError_hander(err);
		BUTTON_ActPush(get[0], get[1], get[2]);
		OSMemPut(&MEM_SI128, get, &err);
		osError_hander(err);
	}
}

void down(int32_t id,int32_t x,int32_t y,int32_t w){
	OS_ERR err;
	int32_t* ret = OSMemGet(&MEM_SI128, &err);
	osError_hander(err);
	ret[0] = id;
	ret[1] = x;
	ret[2] = y;
	OSTaskQPost(&TouchDown_TCB, ret, 12, OS_OPT_POST_FIFO, &err);
	osError_hander(err);
}

void up(int32_t id){
	OS_ERR err;
	int32_t* ret = OSMemGet(&MEM_SI128, &err);
	osError_hander(err);
	*ret = id;
	OSTaskQPost(&TouchUp_TCB, ret, 4, OS_OPT_POST_FIFO, &err);
	osError_hander(err); 
}

void Touck_Task(void* args){
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

void EXTI3_IRQHandler(void){
	OS_ERR err;
	OSIntEnter();
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET){ 
		OSTaskSemPost(&Touck_TCB, OS_OPT_POST_NONE, &err);
	}  
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);     
	OSIntExit();
}
