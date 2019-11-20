  
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
        (  OS_TCB*   )&Test_TCB,                                   //������ƿ�
        ( CPU_CHAR*  )"Test_Task",                                  //������
        (OS_TASK_PTR )Test_Task,                                        //������ָ��
        (   void*    )0,                                          //�״�����ʱ���ݵĲ���
        (  OS_PRIO   )Test_Prio,                                          //�������ȼ�
        (  CPU_STK*  )&Test_STK[0],                                  //�����ջ����ַ
        (CPU_STK_SIZE)Test_TSIZE / 10,                            //��������ջ�ռ�
        (CPU_STK_SIZE)Test_TSIZE,                                 //�����ջ��С
        ( OS_MSG_QTY )10,                                         //����ɽ��յ������Ϣ��
        (  OS_TICK   )0,                                          //������֮��ѭ��ʱ��ʱ��Ƭ��ʱ�������Կ̶ȱ�ʾ��ָ��0��ʹ��Ĭ��ֵ
        (   void*    )0,                                          //TCB��չָ��
        (  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //�����й�������Ϊ��������Ϣ����ѡ�
        (  OS_ERR*   )&err                                      //�������
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