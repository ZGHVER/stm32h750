#include "sdram.h"
#include "delay.h"


void SDRAM_Check(void);
uint32_t RadomBuffer[10000];

uint32_t ReadBuffer[10000];

#define SDRAM_SIZE     (W9825G6KH_SIZE/4)

uint32_t *pSDRAM;

long long count=0,sdram_count=0;

RNG_HandleTypeDef hrng;

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//SDRAM��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
SDRAM_HandleTypeDef SDRAM_Handler;   //SDRAM���
uint8_t SDRAM_Test(void);


static FMC_SDRAM_CommandTypeDef Command;
SDRAM_HandleTypeDef hsdram1;
#define sdramHandle hsdram1
/**
  * @brief  �ӳ�һ��ʱ��
  * @param  �ӳٵ�ʱ�䳤��
  * @retval None
  */
static void SDRAM_delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}

/**
  * @brief  ��ʼ������SDRAM��IO
  * @param  ��
  * @retval ��
  */
static void SDRAM_GPIO_Config(void)
{		
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ʹ��SDRAM��ص�GPIOʱ�� */

  /*��ַ�ź���*/
  FMC_A0_GPIO_CLK;  FMC_A1_GPIO_CLK;  FMC_A2_GPIO_CLK;
  FMC_A3_GPIO_CLK;FMC_A4_GPIO_CLK;FMC_A5_GPIO_CLK;
  FMC_A6_GPIO_CLK; FMC_A7_GPIO_CLK; FMC_A8_GPIO_CLK;
  FMC_A9_GPIO_CLK; FMC_A10_GPIO_CLK;FMC_A11_GPIO_CLK; 
  FMC_A12_GPIO_CLK;
  /*�����ź���*/
  FMC_D0_GPIO_CLK; FMC_D1_GPIO_CLK ; FMC_D2_GPIO_CLK ; 
  FMC_D3_GPIO_CLK ; FMC_D4_GPIO_CLK ; FMC_D5_GPIO_CLK ;
  FMC_D6_GPIO_CLK; FMC_D7_GPIO_CLK ; FMC_D8_GPIO_CLK ;
  FMC_D9_GPIO_CLK ;FMC_D10_GPIO_CLK; FMC_D11_GPIO_CLK;
  FMC_D12_GPIO_CLK; FMC_D13_GPIO_CLK; FMC_D14_GPIO_CLK;
  FMC_D15_GPIO_CLK;  
  /*�����ź���*/
  FMC_CS_GPIO_CLK ; FMC_BA0_GPIO_CLK; FMC_BA1_GPIO_CLK ;
  FMC_WE_GPIO_CLK ; FMC_RAS_GPIO_CLK ; FMC_CAS_GPIO_CLK;
  FMC_CLK_GPIO_CLK ; FMC_CKE_GPIO_CLK; FMC_UDQM_GPIO_CLK;
  FMC_LDQM_GPIO_CLK;


  
  /*-- GPIO ���� -----------------------------------------------------*/

  /* ͨ�� GPIO ���� */       
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;//����Ϊ���ù���
  GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Alternate = GPIO_AF12_FMC;
  
  
  /*��ַ�ź��� �����������*/
  GPIO_InitStructure.Pin = FMC_A0_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A0_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A1_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A1_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A2_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A2_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A3_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A3_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A4_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A4_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A5_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A5_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_A6_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A6_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_A7_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A7_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_A8_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A8_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_A9_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A9_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_A10_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A10_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_A11_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A11_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A12_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A12_GPIO_PORT, &GPIO_InitStructure);

  
  
  /*�����ź��� �����������*/
  GPIO_InitStructure.Pin = FMC_D0_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D0_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D1_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D1_GPIO_PORT, &GPIO_InitStructure);

    
  GPIO_InitStructure.Pin = FMC_D2_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D2_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D3_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D3_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D4_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D4_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D5_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D5_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D6_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D6_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D7_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D7_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D8_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D8_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D9_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D9_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D10_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D10_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D11_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D11_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D12_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D12_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D13_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D13_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D14_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D14_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D15_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D15_GPIO_PORT, &GPIO_InitStructure);

  
  /*�����ź���*/
  GPIO_InitStructure.Pin = FMC_CS_GPIO_PIN; 
  HAL_GPIO_Init(FMC_CS_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_BA0_GPIO_PIN; 
  HAL_GPIO_Init(FMC_BA0_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_BA1_GPIO_PIN;
  HAL_GPIO_Init(FMC_BA1_GPIO_PORT, &GPIO_InitStructure);

    
  GPIO_InitStructure.Pin = FMC_WE_GPIO_PIN; 
  HAL_GPIO_Init(FMC_WE_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_RAS_GPIO_PIN; 
  HAL_GPIO_Init(FMC_RAS_GPIO_PORT, &GPIO_InitStructure);

    
  GPIO_InitStructure.Pin = FMC_CAS_GPIO_PIN; 
  HAL_GPIO_Init(FMC_CAS_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_CLK_GPIO_PIN; 
  HAL_GPIO_Init(FMC_CLK_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_CKE_GPIO_PIN; 
  HAL_GPIO_Init(FMC_CKE_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_UDQM_GPIO_PIN; 
  HAL_GPIO_Init(FMC_UDQM_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_LDQM_GPIO_PIN; 
  HAL_GPIO_Init(FMC_LDQM_GPIO_PORT, &GPIO_InitStructure);

		
}
/**
  * @brief  ��SDRAMоƬ���г�ʼ������
  * @param  None. 
  * @retval None.
  */
static void SDRAM_InitSequence(void)
{
  uint32_t tmpr = 0;
  
/* Step 3 --------------------------------------------------------------------*/
  /* ������������ṩ��SDRAM��ʱ�� */
  Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;
/* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

  /* Step 2: Insert 100 us minimum delay */ 
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  SDRAM_delay(1);
    
/* Step 5 --------------------------------------------------------------------*/
  /* ������������е�bankԤ��� */ 
  Command.CommandMode = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;
/* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);   
  
/* Step 6 --------------------------------------------------------------------*/
  /* ��������Զ�ˢ�� */   
  Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 4;
  Command.ModeRegisterDefinition = 0;
 /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);
  
/* Step 7 --------------------------------------------------------------------*/
  /* ����sdram�Ĵ������� */
  tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2          |
                   SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                   SDRAM_MODEREG_CAS_LATENCY_3           |
                   SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                   SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  
  /* �����������SDRAM�Ĵ��� */
  Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = tmpr;
  /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);
  
/* Step 8 --------------------------------------------------------------------*/

  /* ����ˢ�¼����� */
  /* (7.8125 us x Freq) - 20 */
	/* Step 6: Set the refresh rate counter */
  /* Set the device refresh rate */
  HAL_SDRAM_ProgramRefreshRate(&sdramHandle, 824); 
//  FMC_SetRefreshCount(1386);
//  /* ������������*/
//  while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET)
//  {
//  }
}

//SDRAM��ʼ��
void SDRAM_Init(void)
{
	FMC_SDRAM_TimingTypeDef SdramTiming;
//    u32 temp=0;
//    FMC_SDRAM_TimingTypeDef SDRAM_Timing;
//                                                     
//    SDRAM_Handler.Instance=FMC_Bank5_6;                                  //SDRAM��BANK5,6  
//    SDRAM_Handler.Init.SDBank=FMC_SDRAM_BANK2;                           //SDRAM����BANK6��
//    SDRAM_Handler.Init.ColumnBitsNumber=FMC_SDRAM_COLUMN_BITS_NUM_9;     //������
//    SDRAM_Handler.Init.RowBitsNumber=FMC_SDRAM_ROW_BITS_NUM_13;          //������
//    SDRAM_Handler.Init.MemoryDataWidth=FMC_SDRAM_MEM_BUS_WIDTH_16;       //���ݿ��Ϊ16λ
//    SDRAM_Handler.Init.InternalBankNumber=FMC_SDRAM_INTERN_BANKS_NUM_4;  //һ��4��BANK
//    SDRAM_Handler.Init.CASLatency=FMC_SDRAM_CAS_LATENCY_3;               //CASΪ3
//    SDRAM_Handler.Init.WriteProtection=FMC_SDRAM_WRITE_PROTECTION_DISABLE;//ʧ��д����
//    SDRAM_Handler.Init.SDClockPeriod=FMC_SDRAM_CLOCK_PERIOD_2;           //SDRAMʱ��ΪHCLK/2=216M/2=108M=9.3ns
//    SDRAM_Handler.Init.ReadBurst=FMC_SDRAM_RBURST_DISABLE;                //ʹ��ͻ��
//    SDRAM_Handler.Init.ReadPipeDelay=FMC_SDRAM_RPIPE_DELAY_1;            //��ͨ����ʱ
//    
//    SDRAM_Timing.LoadToActiveDelay=2;                                   //����ģʽ�Ĵ���������ʱ����ӳ�Ϊ2��ʱ������
//    SDRAM_Timing.ExitSelfRefreshDelay=7;                                //�˳���ˢ���ӳ�Ϊ8��ʱ������
//    SDRAM_Timing.SelfRefreshTime=4;                                     //��ˢ��ʱ��Ϊ6��ʱ������                                 
//    SDRAM_Timing.RowCycleDelay=7;                                       //��ѭ���ӳ�Ϊ6��ʱ������
//    SDRAM_Timing.WriteRecoveryTime=2;                                   //�ָ��ӳ�Ϊ2��ʱ������
//    SDRAM_Timing.RPDelay=2;                                             //��Ԥ����ӳ�Ϊ2��ʱ������
//    SDRAM_Timing.RCDDelay=2;                                            //�е����ӳ�Ϊ2��ʱ������
//    HAL_SDRAM_Init(&SDRAM_Handler,&SDRAM_Timing);  
//    
//		
//		
//		
//		
//    //SDRAM��������ʼ������Ժ���Ҫ��������˳���ʼ��SDRAM
//    SDRAM_Send_Cmd(1,FMC_SDRAM_CMD_CLK_ENABLE,1,0); //ʱ������ʹ��
//    delay_us(500);                                  //������ʱ200us
//    SDRAM_Send_Cmd(1,FMC_SDRAM_CMD_PALL,1,0);       //�����д洢��Ԥ���
//    SDRAM_Send_Cmd(1,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0);//������ˢ�´��� 
//    //����ģʽ�Ĵ���,SDRAM��bit0~bit2Ϊָ��ͻ�����ʵĳ��ȣ�
//	//bit3Ϊָ��ͻ�����ʵ����ͣ�bit4~bit6ΪCASֵ��bit7��bit8Ϊ����ģʽ
//	//bit9Ϊָ����дͻ��ģʽ��bit10��bit11λ����λ
//	temp=(u32)SDRAM_MODEREG_BURST_LENGTH_1          |	//����ͻ������:1(������1/2/4/8)
//              SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |	//����ͻ������:����(����������/����)
//              SDRAM_MODEREG_CAS_LATENCY_3           |	//����CASֵ:3(������2/3)
//              SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //���ò���ģʽ:0,��׼ģʽ
//              SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //����ͻ��дģʽ:1,�������
//    SDRAM_Send_Cmd(1,FMC_SDRAM_CMD_LOAD_MODE,1,temp);   //����SDRAM��ģʽ�Ĵ���
//    
//    //ˢ��Ƶ�ʼ�����(��SDCLKƵ�ʼ���),���㷽��:
//	//COUNT=SDRAMˢ������/����-20=SDRAMˢ������(us)*SDCLKƵ��(Mhz)/����
//    //����ʹ�õ�SDRAMˢ������Ϊ64ms,SDCLK=216/2=108Mhz,����Ϊ8192(2^13).
//	//����,COUNT=64*1000*108/8192-20=823
//	HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handler,824);
	
	
	
	
	SDRAM_GPIO_Config();
	
	/* ʹ�� FMC ʱ�� */
	__HAL_RCC_FMC_CLK_ENABLE();
	
	 hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 2;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  HAL_SDRAM_Init(&hsdram1, &SdramTiming);  
  /* FMC SDRAM device initialization sequence */
  SDRAM_InitSequence(); 
	
	
	
//	SDRAM_Test();
//	
//	/*ʹ��RNGʱ��*/
//    __RNG_CLK_ENABLE();
//	/*��ʼ��RNGģ����������*/
//    hrng.Instance = RNG;
//    HAL_RNG_Init(&hrng);
//		
//	for(count=0;count<10000;count++)
//	{
//			RadomBuffer[count]=HAL_RNG_GetRandomNumber(&hrng);
//	} 
//	SDRAM_Check();
	
}

//SDRAM�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_SDRAM_Init()����
//hsdram:SDRAM���
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_FMC_CLK_ENABLE();                 //ʹ��FMCʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();               //ʹ��GPIOCʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE();               //ʹ��GPIODʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE();               //ʹ��GPIOEʱ��
    __HAL_RCC_GPIOF_CLK_ENABLE();               //ʹ��GPIOFʱ��
    __HAL_RCC_GPIOG_CLK_ENABLE();               //ʹ��GPIOGʱ��
    
    //��ʼ��PC0
    GPIO_Initure.Pin=GPIO_PIN_0;  
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //���츴��
    GPIO_Initure.Pull=GPIO_PULLUP;              //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //����
    GPIO_Initure.Alternate=GPIO_AF12_FMC;       //����ΪFMC    
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //��ʼ��
    
    //��ʼ��PD0,1,8,9,10,14,15
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9|\
                     GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOD,&GPIO_Initure); //��ʼ�� 
    
    //��ʼ��PE0,1,7,8,9,10,11,12,13,14,15
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|\
                     GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOE,&GPIO_Initure); //��ʼ��
    
    //��ʼ��PF0,1,2,3,4,5,11,12,13,14,15
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|\
                     GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOF,&GPIO_Initure); //��ʼ��
    
    //��ʼ��PG0,1,2,4,5,8,15
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|\
                     GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);  //��ʼ��  
		
    //��ʼ��PH6,7
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;              
    HAL_GPIO_Init(GPIOH,&GPIO_Initure); //��ʼ�� 

}

//��SDRAM��������
//bankx:0,��BANK5�����SDRAM����ָ��
//      1,��BANK6�����SDRAM����ָ��
//cmd:ָ��(0,����ģʽ/1,ʱ������ʹ��/2,Ԥ������д洢��/3,�Զ�ˢ��/4,����ģʽ�Ĵ���/5,��ˢ��/6,����)
//refresh:��ˢ�´���
//regval:ģʽ�Ĵ����Ķ���
//����ֵ:0,����;1,ʧ��.
u8 SDRAM_Send_Cmd(u8 bankx,u8 cmd,u8 refresh,u16 regval)
{
    u32 target_bank=0;
    FMC_SDRAM_CommandTypeDef Command;
    
    if(bankx==0) target_bank=FMC_SDRAM_CMD_TARGET_BANK1;       
    else if(bankx==1) target_bank=FMC_SDRAM_CMD_TARGET_BANK2;   
    Command.CommandMode=cmd;                //����
    Command.CommandTarget=target_bank;      //Ŀ��SDRAM�洢����
    Command.AutoRefreshNumber=refresh;      //��ˢ�´���
    Command.ModeRegisterDefinition=regval;  //Ҫд��ģʽ�Ĵ�����ֵ
    if(HAL_SDRAM_SendCommand(&SDRAM_Handler,&Command,0X1000)==HAL_OK) //��SDRAM��������
    {
        return 0;  
    }
    else return 1;    
}

//��ָ����ַ(WriteAddr+Bank5_SDRAM_ADDR)��ʼ,����д��n���ֽ�.
//pBuffer:�ֽ�ָ��
//WriteAddr:Ҫд��ĵ�ַ
//n:Ҫд����ֽ���
void FMC_SDRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*(vu8*)(Bank5_SDRAM_ADDR+WriteAddr)=*pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}

//��ָ����ַ((WriteAddr+Bank5_SDRAM_ADDR))��ʼ,��������n���ֽ�.
//pBuffer:�ֽ�ָ��
//ReadAddr:Ҫ��������ʼ��ַ
//n:Ҫд����ֽ���
void FMC_SDRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*pBuffer++=*(vu8*)(Bank5_SDRAM_ADDR+ReadAddr);
		ReadAddr++;
	}
}

/**
  * @brief  ����SDRAM�Ƿ����� 
  * @param  None
  * @retval ��������1���쳣����0
  */
uint8_t SDRAM_Test(void)
{
  /*д�����ݼ�����*/
  uint32_t counter=0;
  
  /* 8λ������ */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16λ������ */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  /*��8λ��ʽ��д���ݣ���У��*/
  
  /* ��SDRAM����ȫ������Ϊ0 ��W9825G6KH_SIZE����8λΪ��λ�� */
  for (counter = 0x00; counter < W9825G6KH_SIZE; counter++)
  {
    *(__IO uint8_t*) (Bank5_SDRAM_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* ������SDRAMд������  8λ */
  for (counter = 0; counter < W9825G6KH_SIZE; counter++)
  {
    *(__IO uint8_t*) (Bank5_SDRAM_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* ��ȡ SDRAM ���ݲ����*/
  for(counter = 0; counter<W9825G6KH_SIZE;counter++ )
  {
    ubReaddata_8b = *(__IO uint8_t*)(Bank5_SDRAM_ADDR + counter);  //�Ӹõ�ַ��������
    
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      SDRAM_ERROR("8λ���ݶ�д����");
      return 0;
    }
  }
	
  
  /*��16λ��ʽ��д���ݣ������*/
  
  /* ��SDRAM����ȫ������Ϊ0 */
  for (counter = 0x00; counter < W9825G6KH_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (Bank5_SDRAM_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* ������SDRAMд������  16λ */
  for (counter = 0; counter < W9825G6KH_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (Bank5_SDRAM_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* ��ȡ SDRAM ���ݲ����*/
  for(counter = 0; counter<W9825G6KH_SIZE/2;counter++ )
  {
    uhReaddata_16b = *(__IO uint16_t*)(Bank5_SDRAM_ADDR + 2*counter);  //�Ӹõ�ַ��������
    
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      SDRAM_ERROR("16λ���ݶ�д����");

      return 0;
    }
  }

  
  SDRAM_INFO("SDRAM��д����������"); 
  /*���������return 1 */
  return 1;
  

}


void SDRAM_Check(void)
{
  pSDRAM=(uint32_t*)SDRAM_BANK_ADDR;
	count=0;
	printf("��ʼд��SDRAM\r\n");
	for(sdram_count=0;sdram_count<SDRAM_SIZE;sdram_count++)
	{
		*pSDRAM=RadomBuffer[count];
		count++;
		pSDRAM++;
		if(count>=10000) 
		{
			count=0;
		}
	}
	printf("д�����ֽ���:%d\r\n",(uint32_t)pSDRAM-SDRAM_BANK_ADDR);

	count=0;
	pSDRAM=(uint32_t*)SDRAM_BANK_ADDR;
	printf("��ʼ��ȡSDRAM����ԭ������Ƚ�\r\n");
	sdram_count=0;
	for(;sdram_count<SDRAM_SIZE;sdram_count++)
	{
		if(*pSDRAM != RadomBuffer[count])
		{
			printf("���ݱȽϴ��󡪡��˳�~\r\n");
			break;
		}
		count++;
		pSDRAM++;
		if(count>=10000)
		{
			count=0;
		}
	}

	printf("�Ƚ�ͨ�����ֽ���:%d\r\n",(uint32_t)pSDRAM-SDRAM_BANK_ADDR);

	if(sdram_count == SDRAM_SIZE)
	{
		printf("SDRAM���Գɹ�\r\n");
	}
	else
	{
		printf("SDRAM����ʧ��\r\n");
	}   
}

