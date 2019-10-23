
#include "main.h"
#include "./bsp/lcd/bsp_lcd.h"
#include "./touch/button.h"
#include "./BSP/lcd/photo1.h"
#include "./touch/bsp_touch_gtxx.h"
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
extern OS_MEM SDRAM_MEM;
int32_t lastid = -1;
BUTTON but[4];

#define Test_TSIZE 128
#define Test_Prio 15
CPU_STK Test_STK[Test_TSIZE];
OS_TCB  Test_TCB;
void    Test_Task(void* args);

#define Test1_TSIZE 128
#define Test1_Prio 15
CPU_STK Test1_STK[Test_TSIZE];
OS_TCB  Test1_TCB;
void    Test1_Task(void* args);



void dd(BUTTON* but){
 char s[10];
    sprintf(s, "%d", but->id);
    LCD_DisplayStringAtABSPos(100, 50, (unsigned char*)s);
}

int main(void){
	OS_ERR err;
	
  	SystemClock_Config();	
	//BSP_SystemClkCfg();
  	HAL_Init();
	BSP_Init();
	CPU_Init();
	SDRAM_Init();
	BUTTON_SetBorderColor(LCD_COLOR_BROWN);
	OSInit(&err);
	LCD_Start();
	BUTTON but[16];
	osError_hander(err);
	
	Button_KeyBoard_Init(but, 400, 100, 80);

	OSTaskCreate(
		(  OS_TCB*   )&Test_TCB,                                   //������ƿ�
		( CPU_CHAR*  )"test",                                      //������
		(OS_TASK_PTR )Test_Task,                                  //������ָ��
		(   void*    )0,                                          //�״�����ʱ���ݵĲ���
		(  OS_PRIO   )Test_Prio,                                  //�������ȼ�
		(  CPU_STK*  )&Test_STK[0],                               //�����ջ����ַ
		(CPU_STK_SIZE)Test_TSIZE / 10,                            //��������ջ�ռ�
		(CPU_STK_SIZE)Test_TSIZE,                                 //�����ջ��С
		( OS_MSG_QTY )10,                                         //����ɽ��յ������Ϣ��
		(  OS_TICK   )0,                                          //������֮��ѭ��ʱ��ʱ��Ƭ��ʱ�������Կ̶ȱ�ʾ��ָ��0��ʹ��Ĭ��ֵ
		(   void*    )0,                                          //TCB��չָ��
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //�����й�������Ϊ��������Ϣ����ѡ�
		(  OS_ERR*   )&err                                        //�������
	);

	OSTaskCreate(
		(  OS_TCB*   )&Test1_TCB,                                   //������ƿ�
		( CPU_CHAR*  )"Test1_Task",                                 //������
		(OS_TASK_PTR )Test1_Task,                                   //������ָ��
		(   void*    )0,                                            //�״�����ʱ���ݵĲ���
		(  OS_PRIO   )Test1_Prio,                                   //�������ȼ�
		(  CPU_STK*  )&Test1_STK[0],                                //�����ջ����ַ
		(CPU_STK_SIZE)Test1_TSIZE / 10,                             //��������ջ�ռ�
		(CPU_STK_SIZE)Test1_TSIZE,                                  //�����ջ��С
		( OS_MSG_QTY )12,                                           //����ɽ��յ������Ϣ��
		(  OS_TICK   )0,                                            //������֮��ѭ��ʱ��ʱ��Ƭ��ʱ�������Կ̶ȱ�ʾ��ָ��0��ʹ��Ĭ��ֵ
		(   void*    )0,                                            //TCB��չָ��
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,    //�����й�������Ϊ��������Ϣ����ѡ�
		(  OS_ERR*   )&err                                          //�������
	);

		OSStart(&err);
	uint32_t k;
	while(1){	
		k ++;
  };
}

void Test_Task(void* args){
	OS_ERR err;
	while(1){
		LCD_SetTextColor(LCD_COLOR_YELLOW);
		LCD_FillRect(100, 100, 50, 50);
		OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
		LCD_SetTextColor(LCD_COLOR_BLUE);
		LCD_FillRect(100, 100, 50, 50);
		OSTimeDly(400, OS_OPT_TIME_DLY, &err);
	}
}

void Test1_Task(void* args){
	OS_ERR err;
	while(1){
		LCD_SetTextColor(LCD_COLOR_RED);
		LCD_FillRect(150, 100, 50, 50);
		OSTimeDly(400, OS_OPT_TIME_DLY, &err);
		LCD_SetTextColor(LCD_COLOR_GREEN);
		LCD_FillRect(150, 100, 50, 50);
		OSTimeDly(200, OS_OPT_TIME_DLY, &err);
	}
}



