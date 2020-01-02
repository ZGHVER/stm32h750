  
#include "main.h"
#include "./dfsdm/dfsdm.h"
#include "./CSR8675/CSR8675.h"
#include "./ES9038/ES9038.h"
#include "arm_math.h"
#include "bsp.h"
#include "./OLED/oled.h"

MDMA_HandleTypeDef hmdma_mdma_channel40_sw_0;
MDMA_HandleTypeDef hmdma_mdma_channel41_sw_0;

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

uint16_t k1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
uint16_t K2[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
uint16_t k[20];
uint16_t kk1[20] = {0};

void MDMA_Init();
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
    MDMA_Init();
    //CSR8675_Init();
    //ES9038_Init();
    //DFSDM_Init();

    __GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIOC_InitType;
    GPIOC_InitType.Pin = GPIO_PIN_5;
    GPIOC_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOC_InitType.Pull = GPIO_PULLUP;
    GPIOC_InitType.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIOC_InitType);

    HAL_MDMA_Start_IT(&hmdma_mdma_channel40_sw_0, (uint32_t)k1, (uint32_t)k, 2, 5);
    OSTimeDly(100, OS_OPT_TIME_DLY, &err);
    HAL_MDMA_Start_IT(&hmdma_mdma_channel41_sw_0, (uint32_t)k1, (uint32_t)kk1, 2, 5);
    OSTaskDel(0, &err);
    osError_hander(err);
}

void LED_Task(void* args){
    OS_ERR err;
    char s[40];
    int i = 0;
    OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
    SCB_InvalidateDCache_by_Addr((uint32_t*)k, 10);
    sprintf(s, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", k[0], k[1], k[2], k[3], k[4], k[5], k[6], k[7], k[8], k[9], k[10]);
    OLED_ShowString(0, 0, s, 12);
    SCB_InvalidateDCache_by_Addr((uint32_t*)kk1, 5);
    sprintf(s, "%d,%d,%d,%d,%d", kk1[0], kk1[1], kk1[2], kk1[3], kk1[4], kk1[5]);
    OLED_ShowString(0, 20, s, 12);
    OLED_Refresh_Gram();
    while(1){
        OSTimeDly(300, OS_OPT_TIME_DLY, &err);
        sprintf(s, "%d", i++);
        GPIOC->ODR ^= GPIO_PIN_5;
    }
}

void MDMA_Init(){
  __HAL_RCC_MDMA_CLK_ENABLE();
  /* Local variables */

  /* Configure MDMA channel MDMA_Channel0 */
  /* Configure MDMA request hmdma_mdma_channel40_sw_0 on MDMA_Channel0 */
  hmdma_mdma_channel40_sw_0.Instance = MDMA_Channel0;
  hmdma_mdma_channel40_sw_0.Init.Request = MDMA_REQUEST_SW;
  hmdma_mdma_channel40_sw_0.Init.TransferTriggerMode = MDMA_FULL_TRANSFER;
  hmdma_mdma_channel40_sw_0.Init.Priority = MDMA_PRIORITY_VERY_HIGH;
  hmdma_mdma_channel40_sw_0.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hmdma_mdma_channel40_sw_0.Init.SourceInc = MDMA_SRC_INC_HALFWORD;
  hmdma_mdma_channel40_sw_0.Init.DestinationInc = MDMA_DEST_INC_WORD;
  hmdma_mdma_channel40_sw_0.Init.SourceDataSize = MDMA_SRC_DATASIZE_HALFWORD;
  hmdma_mdma_channel40_sw_0.Init.DestDataSize = MDMA_DEST_DATASIZE_HALFWORD;
  hmdma_mdma_channel40_sw_0.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
  hmdma_mdma_channel40_sw_0.Init.BufferTransferLength = 2;
  hmdma_mdma_channel40_sw_0.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
  hmdma_mdma_channel40_sw_0.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
  hmdma_mdma_channel40_sw_0.Init.SourceBlockAddressOffset = 0;
  hmdma_mdma_channel40_sw_0.Init.DestBlockAddressOffset = 0;
  if (HAL_MDMA_Init(&hmdma_mdma_channel40_sw_0) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure MDMA channel MDMA_Channel1 */
  /* Configure MDMA request hmdma_mdma_channel41_sw_0 on MDMA_Channel1 */
  hmdma_mdma_channel41_sw_0.Instance = MDMA_Channel1;
  hmdma_mdma_channel41_sw_0.Init.Request = MDMA_REQUEST_SW;
  hmdma_mdma_channel41_sw_0.Init.TransferTriggerMode = MDMA_FULL_TRANSFER;
  hmdma_mdma_channel41_sw_0.Init.Priority = MDMA_PRIORITY_VERY_HIGH;
  hmdma_mdma_channel41_sw_0.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hmdma_mdma_channel41_sw_0.Init.SourceInc = MDMA_SRC_INC_WORD;
  hmdma_mdma_channel41_sw_0.Init.DestinationInc = MDMA_DEST_INC_HALFWORD;
  hmdma_mdma_channel41_sw_0.Init.SourceDataSize = MDMA_SRC_DATASIZE_HALFWORD;
  hmdma_mdma_channel41_sw_0.Init.DestDataSize = MDMA_DEST_DATASIZE_HALFWORD;
  hmdma_mdma_channel41_sw_0.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
  hmdma_mdma_channel41_sw_0.Init.BufferTransferLength = 2;
  hmdma_mdma_channel41_sw_0.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
  hmdma_mdma_channel41_sw_0.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
  hmdma_mdma_channel41_sw_0.Init.SourceBlockAddressOffset = 0;
  hmdma_mdma_channel41_sw_0.Init.DestBlockAddressOffset = 0;
  if (HAL_MDMA_Init(&hmdma_mdma_channel41_sw_0) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_NVIC_SetPriority(MDMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(MDMA_IRQn);

}
