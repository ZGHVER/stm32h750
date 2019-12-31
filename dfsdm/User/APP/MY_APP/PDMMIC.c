/*
 * @Author: your name
 * @Date: 2019-10-30 16:53:21
 * @LastEditTime : 2019-12-30 19:18:06
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \dfsdm\User\APP\MY_APP\PDMMIC.c
 */

#include"app.h"
#include"./dfsdm/dfsdm.h"
#include "PDMMIC.h"

#define BufferLength 1024
static int32_t F0B[2][BufferLength];
static int32_t F1B[2][BufferLength];

CPU_STK Filter0_RF_STK[Filter0_RF_TSIZE];
OS_TCB  Filter0_RF_TCB;
void    Filter0_RF_Task(void* args);

CPU_STK Filter1_RF_STK[Filter1_RF_TSIZE];
OS_TCB  Filter1_RF_TCB;
void    Filter1_RF_Task(void* args);

OS_FLAG_GRP Filter0_FlagGRP;
static const OS_FLAGS F0M0_Full = 1 << 0;
static const OS_FLAGS F0M1_Full = 1 << 1;

OS_FLAG_GRP Filter1_FlagGRP;
static const OS_FLAGS F1M0_Full = 1 << 0;
static const OS_FLAGS F1M1_Full = 1 << 1;

typedef void (*FM_Handle)(int , uint32_t*, int);

//filter0 DMA完成回调 此函数在ISR中执行
void Filter0_Finish_CB(DMA_HandleTypeDef* dma){
    static uint8_t count = 0;
    OS_ERR err;
    if(count == 0){
        OSFlagPost(&Filter0_FlagGRP, F0M0_Full, OS_OPT_POST_FLAG_SET, &err);
        count = 1;
    }else{ 
        OSFlagPost(&Filter0_FlagGRP, F0M1_Full, OS_OPT_POST_FLAG_SET, &err);
        count = 0;
    }
}
//filter1 DMA完成回调 此函数在ISR中执行
void Filter1_Finish_CB(DMA_HandleTypeDef* dma){
    static uint8_t count = 0;
    OS_ERR err;
    if(count == 0){
        OSFlagPost(&Filter1_FlagGRP, F1M0_Full, OS_OPT_POST_FLAG_SET, &err);
        count = 1;
    }else{ 
        OSFlagPost(&Filter1_FlagGRP, F1M1_Full, OS_OPT_POST_FLAG_SET, &err);
        count = 0;
    }
}

/**
 * @description: 启动PDM麦克风
 * @param F0F 声道0缓存接受满 void F0F(int memCode, uint32_t* memAddr, int Size)
 * @param F1F 声道1缓存接受满 void F1F(int memCode, uint32_t* memAddr, int Size)
 */
void PDMMIC_Start(FM_Handle F0F, FM_Handle F1F){
    OS_ERR err;
    DFSDM_Init();

    OSTaskCreate(
        (  OS_TCB*   )&Filter0_RF_TCB,                            //任务控制块
        ( CPU_CHAR*  )"Filter0_RF_Task",                          //任务名
        (OS_TASK_PTR )Filter0_RF_Task,                            //任务函数指针
        (   void*    )F0F,                                        //首次运行时传递的参数
        (  OS_PRIO   )Filter0_RF_Prio,                            //任务优先级
        (  CPU_STK*  )&Filter0_RF_STK[0],                         //任务堆栈基地址
        (CPU_STK_SIZE)Filter0_RF_TSIZE / 10,                      //可用最大堆栈空间
        (CPU_STK_SIZE)Filter0_RF_TSIZE,                           //任务堆栈大小
        ( OS_MSG_QTY )Filter0_RF_MsgSIZE,                         //任务可接收的最大消息数
        (  OS_TICK   )Filter0_RF_TICKS,                           //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
        (   void*    )0,                                          //TCB扩展指针
        (  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
        (  OS_ERR*   )&err                                        //错误变量
    );

    OSTaskCreate(
        (  OS_TCB*   )&Filter1_RF_TCB,                            //任务控制块
        ( CPU_CHAR*  )"Filter1_RF_Task",                          //任务名
        (OS_TASK_PTR )Filter1_RF_Task,                            //任务函数指针
        (   void*    )F1F,                                        //首次运行时传递的参数
        (  OS_PRIO   )Filter1_RF_Prio,                            //任务优先级
        (  CPU_STK*  )&Filter1_RF_STK[0],                         //任务堆栈基地址
        (CPU_STK_SIZE)Filter1_RF_TSIZE / 10,                      //可用最大堆栈空间
        (CPU_STK_SIZE)Filter1_RF_TSIZE,                           //任务堆栈大小
        ( OS_MSG_QTY )Filter1_RF_MsgSIZE,                         //任务可接收的最大消息数
        (  OS_TICK   )Filter1_RF_TICKS,                           //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
        (   void*    )0,                                          //TCB扩展指针
        (  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
        (  OS_ERR*   )&err                                        //错误变量
    );

    OSFlagCreate(&Filter0_FlagGRP, "filter0 flag", 0, &err);
    OSFlagCreate(&Filter1_FlagGRP, "filter1 flag", 0, &err);

    DFSDM_StartConv((uint32_t*)&F0B[0][0], (uint32_t*)&F0B[1][0], //filter0缓存
                    (uint32_t*)&F1B[0][0], (uint32_t*)&F1B[1][0], //filter1缓存
                    BufferLength, 
                    Filter0_Finish_CB, //filter0缓存满回调
                    Filter1_Finish_CB  //filter1缓存满回调
                    );
}

void  Filter0_RF_Task(void* args){
    OS_ERR err;
    FM_Handle handle = (FM_Handle)args;
    while (1){
        OS_FLAGS flag = OSFlagPend(&Filter0_FlagGRP, 
                                   F0M0_Full | F0M1_Full, 0, 
                                   OS_OPT_PEND_FLAG_SET_ANY | OS_OPT_PEND_FLAG_CONSUME,
                                   (CPU_TS *)0,
                                   &err
                                  );
        if((flag & F0M0_Full) == F0M0_Full){
            handle(0, (uint32_t*)&F0B[0][0], BufferLength);
        }else if((flag & F0M1_Full) == F0M1_Full){
            handle(1, (uint32_t*)&F0B[1][0], BufferLength);
        }else{
            
        }
    }
}

void  Filter1_RF_Task(void* args){
    OS_ERR err;
    FM_Handle handle = (FM_Handle)args;
    while (1){
        OS_FLAGS flag = OSFlagPend(&Filter1_FlagGRP, 
                                   F1M0_Full | F1M1_Full, 0, 
                                   OS_OPT_PEND_FLAG_SET_ANY | OS_OPT_PEND_FLAG_CONSUME,
                                   (CPU_TS *)0,
                                   &err
                                  );
        if((flag & F1M0_Full) == F0M0_Full){
            handle(0, (uint32_t*)&F1B[0][0], BufferLength);
        }else if((flag & F0M1_Full) == F0M1_Full){
            handle(1, (uint32_t*)&F1B[1][0], BufferLength);
        }else{
            
        }
    }
}
