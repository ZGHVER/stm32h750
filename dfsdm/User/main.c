  
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


	OSTaskCreate(
		(  OS_TCB*   )&Task_TCB,                                   //������ƿ�
		( CPU_CHAR*  )"Task_Task",                                  //������
		(OS_TASK_PTR )Task_Task,                                        //������ָ��
		(   void*    )0,                                          //�״�����ʱ���ݵĲ���
		(  OS_PRIO   )Task_Prio,                                          //�������ȼ�
		(  CPU_STK*  )&Task_STK[0],                                  //�����ջ����ַ
		(CPU_STK_SIZE)Task_TSIZE / 10,                            //��������ջ�ռ�
		(CPU_STK_SIZE)Task_TSIZE,                                 //�����ջ��С
		( OS_MSG_QTY )10,                                         //����ɽ��յ������Ϣ��
		(  OS_TICK   )0,                                          //������֮��ѭ��ʱ��ʱ��Ƭ��ʱ�������Կ̶ȱ�ʾ��ָ��0��ʹ��Ĭ��ֵ
		(   void*    )0,                                          //TCB��չָ��
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //�����й�������Ϊ��������Ϣ����ѡ�
		(  OS_ERR*   )&err                                      //�������
	);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseInitTypeDef tim1_BaseInit;
	tim1_BaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
	tim1_BaseInit.TIM_CounterMode = TIM_CounterMode_Up;      //������ģʽ
	tim1_BaseInit.TIM_Period = 10;                         //��װ��ֵ
	tim1_BaseInit.TIM_Prescaler = 1000;                   //��Ƶϵ��
	TIM_TimeBaseInit(TIM1, &tim1_BaseInit);

	TIM_OCInitTypeDef TIM1_oc2_Init;                            //��ʱ��1Ƶ��2PWM��ʼ��
	TIM1_oc2_Init.TIM_OCMode = TIM_OCMode_PWM1;                 //PWMģʽѡ��
	TIM1_oc2_Init.TIM_OCPolarity = TIM_OCPolarity_High;         //PWM��Ч��ƽ����
	TIM1_oc2_Init.TIM_OutputState = TIM_OutputState_Enable;     //����Ƚ�״̬
	TIM_OC2Init(TIM1, &TIM1_oc2_Init);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_SetCompare2(TIM1, 500);
	

}

void Tast_Task(void* args){

}