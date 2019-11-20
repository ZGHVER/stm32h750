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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//SDRAM驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
SDRAM_HandleTypeDef SDRAM_Handler;   //SDRAM句柄
uint8_t SDRAM_Test(void);


static FMC_SDRAM_CommandTypeDef Command;
SDRAM_HandleTypeDef hsdram1;
#define sdramHandle hsdram1
/**
  * @brief  延迟一段时间
  * @param  延迟的时间长度
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
  * @brief  初始化控制SDRAM的IO
  * @param  无
  * @retval 无
  */
static void SDRAM_GPIO_Config(void)
{		
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* 使能SDRAM相关的GPIO时钟 */

  /*地址信号线*/
  FMC_A0_GPIO_CLK;  FMC_A1_GPIO_CLK;  FMC_A2_GPIO_CLK;
  FMC_A3_GPIO_CLK;FMC_A4_GPIO_CLK;FMC_A5_GPIO_CLK;
  FMC_A6_GPIO_CLK; FMC_A7_GPIO_CLK; FMC_A8_GPIO_CLK;
  FMC_A9_GPIO_CLK; FMC_A10_GPIO_CLK;FMC_A11_GPIO_CLK; 
  FMC_A12_GPIO_CLK;
  /*数据信号线*/
  FMC_D0_GPIO_CLK; FMC_D1_GPIO_CLK ; FMC_D2_GPIO_CLK ; 
  FMC_D3_GPIO_CLK ; FMC_D4_GPIO_CLK ; FMC_D5_GPIO_CLK ;
  FMC_D6_GPIO_CLK; FMC_D7_GPIO_CLK ; FMC_D8_GPIO_CLK ;
  FMC_D9_GPIO_CLK ;FMC_D10_GPIO_CLK; FMC_D11_GPIO_CLK;
  FMC_D12_GPIO_CLK; FMC_D13_GPIO_CLK; FMC_D14_GPIO_CLK;
  FMC_D15_GPIO_CLK;  
  /*控制信号线*/
  FMC_CS_GPIO_CLK ; FMC_BA0_GPIO_CLK; FMC_BA1_GPIO_CLK ;
  FMC_WE_GPIO_CLK ; FMC_RAS_GPIO_CLK ; FMC_CAS_GPIO_CLK;
  FMC_CLK_GPIO_CLK ; FMC_CKE_GPIO_CLK; FMC_UDQM_GPIO_CLK;
  FMC_LDQM_GPIO_CLK;


  
  /*-- GPIO 配置 -----------------------------------------------------*/

  /* 通用 GPIO 配置 */       
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;//配置为复用功能
  GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Alternate = GPIO_AF12_FMC;
  
  
  /*地址信号线 针对引脚配置*/
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

  
  
  /*数据信号线 针对引脚配置*/
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

  
  /*控制信号线*/
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
  * @brief  对SDRAM芯片进行初始化配置
  * @param  None. 
  * @retval None.
  */
static void SDRAM_InitSequence(void)
{
  uint32_t tmpr = 0;
  
/* Step 3 --------------------------------------------------------------------*/
  /* 配置命令：开启提供给SDRAM的时钟 */
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
  /* 配置命令：对所有的bank预充电 */ 
  Command.CommandMode = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;
/* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);   
  
/* Step 6 --------------------------------------------------------------------*/
  /* 配置命令：自动刷新 */   
  Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 4;
  Command.ModeRegisterDefinition = 0;
 /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);
  
/* Step 7 --------------------------------------------------------------------*/
  /* 设置sdram寄存器配置 */
  tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2          |
                   SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                   SDRAM_MODEREG_CAS_LATENCY_3           |
                   SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                   SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  
  /* 配置命令：设置SDRAM寄存器 */
  Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = tmpr;
  /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);
  
/* Step 8 --------------------------------------------------------------------*/

  /* 设置刷新计数器 */
  /* (7.8125 us x Freq) - 20 */
	/* Step 6: Set the refresh rate counter */
  /* Set the device refresh rate */
  HAL_SDRAM_ProgramRefreshRate(&sdramHandle, 824); 
//  FMC_SetRefreshCount(1386);
//  /* 发送上述命令*/
//  while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET)
//  {
//  }
}

//SDRAM初始化
void SDRAM_Init(void)
{
	FMC_SDRAM_TimingTypeDef SdramTiming;
//    u32 temp=0;
//    FMC_SDRAM_TimingTypeDef SDRAM_Timing;
//                                                     
//    SDRAM_Handler.Instance=FMC_Bank5_6;                                  //SDRAM在BANK5,6  
//    SDRAM_Handler.Init.SDBank=FMC_SDRAM_BANK2;                           //SDRAM接在BANK6上
//    SDRAM_Handler.Init.ColumnBitsNumber=FMC_SDRAM_COLUMN_BITS_NUM_9;     //列数量
//    SDRAM_Handler.Init.RowBitsNumber=FMC_SDRAM_ROW_BITS_NUM_13;          //行数量
//    SDRAM_Handler.Init.MemoryDataWidth=FMC_SDRAM_MEM_BUS_WIDTH_16;       //数据宽度为16位
//    SDRAM_Handler.Init.InternalBankNumber=FMC_SDRAM_INTERN_BANKS_NUM_4;  //一共4个BANK
//    SDRAM_Handler.Init.CASLatency=FMC_SDRAM_CAS_LATENCY_3;               //CAS为3
//    SDRAM_Handler.Init.WriteProtection=FMC_SDRAM_WRITE_PROTECTION_DISABLE;//失能写保护
//    SDRAM_Handler.Init.SDClockPeriod=FMC_SDRAM_CLOCK_PERIOD_2;           //SDRAM时钟为HCLK/2=216M/2=108M=9.3ns
//    SDRAM_Handler.Init.ReadBurst=FMC_SDRAM_RBURST_DISABLE;                //使能突发
//    SDRAM_Handler.Init.ReadPipeDelay=FMC_SDRAM_RPIPE_DELAY_1;            //读通道延时
//    
//    SDRAM_Timing.LoadToActiveDelay=2;                                   //加载模式寄存器到激活时间的延迟为2个时钟周期
//    SDRAM_Timing.ExitSelfRefreshDelay=7;                                //退出自刷新延迟为8个时钟周期
//    SDRAM_Timing.SelfRefreshTime=4;                                     //自刷新时间为6个时钟周期                                 
//    SDRAM_Timing.RowCycleDelay=7;                                       //行循环延迟为6个时钟周期
//    SDRAM_Timing.WriteRecoveryTime=2;                                   //恢复延迟为2个时钟周期
//    SDRAM_Timing.RPDelay=2;                                             //行预充电延迟为2个时钟周期
//    SDRAM_Timing.RCDDelay=2;                                            //行到列延迟为2个时钟周期
//    HAL_SDRAM_Init(&SDRAM_Handler,&SDRAM_Timing);  
//    
//		
//		
//		
//		
//    //SDRAM控制器初始化完成以后还需要按照如下顺序初始化SDRAM
//    SDRAM_Send_Cmd(1,FMC_SDRAM_CMD_CLK_ENABLE,1,0); //时钟配置使能
//    delay_us(500);                                  //至少延时200us
//    SDRAM_Send_Cmd(1,FMC_SDRAM_CMD_PALL,1,0);       //对所有存储区预充电
//    SDRAM_Send_Cmd(1,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0);//设置自刷新次数 
//    //配置模式寄存器,SDRAM的bit0~bit2为指定突发访问的长度，
//	//bit3为指定突发访问的类型，bit4~bit6为CAS值，bit7和bit8为运行模式
//	//bit9为指定的写突发模式，bit10和bit11位保留位
//	temp=(u32)SDRAM_MODEREG_BURST_LENGTH_1          |	//设置突发长度:1(可以是1/2/4/8)
//              SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |	//设置突发类型:连续(可以是连续/交错)
//              SDRAM_MODEREG_CAS_LATENCY_3           |	//设置CAS值:3(可以是2/3)
//              SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //设置操作模式:0,标准模式
//              SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //设置突发写模式:1,单点访问
//    SDRAM_Send_Cmd(1,FMC_SDRAM_CMD_LOAD_MODE,1,temp);   //设置SDRAM的模式寄存器
//    
//    //刷新频率计数器(以SDCLK频率计数),计算方法:
//	//COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数
//    //我们使用的SDRAM刷新周期为64ms,SDCLK=216/2=108Mhz,行数为8192(2^13).
//	//所以,COUNT=64*1000*108/8192-20=823
//	HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handler,824);
	
	
	
	
	SDRAM_GPIO_Config();
	
	/* 使能 FMC 时钟 */
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
//	/*使能RNG时钟*/
//    __RNG_CLK_ENABLE();
//	/*初始化RNG模块产生随机数*/
//    hrng.Instance = RNG;
//    HAL_RNG_Init(&hrng);
//		
//	for(count=0;count<10000;count++)
//	{
//			RadomBuffer[count]=HAL_RNG_GetRandomNumber(&hrng);
//	} 
//	SDRAM_Check();
	
}

//SDRAM底层驱动，引脚配置，时钟使能
//此函数会被HAL_SDRAM_Init()调用
//hsdram:SDRAM句柄
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_FMC_CLK_ENABLE();                 //使能FMC时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();               //使能GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();               //使能GPIOD时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();               //使能GPIOE时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();               //使能GPIOF时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();               //使能GPIOG时钟
    
    //初始化PC0
    GPIO_Initure.Pin=GPIO_PIN_0;  
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //高速
    GPIO_Initure.Alternate=GPIO_AF12_FMC;       //复用为FMC    
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //初始化
    
    //初始化PD0,1,8,9,10,14,15
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9|\
                     GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOD,&GPIO_Initure); //初始化 
    
    //初始化PE0,1,7,8,9,10,11,12,13,14,15
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|\
                     GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOE,&GPIO_Initure); //初始化
    
    //初始化PF0,1,2,3,4,5,11,12,13,14,15
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|\
                     GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOF,&GPIO_Initure); //初始化
    
    //初始化PG0,1,2,4,5,8,15
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|\
                     GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);  //初始化  
		
    //初始化PH6,7
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;              
    HAL_GPIO_Init(GPIOH,&GPIO_Initure); //初始化 

}

//向SDRAM发送命令
//bankx:0,向BANK5上面的SDRAM发送指令
//      1,向BANK6上面的SDRAM发送指令
//cmd:指令(0,正常模式/1,时钟配置使能/2,预充电所有存储区/3,自动刷新/4,加载模式寄存器/5,自刷新/6,掉电)
//refresh:自刷新次数
//regval:模式寄存器的定义
//返回值:0,正常;1,失败.
u8 SDRAM_Send_Cmd(u8 bankx,u8 cmd,u8 refresh,u16 regval)
{
    u32 target_bank=0;
    FMC_SDRAM_CommandTypeDef Command;
    
    if(bankx==0) target_bank=FMC_SDRAM_CMD_TARGET_BANK1;       
    else if(bankx==1) target_bank=FMC_SDRAM_CMD_TARGET_BANK2;   
    Command.CommandMode=cmd;                //命令
    Command.CommandTarget=target_bank;      //目标SDRAM存储区域
    Command.AutoRefreshNumber=refresh;      //自刷新次数
    Command.ModeRegisterDefinition=regval;  //要写入模式寄存器的值
    if(HAL_SDRAM_SendCommand(&SDRAM_Handler,&Command,0X1000)==HAL_OK) //向SDRAM发送命令
    {
        return 0;  
    }
    else return 1;    
}

//在指定地址(WriteAddr+Bank5_SDRAM_ADDR)开始,连续写入n个字节.
//pBuffer:字节指针
//WriteAddr:要写入的地址
//n:要写入的字节数
void FMC_SDRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*(vu8*)(Bank5_SDRAM_ADDR+WriteAddr)=*pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}

//在指定地址((WriteAddr+Bank5_SDRAM_ADDR))开始,连续读出n个字节.
//pBuffer:字节指针
//ReadAddr:要读出的起始地址
//n:要写入的字节数
void FMC_SDRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*pBuffer++=*(vu8*)(Bank5_SDRAM_ADDR+ReadAddr);
		ReadAddr++;
	}
}

/**
  * @brief  测试SDRAM是否正常 
  * @param  None
  * @retval 正常返回1，异常返回0
  */
uint8_t SDRAM_Test(void)
{
  /*写入数据计数器*/
  uint32_t counter=0;
  
  /* 8位的数据 */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16位的数据 */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  /*按8位格式读写数据，并校验*/
  
  /* 把SDRAM数据全部重置为0 ，W9825G6KH_SIZE是以8位为单位的 */
  for (counter = 0x00; counter < W9825G6KH_SIZE; counter++)
  {
    *(__IO uint8_t*) (Bank5_SDRAM_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* 向整个SDRAM写入数据  8位 */
  for (counter = 0; counter < W9825G6KH_SIZE; counter++)
  {
    *(__IO uint8_t*) (Bank5_SDRAM_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* 读取 SDRAM 数据并检测*/
  for(counter = 0; counter<W9825G6KH_SIZE;counter++ )
  {
    ubReaddata_8b = *(__IO uint8_t*)(Bank5_SDRAM_ADDR + counter);  //从该地址读出数据
    
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SDRAM_ERROR("8位数据读写错误！");
      return 0;
    }
  }
	
  
  /*按16位格式读写数据，并检测*/
  
  /* 把SDRAM数据全部重置为0 */
  for (counter = 0x00; counter < W9825G6KH_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (Bank5_SDRAM_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* 向整个SDRAM写入数据  16位 */
  for (counter = 0; counter < W9825G6KH_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (Bank5_SDRAM_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* 读取 SDRAM 数据并检测*/
  for(counter = 0; counter<W9825G6KH_SIZE/2;counter++ )
  {
    uhReaddata_16b = *(__IO uint16_t*)(Bank5_SDRAM_ADDR + 2*counter);  //从该地址读出数据
    
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SDRAM_ERROR("16位数据读写错误！");

      return 0;
    }
  }

  
  SDRAM_INFO("SDRAM读写测试正常！"); 
  /*检测正常，return 1 */
  return 1;
  

}


void SDRAM_Check(void)
{
  pSDRAM=(uint32_t*)SDRAM_BANK_ADDR;
	count=0;
	printf("开始写入SDRAM\r\n");
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
	printf("写入总字节数:%d\r\n",(uint32_t)pSDRAM-SDRAM_BANK_ADDR);

	count=0;
	pSDRAM=(uint32_t*)SDRAM_BANK_ADDR;
	printf("开始读取SDRAM并与原随机数比较\r\n");
	sdram_count=0;
	for(;sdram_count<SDRAM_SIZE;sdram_count++)
	{
		if(*pSDRAM != RadomBuffer[count])
		{
			printf("数据比较错误——退出~\r\n");
			break;
		}
		count++;
		pSDRAM++;
		if(count>=10000)
		{
			count=0;
		}
	}

	printf("比较通过总字节数:%d\r\n",(uint32_t)pSDRAM-SDRAM_BANK_ADDR);

	if(sdram_count == SDRAM_SIZE)
	{
		printf("SDRAM测试成功\r\n");
	}
	else
	{
		printf("SDRAM测试失败\r\n");
	}   
}

