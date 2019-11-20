  
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

#define TF2_TSIZE   128
#define TF2_Prio    10
#define TF2_MsgSIZE 10
#define TF2_TICKS   0
CPU_STK TF2_STK[TF2_TSIZE];
OS_TCB  TF2_TCB;
void    TF2_Task(void* args);

#define TF1_TSIZE   128
#define TF1_Prio    10
#define TF1_MsgSIZE 10
#define TF1_TICKS   0
CPU_STK TF1_STK[TF1_TSIZE];
OS_TCB  TF1_TCB;
void    TF1_Task(void* args);

#define FLAG_TF1 0x01
#define FLAG_TR1 0x02
OS_FLAG_GRP tf1_flg;
#define FLAG_TF2 0x01
#define FLAG_TR2 0x02
OS_FLAG_GRP tf2_flg;

MDMA_HandleTypeDef hmdma_mdma_channel41_sw_0;
DMA_HandleTypeDef hdma_memtomem_dma1_stream1;
DMA_HandleTypeDef hdma_memtomem_dma1_stream2;
#define BS 4096
uint32_t TB0[BS];
uint32_t TB1[BS];
uint32_t RB0[BS];
uint32_t RB1[BS];

uint8_t a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
uint8_t b[10], c[10];
char ss[100];
void MdmaInit();
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
        ( CPU_CHAR*  )"LED_Task",                                  //任务名
        (OS_TASK_PTR )LED_Task,                                        //任务函数指针
        (   void*    )0,                                          //首次运行时传递的参数
        (  OS_PRIO   )LED_Prio,                                          //任务优先级
        (  CPU_STK*  )&LED_STK[0],                                  //任务堆栈基地址
        (CPU_STK_SIZE)LED_TSIZE / 10,                            //可用最大堆栈空间
        (CPU_STK_SIZE)LED_TSIZE,                                 //任务堆栈大小
        ( OS_MSG_QTY )10,                                         //任务可接收的最大消息数
        (  OS_TICK   )0,                                          //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
        (   void*    )0,                                          //TCB扩展指针
        (  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
        (  OS_ERR*   )&err                                      //错误变量
    );
    OSStart(&err);
    while(1){
    }
}

void tf(MDMA_HandleTypeDef* d){
    sprintf(ss, "%d,%d,%d,%d,%d", b[0], b[1], b[2], b[3], b[4]);
    OLED_ShowString(10, 10, ss, 12);
    OLED_Refresh_Gram();
}

void TF1(DMA_HandleTypeDef* hd){
    OS_ERR err;
    OSFlagPost(&tf1_flg, FLAG_TF1, OS_OPT_POST_FLAG_SET, &err);
    osError_hander(err);
}

void TF2(DMA_HandleTypeDef* hd){
    OS_ERR err;
    OSFlagPost(&tf2_flg, FLAG_TF2, OS_OPT_POST_FLAG_SET, &err);
    osError_hander(err);
}

void RF1(DMA_HandleTypeDef *hd){
    OS_ERR err;
    OSFlagPost(&tf1_flg, FLAG_TR1, OS_OPT_POST_FLAG_SET, &err);
    if(TF1_TCB.TaskState == OS_TASK_STATE_SUSPENDED){
        OSTaskResume(&TF1_TCB, &err);
    }
    osError_hander(err);
}

void RF2(DMA_HandleTypeDef* hd){
    OS_ERR err;
    OSFlagPost(&tf2_flg, FLAG_TR2, OS_OPT_POST_FLAG_SET, &err);
    osError_hander(err);
}
 
int i = 0;
int q = 0;
int w = 0;
void TF1_Task(void* args){
    OS_ERR err;
    while(1){
        OSFlagPend(&tf1_flg, 
                   FLAG_TF1 | FLAG_TR1, 
                   100,
                   OS_OPT_PEND_FLAG_SET_ALL | OS_OPT_PEND_FLAG_CONSUME | OS_OPT_PEND_BLOCKING, 
                   (CPU_TS*)0, 
                   &err
                   );
        if(err == OS_ERR_TIMEOUT){
            ES9038_StopPlay();
            OSTaskSuspend(&TF2_TCB, &err);
            OSTaskSuspend(0, &err);
        }
        
        HAL_DMA_Start_IT(&hdma_memtomem_dma1_stream1, (uint32_t)&RB0[0], (uint32_t)&TB0[0], BS);
        if(TF2_TCB.TaskState == OS_TASK_STATE_SUSPENDED){
            OSTaskResume(&TF2_TCB, &err);
            ES9038_Continue();
        }

        if(i == 0){
            ES9038_StartPlay(&TB0[0], &TB1[0], BS, TF1, TF2);
            i = 1;
        }
    }
}

void TF2_Task(void* args){
    OS_ERR err;
    while(1){
        OSFlagPend(&tf2_flg, 
                   FLAG_TF2 | FLAG_TR2, 
                   0,
                   OS_OPT_PEND_FLAG_SET_ALL | OS_OPT_PEND_FLAG_CONSUME | OS_OPT_PEND_BLOCKING, 
                   (CPU_TS*)0, 
                   &err
                   );
        HAL_DMA_Start_IT(&hdma_memtomem_dma1_stream2, (uint32_t)&RB1[0], (uint32_t)&TB1[0], BS);
		}
}

void Init_Task(void* args){
    OS_ERR err;
    OLED_Init();
    MdmaInit();
    CSR8675_Init();
    ES9038_Init();

    OSFlagCreate(&tf1_flg, "tf1", 0, &err);
    OSFlagCreate(&tf2_flg, "tf2", 0, &err);
    OSFlagPost(&tf1_flg, FLAG_TF1, OS_OPT_POST_FLAG_SET, &err);
    OSTaskCreate(
        (  OS_TCB*   )&TF2_TCB,                                  //任务控制块
        ( CPU_CHAR*  )"TF2_Task",                                //任务名
        (OS_TASK_PTR )TF2_Task,                                  //任务函数指针
        (   void*    )0,                                         //首次运行时传递的参数
        (  OS_PRIO   )TF2_Prio,                                  //任务优先级
        (  CPU_STK*  )&TF2_STK[0],                               //任务堆栈基地址
        (CPU_STK_SIZE)TF2_TSIZE / 10,                            //可用最大堆栈空间
        (CPU_STK_SIZE)TF2_TSIZE,                                 //任务堆栈大小
        ( OS_MSG_QTY )TF2_MsgSIZE,                               //任务可接收的最大消息数
        (  OS_TICK   )TF2_TICKS,                                 //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
        (   void*    )0,                                         //TCB扩展指针
        (  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, //包含有关任务行为的其他信息（或选项）
        (  OS_ERR*   )&err                                       //错误变量
    );

    OSTaskCreate(
        (  OS_TCB*   )&TF1_TCB,                                   //任务控制块
        ( CPU_CHAR*  )"TF1_Task",                                 //任务名
        (OS_TASK_PTR )TF1_Task,                                   //任务函数指针
        (   void*    )0,                                          //首次运行时传递的参数
        (  OS_PRIO   )TF1_Prio,                                   //任务优先级
        (  CPU_STK*  )&TF1_STK[0],                                //任务堆栈基地址
        (CPU_STK_SIZE)TF1_TSIZE / 10,                             //可用最大堆栈空间
        (CPU_STK_SIZE)TF1_TSIZE,                                  //任务堆栈大小
        ( OS_MSG_QTY )TF1_MsgSIZE,                                //任务可接收的最大消息数
        (  OS_TICK   )TF1_TICKS,                                  //在任务之间循环时的时间片的时间量（以刻度表示）指定0以使用默认值
        (   void*    )0,                                          //TCB扩展指针
        (  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //包含有关任务行为的其他信息（或选项）
        (  OS_ERR*   )&err                                        //错误变量
    );

    CSR8675_StartReceive(&RB0[0], &RB1[0], BS, RF1, RF2);
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
         sprintf(ss, "%d,%d,%d,%d,%d", b[0], b[1], b[2], b[3], b[4]);
        OLED_ShowString(10, 10, ss, 12);
        OLED_ShowString(40, 40, (uint8_t *)&s[0], 12);

        OLED_Refresh_Gram();
        
    }
}

void MdmaInit(){
  __HAL_RCC_MDMA_CLK_ENABLE();
  hmdma_mdma_channel41_sw_0.Instance = MDMA_Channel1;
  hmdma_mdma_channel41_sw_0.Init.Request = MDMA_REQUEST_SW;
  hmdma_mdma_channel41_sw_0.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
  hmdma_mdma_channel41_sw_0.Init.Priority = MDMA_PRIORITY_VERY_HIGH;
  hmdma_mdma_channel41_sw_0.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hmdma_mdma_channel41_sw_0.Init.SourceInc = MDMA_SRC_INC_HALFWORD;
  hmdma_mdma_channel41_sw_0.Init.DestinationInc = MDMA_DEST_INC_BYTE;
  hmdma_mdma_channel41_sw_0.Init.SourceDataSize = MDMA_SRC_DATASIZE_BYTE;
  hmdma_mdma_channel41_sw_0.Init.DestDataSize = MDMA_DEST_DATASIZE_BYTE;
  hmdma_mdma_channel41_sw_0.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
  hmdma_mdma_channel41_sw_0.Init.BufferTransferLength = 1;
  hmdma_mdma_channel41_sw_0.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
  hmdma_mdma_channel41_sw_0.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
  hmdma_mdma_channel41_sw_0.Init.SourceBlockAddressOffset = 0;
  hmdma_mdma_channel41_sw_0.Init.DestBlockAddressOffset = 0;
  if (HAL_MDMA_Init(&hmdma_mdma_channel41_sw_0) != HAL_OK)
  {
      while(1);
  }
  //__HAL_MDMA_ENABLE(&hmdma_mdma_channel41_sw_0);
 // HAL_NVIC_EnableIRQ(MDMA_IRQn);
  //HAL_NVIC_SetPriority(MDMA_IRQn, 0, 0);

  //HAL_MDMA_RegisterCallback(&hmdma_mdma_channel41_sw_0, HAL_MDMA_XFER_CPLT_CB_ID, tf);


    __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure DMA request hdma_memtomem_dma1_stream1 on DMA1_Stream1 */
  hdma_memtomem_dma1_stream1.Instance = DMA1_Stream1;
  hdma_memtomem_dma1_stream1.Init.Request = DMA_REQUEST_MEM2MEM;
  hdma_memtomem_dma1_stream1.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem_dma1_stream1.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_memtomem_dma1_stream1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_memtomem_dma1_stream1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_memtomem_dma1_stream1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_memtomem_dma1_stream1.Init.Mode = DMA_NORMAL;
  hdma_memtomem_dma1_stream1.Init.Priority = DMA_PRIORITY_VERY_HIGH;
  hdma_memtomem_dma1_stream1.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma_memtomem_dma1_stream1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
  hdma_memtomem_dma1_stream1.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_memtomem_dma1_stream1.Init.PeriphBurst = DMA_PBURST_SINGLE;
  if (HAL_DMA_Init(&hdma_memtomem_dma1_stream1) != HAL_OK)
  {
  }

    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 0);

  hdma_memtomem_dma1_stream2.Instance = DMA1_Stream2;
  hdma_memtomem_dma1_stream2.Init.Request = DMA_REQUEST_MEM2MEM;
  hdma_memtomem_dma1_stream2.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem_dma1_stream2.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_memtomem_dma1_stream2.Init.MemInc = DMA_MINC_ENABLE;
  hdma_memtomem_dma1_stream2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_memtomem_dma1_stream2.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_memtomem_dma1_stream2.Init.Mode = DMA_NORMAL;
  hdma_memtomem_dma1_stream2.Init.Priority = DMA_PRIORITY_VERY_HIGH;
  hdma_memtomem_dma1_stream2.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma_memtomem_dma1_stream2.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
  hdma_memtomem_dma1_stream2.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_memtomem_dma1_stream2.Init.PeriphBurst = DMA_PBURST_SINGLE;
  if (HAL_DMA_Init(&hdma_memtomem_dma1_stream2) != HAL_OK)
  {
  }

    HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
    HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 1, 0);
}
