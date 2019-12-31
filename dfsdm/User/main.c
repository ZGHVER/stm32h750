  
#include "main.h"
#include "./dfsdm/dfsdm.h"
#include "./CSR8675/CSR8675.h"
#include "./ES9038/ES9038.h"
#include "arm_math.h"
#include "bsp.h"
#include "./OLED/oled.h"

#define Init_TSIZE 512
#define Init_Prio 15
CPU_STK Init_STK[Init_TSIZE];
OS_TCB  Init_TCB;
void    Init_Task(void* args);  

#define LED_TSIZE 128
#define LED_Prio 9
CPU_STK LED_STK[LED_TSIZE];
OS_TCB  LED_TCB;
void    LED_Task(void* args);

int main(){
    OS_ERR err;
		SystemClock_Config1();
    HAL_Init();
    BSP_Init();
    CPU_Init();
    OSInit(&err);

    OSTaskCreate(
        (  OS_TCB*   )&Init_TCB,                                  //任务控制块
        ( CPU_CHAR*  )"Init_Task",                                //任务名
        (OS_TASK_PTR )Init_Task,                                  //任务函数指针
        (   void*    )0,                                          //首次运行时传递的参数
        (  OS_PRIO   )Init_Prio,                                  //任务优先级
        (  CPU_STK*  )&Init_STK[0],                               //任务堆栈基地址
        (CPU_STK_SIZE)Init_TSIZE / 10,                            //可用最大堆栈空间
        (CPU_STK_SIZE)Init_TSIZE,                                 //任务堆栈大小
        ( OS_MSG_QTY )10,                                         //任务可接收的最大消息数
        (  OS_TICK   )0,                                          //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
        (   void*    )0,                                          //TCB扩展指针
        (  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
        (  OS_ERR*   )&err                                        //错误变量
    );
   
    OSTaskCreate(
        (  OS_TCB*   )&LED_TCB,                                   //任务控制块
        ( CPU_CHAR*  )"LED_Task",                                 //任务名
        (OS_TASK_PTR )LED_Task,                                   //任务函数指针
        (   void*    )0,                                          //首次运行时传递的参数
        (  OS_PRIO   )LED_Prio,                                   //任务优先级
        (  CPU_STK*  )&LED_STK[0],                                //任务堆栈基地址
        (CPU_STK_SIZE)LED_TSIZE / 10,                             //可用最大堆栈空间
        (CPU_STK_SIZE)LED_TSIZE,                                  //任务堆栈大小
        ( OS_MSG_QTY )10,                                         //任务可接收的最大消息数
        (  OS_TICK   )0,                                          //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
        (   void*    )0,                                          //TCB扩展指针
        (  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
        (  OS_ERR*   )&err                                        //错误变量
    );
    OSStart(&err);
    while(1){
    }
}

void Init_Task(void* args){
    OS_ERR err;
    OLED_Init();
    CSR8675_Init();
    ES9038_Init();
    DFSDM_Init();

    __GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIOC_InitType;
    GPIOC_InitType.Pin = GPIO_PIN_5;
    GPIOC_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOC_InitType.Pull = GPIO_PULLUP;
    GPIOC_InitType.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIOC_InitType);

    OSTaskDel(0, &err);
    osError_hander(err);
}

void LED_Task(void* args){
    OS_ERR err;
    char s[10];
    int i = 0;
    while(1){
        OSTimeDly(300, OS_OPT_TIME_DLY, &err);
        sprintf(s, "%d", i++);
        GPIOC->ODR ^= GPIO_PIN_5;
       //  sprintf(ss, "%d,%d,%d,%d,%d", b[0], b[1], b[2], b[3], b[4]);
       // OLED_ShowString(10, 10, ss, 12);
        OLED_ShowString(40, 40, (uint8_t *)&s[0], 12);

        OLED_Refresh_Gram();

    }
}
