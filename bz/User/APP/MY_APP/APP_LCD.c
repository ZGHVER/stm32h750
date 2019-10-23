#include "app.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_i2c_touch.h"
#include "./touch/bsp_touch_gtxx.h"
#include "./touch/button.h"

#define Tuch_TSIZE 256
#define Tuch_Prio 10
CPU_STK Tuch_stk[Tuch_TSIZE];
OS_TCB  Tuck_TCB;
void    Tuck_Task(void* args);


void down(int32_t id,int32_t x,int32_t y,int32_t w){
	BUTTON_ActPush(id, x, y);
}

void up(int32_t id){
	BUTTON_ActUplift(id);
}

void LCD_Start(){
	OS_ERR err;
    LCD_Init();
    GTP_Init_Panel();

	LCD_SelectLayer(0);
	LCD_Clear(LCD_COLOR_WHITE);  
	LCD_SelectLayer(1);
	LCD_Clear(LCD_COLOR_LUCENCY);
	LCD_SetTransparency(0, 255);
	LCD_SetTransparency(1, 255);
	
	GTP_TouchRegisterCallBack(up, down);
	
	
	OSTaskCreate(
		(  OS_TCB*   )&Tuck_TCB,                                  //������ƿ�
		( CPU_CHAR*  )"Tuck_TCB",                                 //������
		(OS_TASK_PTR )Tuck_Task,                                  //������ָ��
		(   void*    )0,                                          //�״�����ʱ���ݵĲ���
		(  OS_PRIO   )Tuch_Prio,                                  //�������ȼ�
		(  CPU_STK*  )&Tuch_stk[0],                               //�����ջ����ַ
		(CPU_STK_SIZE)Tuch_TSIZE / 10,                            //��������ջ�ռ�
		(CPU_STK_SIZE)Tuch_TSIZE,                                 //�����ջ��С
		( OS_MSG_QTY )0,                                          //����ɽ��յ������Ϣ��
		(  OS_TICK   )0,                                          //������֮��ѭ��ʱ��ʱ��Ƭ��ʱ�������Կ̶ȱ�ʾ��ָ��0��ʹ��Ĭ��ֵ
		(   void*    )0,                                          //TCB��չָ��
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //�����й�������Ϊ��������Ϣ����ѡ�
		(  OS_ERR*   )&err                                        //�������
	);

}

void  Tuck_Task(void* args){
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

void EXTI3_IRQHandler(void)
{
	OS_ERR err;
	OSIntEnter();
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		//Goodix_TS_Work_Func();
		OSTaskSemPost(&Tuck_TCB, OS_OPT_POST_NONE, &err);
	}  
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);     //����жϱ�־λ
	OSIntExit();
}
