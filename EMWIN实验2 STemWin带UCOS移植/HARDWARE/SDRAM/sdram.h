#ifndef _SDRAM_H
#define _SDRAM_H
#include "sys.h"
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
extern SDRAM_HandleTypeDef SDRAM_Handler;//SDRAM句柄
#define Bank5_SDRAM_ADDR    ((u32)(0XD0000000)) //SDRAM开始地址
#define W9825G6KH_SIZE 0x2000000  //2000000*16bits = 0x2000000  ，32M字节


/*SDRAM 的bank选择*/  
#define FMC_BANK_SDRAM            FMC_Bank2_SDRAM  
#define FMC_COMMAND_TARGET_BANK   FMC_SDRAM_CMD_TARGET_BANK2

/**
  * @brief  FMC SDRAM 数据基地址
  */   
#define SDRAM_BANK_ADDR     ((uint32_t)0xD0000000)
  
/**
  * @brief  FMC SDRAM 数据宽度
  */  
/* #define SDRAM_MEMORY_WIDTH   FMC_SDMemory_Width_8b  */
#define SDRAM_MEMORY_WIDTH    FMC_SDRAM_MEM_BUS_WIDTH_16 

/**
  * @brief  FMC SDRAM CAS Latency
  */  
/* #define SDRAM_CAS_LATENCY   FMC_CAS_Latency_2  */
#define SDRAM_CAS_LATENCY    FMC_SDRAM_CAS_LATENCY_3

/**
  * @brief  FMC SDRAM SDCLK时钟分频因子
  */  
#define SDCLOCK_PERIOD    FMC_SDRAM_CLOCK_PERIOD_2        /* Default configuration used with LCD */
/* #define SDCLOCK_PERIOD    FMC_SDClock_Period_3 */

/**
  * @brief  FMC SDRAM 突发读取特性
  */  
#define SDRAM_READBURST    FMC_SDRAM_RBURST_DISABLE    /* Default configuration used with LCD */
/* #define SDRAM_READBURST    FMC_Read_Burst_Enable  */
/**
  * @brief  FMC SDRAM Bank Remap
  */    
/* #define SDRAM_BANK_REMAP */   

#define SDRAM_TIMEOUT                    ((uint32_t)0xFFFF)




//SDRAM配置参数
#define SDRAM_MODEREG_BURST_LENGTH_1             ((u16)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((u16)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((u16)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((u16)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((u16)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((u16)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((u16)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((u16)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((u16)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((u16)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((u16)0x0200)

/*信息输出*/
#define SDRAM_DEBUG_ON         1

#define SDRAM_INFO(fmt,arg...)           printf("<<-SDRAM-INFO->> "fmt"\n",##arg)
#define SDRAM_ERROR(fmt,arg...)          printf("<<-SDRAM-ERROR->> "fmt"\n",##arg)
#define SDRAM_DEBUG(fmt,arg...)          do{\
                                          if(SDRAM_DEBUG_ON)\
                                          printf("<<-SDRAM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)
  
/*地址信号线*/  
#define FMC_A0_GPIO_PORT        GPIOF
#define FMC_A0_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A0_GPIO_PIN         GPIO_PIN_0
#define FMC_A0_PINSOURCE        GPIO_PinSource0
#define FMC_A0_AF               GPIO_AF_FMC

#define FMC_A1_GPIO_PORT        GPIOF
#define FMC_A1_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A1_GPIO_PIN         GPIO_PIN_1
#define FMC_A1_PINSOURCE        GPIO_PinSource1
#define FMC_A1_AF               GPIO_AF_FMC

#define FMC_A2_GPIO_PORT        GPIOF
#define FMC_A2_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A2_GPIO_PIN         GPIO_PIN_2
#define FMC_A2_PINSOURCE        GPIO_PinSource2
#define FMC_A2_AF               GPIO_AF_FMC

#define FMC_A3_GPIO_PORT        GPIOF
#define FMC_A3_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A3_GPIO_PIN         GPIO_PIN_3
#define FMC_A3_PINSOURCE        GPIO_PinSource3
#define FMC_A3_AF               GPIO_AF_FMC

#define FMC_A4_GPIO_PORT        GPIOF
#define FMC_A4_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A4_GPIO_PIN         GPIO_PIN_4
#define FMC_A4_PINSOURCE        GPIO_PinSource4
#define FMC_A4_AF               GPIO_AF_FMC

#define FMC_A5_GPIO_PORT        GPIOF
#define FMC_A5_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A5_GPIO_PIN         GPIO_PIN_5
#define FMC_A5_PINSOURCE        GPIO_PinSource5
#define FMC_A5_AF               GPIO_AF_FMC

#define FMC_A6_GPIO_PORT        GPIOF
#define FMC_A6_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A6_GPIO_PIN         GPIO_PIN_12
#define FMC_A6_PINSOURCE        GPIO_PinSource12
#define FMC_A6_AF               GPIO_AF_FMC

#define FMC_A7_GPIO_PORT        GPIOF
#define FMC_A7_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A7_GPIO_PIN         GPIO_PIN_13
#define FMC_A7_PINSOURCE        GPIO_PinSource13
#define FMC_A7_AF               GPIO_AF_FMC

#define FMC_A8_GPIO_PORT        GPIOF
#define FMC_A8_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A8_GPIO_PIN         GPIO_PIN_14
#define FMC_A8_PINSOURCE        GPIO_PinSource14
#define FMC_A8_AF               GPIO_AF_FMC

#define FMC_A9_GPIO_PORT        GPIOF
#define FMC_A9_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A9_GPIO_PIN         GPIO_PIN_15
#define FMC_A9_PINSOURCE        GPIO_PinSource15
#define FMC_A9_AF               GPIO_AF_FMC


#define FMC_A10_GPIO_PORT        GPIOG
#define FMC_A10_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_A10_GPIO_PIN         GPIO_PIN_0
#define FMC_A10_PINSOURCE        GPIO_PinSource0
#define FMC_A10_AF               GPIO_AF_FMC


#define FMC_A11_GPIO_PORT        GPIOG
#define FMC_A11_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_A11_GPIO_PIN         GPIO_PIN_1
#define FMC_A11_PINSOURCE        GPIO_PinSource1
#define FMC_A11_AF               GPIO_AF_FMC

#define FMC_A12_GPIO_PORT        GPIOG
#define FMC_A12_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_A12_GPIO_PIN         GPIO_PIN_2
#define FMC_A12_PINSOURCE        GPIO_PinSource2
#define FMC_A12_AF               GPIO_AF_FMC

/*数据信号线*/
#define FMC_D0_GPIO_PORT        GPIOD
#define FMC_D0_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D0_GPIO_PIN         GPIO_PIN_14
#define FMC_D0_PINSOURCE        GPIO_PinSource14
#define FMC_D0_AF               GPIO_AF_FMC

#define FMC_D1_GPIO_PORT        GPIOD
#define FMC_D1_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D1_GPIO_PIN         GPIO_PIN_15
#define FMC_D1_PINSOURCE        GPIO_PinSource15
#define FMC_D1_AF               GPIO_AF_FMC

#define FMC_D2_GPIO_PORT        GPIOD
#define FMC_D2_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D2_GPIO_PIN         GPIO_PIN_0
#define FMC_D2_PINSOURCE        GPIO_PinSource0
#define FMC_D2_AF               GPIO_AF_FMC

#define FMC_D3_GPIO_PORT        GPIOD
#define FMC_D3_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D3_GPIO_PIN         GPIO_PIN_1
#define FMC_D3_PINSOURCE        GPIO_PinSource1
#define FMC_D3_AF               GPIO_AF_FMC

#define FMC_D4_GPIO_PORT        GPIOE
#define FMC_D4_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D4_GPIO_PIN         GPIO_PIN_7
#define FMC_D4_PINSOURCE        GPIO_PinSource7
#define FMC_D4_AF               GPIO_AF_FMC

#define FMC_D5_GPIO_PORT        GPIOE
#define FMC_D5_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D5_GPIO_PIN         GPIO_PIN_8
#define FMC_D5_PINSOURCE        GPIO_PinSource8
#define FMC_D5_AF               GPIO_AF_FMC

#define FMC_D6_GPIO_PORT        GPIOE
#define FMC_D6_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D6_GPIO_PIN         GPIO_PIN_9
#define FMC_D6_PINSOURCE        GPIO_PinSource9
#define FMC_D6_AF               GPIO_AF_FMC

#define FMC_D7_GPIO_PORT        GPIOE
#define FMC_D7_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D7_GPIO_PIN         GPIO_PIN_10
#define FMC_D7_PINSOURCE        GPIO_PinSource10
#define FMC_D7_AF               GPIO_AF_FMC

#define FMC_D8_GPIO_PORT        GPIOE
#define FMC_D8_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D8_GPIO_PIN         GPIO_PIN_11
#define FMC_D8_PINSOURCE        GPIO_PinSource11
#define FMC_D8_AF               GPIO_AF_FMC

#define FMC_D9_GPIO_PORT        GPIOE
#define FMC_D9_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D9_GPIO_PIN         GPIO_PIN_12
#define FMC_D9_PINSOURCE        GPIO_PinSource12
#define FMC_D9_AF               GPIO_AF_FMC

#define FMC_D10_GPIO_PORT        GPIOE
#define FMC_D10_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D10_GPIO_PIN         GPIO_PIN_13
#define FMC_D10_PINSOURCE        GPIO_PinSource13
#define FMC_D10_AF               GPIO_AF_FMC

#define FMC_D11_GPIO_PORT        GPIOE
#define FMC_D11_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D11_GPIO_PIN         GPIO_PIN_14
#define FMC_D11_PINSOURCE        GPIO_PinSource14
#define FMC_D11_AF               GPIO_AF_FMC

#define FMC_D12_GPIO_PORT        GPIOE
#define FMC_D12_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D12_GPIO_PIN         GPIO_PIN_15
#define FMC_D12_PINSOURCE        GPIO_PinSource15
#define FMC_D12_AF               GPIO_AF_FMC

#define FMC_D13_GPIO_PORT        GPIOD
#define FMC_D13_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D13_GPIO_PIN         GPIO_PIN_8
#define FMC_D13_PINSOURCE        GPIO_PinSource8
#define FMC_D13_AF               GPIO_AF_FMC

#define FMC_D14_GPIO_PORT        GPIOD
#define FMC_D14_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D14_GPIO_PIN         GPIO_PIN_9
#define FMC_D14_PINSOURCE        GPIO_PinSource9
#define FMC_D14_AF               GPIO_AF_FMC

#define FMC_D15_GPIO_PORT        GPIOD
#define FMC_D15_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D15_GPIO_PIN         GPIO_PIN_10
#define FMC_D15_PINSOURCE        GPIO_PinSource10
#define FMC_D15_AF               GPIO_AF_FMC


/*控制信号线*/  
#define FMC_CS_GPIO_PORT        GPIOH
#define FMC_CS_GPIO_CLK         __HAL_RCC_GPIOH_CLK_ENABLE()
#define FMC_CS_GPIO_PIN         GPIO_PIN_6
#define FMC_CS_PINSOURCE        GPIO_PinSource6
#define FMC_CS_AF               GPIO_AF_FMC

#define FMC_BA0_GPIO_PORT        GPIOG
#define FMC_BA0_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_BA0_GPIO_PIN         GPIO_PIN_4
#define FMC_BA0_PINSOURCE        GPIO_PinSource4
#define FMC_BA0_AF               GPIO_AF_FMC

#define FMC_BA1_GPIO_PORT        GPIOG
#define FMC_BA1_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_BA1_GPIO_PIN         GPIO_PIN_5
#define FMC_BA1_PINSOURCE        GPIO_PinSource5
#define FMC_BA1_AF               GPIO_AF_FMC

#define FMC_WE_GPIO_PORT        GPIOC
#define FMC_WE_GPIO_CLK         __HAL_RCC_GPIOC_CLK_ENABLE()
#define FMC_WE_GPIO_PIN         GPIO_PIN_0
#define FMC_WE_PINSOURCE        GPIO_PinSource0
#define FMC_WE_AF               GPIO_AF_FMC

#define FMC_RAS_GPIO_PORT        GPIOF
#define FMC_RAS_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_RAS_GPIO_PIN         GPIO_PIN_11
#define FMC_RAS_PINSOURCE        GPIO_PinSource11
#define FMC_RAS_AF               GPIO_AF_FMC

#define FMC_CAS_GPIO_PORT        GPIOG
#define FMC_CAS_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_CAS_GPIO_PIN         GPIO_PIN_15
#define FMC_CAS_PINSOURCE        GPIO_PinSource15
#define FMC_CAS_AF               GPIO_AF_FMC

#define FMC_CLK_GPIO_PORT        GPIOG
#define FMC_CLK_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_CLK_GPIO_PIN         GPIO_PIN_8
#define FMC_CLK_PINSOURCE        GPIO_PinSource8
#define FMC_CLK_AF               GPIO_AF_FMC

#define FMC_CKE_GPIO_PORT        GPIOH
#define FMC_CKE_GPIO_CLK         __HAL_RCC_GPIOH_CLK_ENABLE()
#define FMC_CKE_GPIO_PIN         GPIO_PIN_7
#define FMC_CKE_PINSOURCE        GPIO_PinSource7
#define FMC_CKE_AF               GPIO_AF_FMC

/*UDQM LDQM*/
#define FMC_UDQM_GPIO_PORT        GPIOE
#define FMC_UDQM_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_UDQM_GPIO_PIN         GPIO_PIN_1
#define FMC_UDQM_PINSOURCE        GPIO_PinSource1
#define FMC_UDQM_AF               GPIO_AF_FMC

#define FMC_LDQM_GPIO_PORT        GPIOE
#define FMC_LDQM_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_LDQM_GPIO_PIN         GPIO_PIN_0
#define FMC_LDQM_PINSOURCE        GPIO_PinSource0
#define FMC_LDQM_AF               GPIO_AF_FMC

void SDRAM_Init(void);
void SDRAM_MPU_Config(void);
u8 SDRAM_Send_Cmd(u8 bankx,u8 cmd,u8 refresh,u16 regval);
void FMC_SDRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n);
void FMC_SDRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n);
#endif
