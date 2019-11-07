#include "app.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_i2c_touch.h"
#include "./touch/bsp_touch_gtxx.h"
#include "./touch/button.h"

#define Touch_TSIZE 256
#define Touch_Prio 10
CPU_STK Touch_stk[Touch_TSIZE];
OS_TCB  Touck_TCB;
void    Touck_Task(void* args);

#define TouchUp_TSIZE 128
#define TouchUp_Prio 12
CPU_STK TouchUp_STK[TouchUp_TSIZE];
OS_TCB  TouchUp_TCB;
void    TouchUp_Task(void* args);

#define TouchDown_TSIZE 128
#define TouchDown_Prio 12
CPU_STK TouchDown_STK[TouchDown_TSIZE];
OS_TCB  TouchDown_TCB;
void    TouchDown_Task(void* args);

OS_MEM 	MEM_SI128;

#define SI128_BlockNum 100
#define SI128_BlockSize 16
uint8_t mem[SI128_BlockNum][SI128_BlockSize];

void down(int32_t id,int32_t x,int32_t y,int32_t w);
void up(int32_t id);

void LCD_Start(){
	OS_ERR err;
    LCD_Init();

	LCD_SelectLayer(0);
	LCD_Clear(LCD_COLOR_WHITE);  
	LCD_SelectLayer(1);
	LCD_Clear(LCD_COLOR_LUCENCY);
	LCD_SetTransparency(0, 255);
	LCD_SetTransparency(1, 255);
	
    GTP_Init_Panel();
	GTP_TouchRegisterCallBack(up, down);
	
	//������������
	OSTaskCreate( 
		(  OS_TCB*   )&Touck_TCB,                                 //������ƿ�
		( CPU_CHAR*  )"Touck_TCB",                                //������
		(OS_TASK_PTR )Touck_Task,                                 //������ָ��
		(   void*    )0,                                          //�״�����ʱ���ݵĲ���
		(  OS_PRIO   )Touch_Prio,                                 //�������ȼ�
		(  CPU_STK*  )&Touch_stk[0],                              //�����ջ����ַ
		(CPU_STK_SIZE)Touch_TSIZE / 10,                           //��������ջ�ռ�
		(CPU_STK_SIZE)Touch_TSIZE,                                //�����ջ��С
		( OS_MSG_QTY )0,                                          //����ɽ��յ������Ϣ��
		(  OS_TICK   )10,                                         //������֮��ѭ��ʱ��ʱ��Ƭ��ʱ�������Կ̶ȱ�ʾ��ָ��0��ʹ��Ĭ��ֵ
		(   void*    )0,                                          //TCB��չָ��
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //�����й�������Ϊ��������Ϣ����ѡ�
		(  OS_ERR*   )&err                                        //�������
	);
	//����̧����
	OSTaskCreate(
		(  OS_TCB*   )&TouchUp_TCB,                               //������ƿ�
		( CPU_CHAR*  )"TouchUp_Task",                             //������
		(OS_TASK_PTR )TouchUp_Task,                               //������ָ��
		(   void*    )0,                                          //�״�����ʱ���ݵĲ���
		(  OS_PRIO   )TouchUp_Prio,                               //�������ȼ�
		(  CPU_STK*  )&TouchUp_STK[0],                            //�����ջ����ַ
		(CPU_STK_SIZE)TouchUp_TSIZE / 10,                         //��������ջ�ռ�
		(CPU_STK_SIZE)TouchUp_TSIZE,                              //�����ջ��С
		( OS_MSG_QTY )10,                                         //����ɽ��յ������Ϣ��
		(  OS_TICK   )0,                                          //������֮��ѭ��ʱ��ʱ��Ƭ��ʱ�������Կ̶ȱ�ʾ��ָ��0��ʹ��Ĭ��ֵ
		(   void*    )0,                                          //TCB��չָ��
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //�����й�������Ϊ��������Ϣ����ѡ�
		(  OS_ERR*   )&err                                        //�������
	);
	//�������´���
	OSTaskCreate(
		(  OS_TCB*   )&TouchDown_TCB,                             //������ƿ�
		( CPU_CHAR*  )"TouchDown_Task",                           //������
		(OS_TASK_PTR )TouchDown_Task,                             //������ָ��
		(   void*    )0,                                          //�״�����ʱ���ݵĲ���
		(  OS_PRIO   )TouchDown_Prio,                             //�������ȼ�
		(  CPU_STK*  )&TouchDown_STK[0],                          //�����ջ����ַ
		(CPU_STK_SIZE)TouchDown_TSIZE / 10,                       //��������ջ�ռ�
		(CPU_STK_SIZE)TouchDown_TSIZE,                            //�����ջ��С
		( OS_MSG_QTY )10,                                         //����ɽ��յ������Ϣ��
		(  OS_TICK   )0,                                          //������֮��ѭ��ʱ��ʱ��Ƭ��ʱ�������Կ̶ȱ�ʾ��ָ��0��ʹ��Ĭ��ֵ
		(   void*    )0,                                          //TCB��չָ��
		(  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //�����й�������Ϊ��������Ϣ����ѡ�
		(  OS_ERR*   )&err                                        //�������
	);

	OSMemCreate(&MEM_SI128, 
				"block size 128",
				(void* )mem,
				SI128_BlockNum,
				SI128_BlockSize,
				&err
	);				 
}

void TouchUp_Task(void* args){
	OS_ERR       err;
	int32_t*     get;
	OS_MSG_SIZE  size;

	while(1){
		get = (int32_t*)OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &size, (void*)0, &err);
		osError_hander(err);
		BUTTON_ActUplift(*get);
		OSMemPut(&MEM_SI128, get, &err);
		osError_hander(err);
	}
}

void TouchDown_Task(void* args){
	OS_ERR       err;
	int32_t*     get;
	OS_MSG_SIZE  size;

	while(1){
		get = (int32_t*)OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &size, (void*)0, &err);
		osError_hander(err);
		BUTTON_ActPush(get[0], get[1], get[2]);
		OSMemPut(&MEM_SI128, get, &err);
		osError_hander(err);
	}
}

void down(int32_t id,int32_t x,int32_t y,int32_t w){
	OS_ERR err;
	int32_t* ret = OSMemGet(&MEM_SI128, &err);
	osError_hander(err);
	ret[0] = id;
	ret[1] = x;
	ret[2] = y;
	OSTaskQPost(&TouchDown_TCB, ret, 12, OS_OPT_POST_FIFO, &err);
	osError_hander(err);
}

void up(int32_t id){
	OS_ERR err;
	int32_t* ret = OSMemGet(&MEM_SI128, &err);
	osError_hander(err);
	*ret = id;
	OSTaskQPost(&TouchUp_TCB, ret, 4, OS_OPT_POST_FIFO, &err);
	osError_hander(err); 
}

void Touck_Task(void* args){
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

void EXTI3_IRQHandler(void){
	OS_ERR err;
	OSIntEnter();
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET){ 
		OSTaskSemPost(&Touck_TCB, OS_OPT_POST_NONE, &err);
	}  
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);     
	OSIntExit();
}
