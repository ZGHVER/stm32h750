  
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
        (  OS_TCB*   )&Init_TCB,                                  //������ƿ�
        ( CPU_CHAR*  )"Init_Task",                                //������
        (OS_TASK_PTR )Init_Task,                                  //������ָ��
        (   void*    )0,                                          //�״�����ʱ���ݵĲ���
        (  OS_PRIO   )Init_Prio,                                  //�������ȼ�
        (  CPU_STK*  )&Init_STK[0],                               //�����ջ����ַ
        (CPU_STK_SIZE)Init_TSIZE / 10,                            //��������ջ�ռ�
        (CPU_STK_SIZE)Init_TSIZE,                                 //�����ջ��С
        ( OS_MSG_QTY )10,                                         //����ɽ��յ������Ϣ��
        (  OS_TICK   )0,                                          //������֮��ѭ��ʱ��ʱ��Ƭ��ʱ�������Կ̶ȱ�ʾ��ָ��0��ʹ��Ĭ��ֵ
        (   void*    )0,                                          //TCB��չָ��
        (  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //�����й�������Ϊ��������Ϣ����ѡ�
        (  OS_ERR*   )&err                                        //�������
    );
   
    OSTaskCreate(
        (  OS_TCB*   )&LED_TCB,                                   //������ƿ�
        ( CPU_CHAR*  )"LED_Task",                                 //������
        (OS_TASK_PTR )LED_Task,                                   //������ָ��
        (   void*    )0,                                          //�״�����ʱ���ݵĲ���
        (  OS_PRIO   )LED_Prio,                                   //�������ȼ�
        (  CPU_STK*  )&LED_STK[0],                                //�����ջ����ַ
        (CPU_STK_SIZE)LED_TSIZE / 10,                             //��������ջ�ռ�
        (CPU_STK_SIZE)LED_TSIZE,                                  //�����ջ��С
        ( OS_MSG_QTY )10,                                         //����ɽ��յ������Ϣ��
        (  OS_TICK   )0,                                          //������֮��ѭ��ʱ��ʱ��Ƭ��ʱ�������Կ̶ȱ�ʾ��ָ��0��ʹ��Ĭ��ֵ
        (   void*    )0,                                          //TCB��չָ��
        (  OS_OPT    )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,  //�����й�������Ϊ��������Ϣ����ѡ�
        (  OS_ERR*   )&err                                        //�������
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
