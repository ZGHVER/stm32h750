  
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
#include "GUIDEMO.h"

extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter1;
extern DMA_HandleTypeDef hdma_dfsdm1_flt0;
extern DMA_HandleTypeDef hdma_dfsdm1_flt1;
char SDPath[4];
FATFS fs;


#define Test_TSIZE 128
#define Test_Prio 15
CPU_STK Test_STK[Test_TSIZE];
OS_TCB  Test_TCB;
void    Test_Task(void* args);

int main(){
    OS_ERR err;
	SystemClock_Config1();
    BSP_Init();
    CPU_Init();

    OSInit(&err);
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
    
    OSStart(&err);

    while(1);

}

void    Test_Task(void* args){
   MLCD_Start();
    HAL_NVIC_DisableIRQ(SDMMC1_IRQn);
    FATFS_LinkDriver(&SD_Driver, SDPath);
    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
    FRESULT res_sd = f_mount(&fs,"0:",1);  
	if(res_sd == FR_NO_FILESYSTEM){
        res_sd=f_mkfs("0:",0,0);	
        MLCD_Clear(MLCD_COLOR_BLUE);
      if(res_sd == FR_OK)
      {
          MLCD_Clear(MLCD_COLOR_BLACK);
      }
    }
    else
			MLCD_Clear(MLCD_COLOR_BLACK);
    while(1);
  while(1);
}