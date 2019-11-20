/**
  ******************************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   sdram应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 H750 开发板  
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./sdram/bsp_sdram.h"  
#include"stm32h7xx_hal_sdram.h"
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
  
	/* 使能SDRAM相关的IO时钟 */
	/*地址信号线*/
	FMC_A0_GPIO_CLK();FMC_A1_GPIO_CLK(); FMC_A2_GPIO_CLK();
	FMC_A3_GPIO_CLK();FMC_A4_GPIO_CLK(); FMC_A5_GPIO_CLK();
	FMC_A6_GPIO_CLK();FMC_A7_GPIO_CLK(); FMC_A8_GPIO_CLK();
	FMC_A9_GPIO_CLK();FMC_A10_GPIO_CLK();FMC_A11_GPIO_CLK();
	FMC_A12_GPIO_CLK();
	/*数据信号线*/
	FMC_D0_GPIO_CLK(); FMC_D1_GPIO_CLK() ; FMC_D2_GPIO_CLK() ; 
	FMC_D3_GPIO_CLK(); FMC_D4_GPIO_CLK() ; FMC_D5_GPIO_CLK() ;
	FMC_D6_GPIO_CLK(); FMC_D7_GPIO_CLK() ; FMC_D8_GPIO_CLK() ;
	FMC_D9_GPIO_CLK(); FMC_D10_GPIO_CLK(); FMC_D11_GPIO_CLK();
	FMC_D12_GPIO_CLK();FMC_D13_GPIO_CLK(); FMC_D14_GPIO_CLK();
	FMC_D15_GPIO_CLK();FMC_D16_GPIO_CLK(); FMC_D17_GPIO_CLK(); 
	FMC_D18_GPIO_CLK();FMC_D19_GPIO_CLK(); FMC_D20_GPIO_CLK();
	FMC_D21_GPIO_CLK();FMC_D22_GPIO_CLK(); FMC_D23_GPIO_CLK();
	FMC_D24_GPIO_CLK();FMC_D25_GPIO_CLK(); FMC_D26_GPIO_CLK();
	FMC_D27_GPIO_CLK();FMC_D28_GPIO_CLK(); FMC_D29_GPIO_CLK();
	FMC_D30_GPIO_CLK();FMC_D31_GPIO_CLK();	
	/*控制信号线*/
	FMC_CS_GPIO_CLK() ; FMC_BA0_GPIO_CLK(); FMC_BA1_GPIO_CLK() ;
	FMC_WE_GPIO_CLK() ; FMC_RAS_GPIO_CLK(); FMC_CAS_GPIO_CLK();
	FMC_CLK_GPIO_CLK(); FMC_CKE_GPIO_CLK(); FMC_UDQM_GPIO_CLK();
	FMC_LDQM_GPIO_CLK();FMC_UDQM2_GPIO_CLK();FMC_LDQM2_GPIO_CLK();
  
	/*-- SDRAM IO 配置 -----------------------------------------------------*/     
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;//配置为复用功能
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
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

	GPIO_InitStructure.Pin = FMC_D16_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D16_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D17_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D17_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D18_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D18_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D19_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D19_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D20_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D20_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D21_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D21_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D22_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D22_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D23_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D23_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D24_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D24_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D25_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D25_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D26_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D26_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D27_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D27_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D28_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D28_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D29_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D29_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D30_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D30_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D31_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D31_GPIO_PORT, &GPIO_InitStructure);
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

	GPIO_InitStructure.Pin = FMC_UDQM2_GPIO_PIN; 
	HAL_GPIO_Init(FMC_UDQM2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_LDQM2_GPIO_PIN; 
	HAL_GPIO_Init(FMC_LDQM2_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  对SDRAM芯片进行初始化配置
  * @param  None. 
  * @retval None.
  */
static void SDRAM_InitSequence(void)
{
	uint32_t tmpr = 0;

	/* Step 1 ----------------------------------------------------------------*/
	/* 配置命令：开启提供给SDRAM的时钟 */
	Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = 0;
	/* 发送配置命令 */
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 2: 延时100us */ 
	SDRAM_delay(1);

	/* Step 3 ----------------------------------------------------------------*/
	/* 配置命令：对所有的bank预充电 */ 
	Command.CommandMode = FMC_SDRAM_CMD_PALL;
	Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = 0;
	/* 发送配置命令 */
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);   

	/* Step 4 ----------------------------------------------------------------*/
	/* 配置命令：自动刷新 */   
	Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
	Command.AutoRefreshNumber = 8;
	Command.ModeRegisterDefinition = 0;
	/* 发送配置命令 */
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 5 ----------------------------------------------------------------*/
	/* 设置sdram寄存器配置 */
	tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
				   SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
				   SDRAM_MODEREG_CAS_LATENCY_3           |
				   SDRAM_MODEREG_OPERATING_MODE_STANDARD |
				   SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	/* 配置命令：设置SDRAM寄存器 */
	Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = tmpr;
	/* 发送配置命令 */
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 6 ----------------------------------------------------------------*/

	/* 设置刷新计数器 */
	/* 刷新周期=64ms/4096行=15.625us */
	/* COUNT=(15.625us x Freq) - 20 */
	/* 设置自刷新速率 */
	HAL_SDRAM_ProgramRefreshRate(&sdramHandle, 1855); 
}


/**
  * @brief  初始化配置使用SDRAM的FMC及GPIO接口，
  *         本函数在SDRAM读写操作前需要被调用
  * @param  None
  * @retval None
  */
void SDRAM_Init(void)
{

  	FMC_SDRAM_TimingTypeDef SdramTiming;
	RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
	
  /* 配置FMC接口相关的 GPIO*/
  SDRAM_GPIO_Config();
  
/*
	//配置SDRAM时钟源
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FMC;
  RCC_PeriphClkInit.PLL2.PLL2M = 5;
  RCC_PeriphClkInit.PLL2.PLL2N = 144;
  RCC_PeriphClkInit.PLL2.PLL2P = 2;
  RCC_PeriphClkInit.PLL2.PLL2Q = 2;
  RCC_PeriphClkInit.PLL2.PLL2R = 3;
  RCC_PeriphClkInit.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
  RCC_PeriphClkInit.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  RCC_PeriphClkInit.PLL2.PLL2FRACN = 0;
  RCC_PeriphClkInit.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
  {
    while(1);
  }
  */

  /* 使能 FMC 时钟 */
  __FMC_CLK_ENABLE();

  /*执行SDRAM1的内存初始化序列 */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1结构体初始化*/
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;//SDRAM列数
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;//SDRAM行数
  hsdram1.Init.MemoryDataWidth = SDRAM_MEMORY_WIDTH;//总线数据宽度为32位
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;//4个扇区
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;//列地址选通信延时
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;//禁止写保护
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;//SDRAM时钟120MHz
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;     //使能突发传输模式
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1; //读通道延时
  /* SDRAM时序 */
  SdramTiming.LoadToActiveDelay = 2;//加载模式寄存器命令与行有效或刷新命令之间的延迟
  SdramTiming.ExitSelfRefreshDelay = 8;//退出自我刷新到行有效命令之间的延迟
  SdramTiming.SelfRefreshTime = 5;//行有效与预充电命令之间的延迟
  SdramTiming.RowCycleDelay = 8;//两个刷新命令或两个行有效命令之间的延迟
  SdramTiming.WriteRecoveryTime = 2;//写入命令到预充电命令之间的延迟
  SdramTiming.RPDelay = 2;//预充电与行有效命令之间的延迟
  SdramTiming.RCDDelay = 2;//行有效与列读写命令之间的延迟

  HAL_SDRAM_Init(&hsdram1, &SdramTiming);  
  /* FMC SDRAM 设备时序初始化 */
  SDRAM_InitSequence(); 
  
}
/*********************************************END OF FILE**********************/

