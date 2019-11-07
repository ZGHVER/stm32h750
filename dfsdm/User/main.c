  
#include "main.h"
#include "./bsp/lcd/bsp_lcd.h"
#include "./touch/button.h"
#include "./BSP/lcd/photo1.h"
#include "./touch/bsp_touch_gtxx.h"
#include "./dfsdm/dfsdm.h"
#include "./BSP/sd_card/bsp_sdio_sd.h"
#include "./BSP/FATFS/ff.h"
#include "./BSP/FATFS/ff_gen_drv.h"
#include "./BSP/FATFS/sd_diskio.h"
#include "./touch/graph.h"
#include "arm_math.h"

extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter1;
extern DMA_HandleTypeDef hdma_dfsdm1_flt0;
extern DMA_HandleTypeDef hdma_dfsdm1_flt1;


#define Tast_TSIZE 128
#define Tast_Prio 15
CPU_STK Tast_STK[Tast_TSIZE];
OS_TCB  Tast_TCB;
void    Tast_Task(void* args);


void main(){
	OSTaskCreate(
		(  OS_TCB*   )&Test_TCB,                                   //任务控制块
		( CPU_CHAR*  )"Test_Task",                                  //任务名
		(OS_TASK_PTR )Test_Task,                                        //任务函数指针
		(   void*    )0,                                          //首次运行时传递的参数
		(  OS_PRIO   )Test_Prio,                                          //任务优先级
		(  CPU_STK*  )&Test_STK[0],                                  //任务堆栈基地址
		(CPU_STK_SIZE)Test_TSIZE / 10,                            //可用最大堆栈空间
		(CPU_STK_SIZE)Test_TSIZE,                                 //任务堆栈大小
		( OS_MSG_QTY )10,                                         //任务可接收的最大消息数
		(  OS_TICK   )0,                                          //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
		(   void*    )0,                                          //TCB扩展指针
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
		(  OS_ERR*   )&err                                      //错误变量
	);


	OSTaskCreate(
		(  OS_TCB*   )&Task_TCB,                                   //任务控制块
		( CPU_CHAR*  )"Task_Task",                                  //任务名
		(OS_TASK_PTR )Task_Task,                                        //任务函数指针
		(   void*    )0,                                          //首次运行时传递的参数
		(  OS_PRIO   )Task_Prio,                                          //任务优先级
		(  CPU_STK*  )&Task_STK[0],                                  //任务堆栈基地址
		(CPU_STK_SIZE)Task_TSIZE / 10,                            //可用最大堆栈空间
		(CPU_STK_SIZE)Task_TSIZE,                                 //任务堆栈大小
		( OS_MSG_QTY )10,                                         //任务可接收的最大消息数
		(  OS_TICK   )0,                                          //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
		(   void*    )0,                                          //TCB扩展指针
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
		(  OS_ERR*   )&err                                      //错误变量
	);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseInitTypeDef tim1_BaseInit;
	tim1_BaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
	tim1_BaseInit.TIM_CounterMode = TIM_CounterMode_Up;      //计数器模式
	tim1_BaseInit.TIM_Period = 10;                         //重装载值
	tim1_BaseInit.TIM_Prescaler = 1000;                   //分频系数
	TIM_TimeBaseInit(TIM1, &tim1_BaseInit);

	TIM_OCInitTypeDef TIM1_oc2_Init;                            //定时器1频道2PWM初始化
	TIM1_oc2_Init.TIM_OCMode = TIM_OCMode_PWM1;                 //PWM模式选择
	TIM1_oc2_Init.TIM_OCPolarity = TIM_OCPolarity_High;         //PWM有效电平设置
	TIM1_oc2_Init.TIM_OutputState = TIM_OutputState_Enable;     //输出比较状态
	TIM_OC2Init(TIM1, &TIM1_oc2_Init);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_SetCompare2(TIM1, 500);
	

}

void Tast_Task(void* args){

}