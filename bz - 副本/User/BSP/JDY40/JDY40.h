#ifndef __JDY40_H_
#define __JDY40_H_
#include "stm32h7xx_hal_conf.h"
#include"stm32h7xx_hal.h"



#define JDY_BaundRate_1200      '1'
#define JDY_BaundRate_2400      '2'
#define JDY_BaundRate_4800      '3'
#define JDY_BaundRate_9600      '4'
#define JDY_BaundRate_14400     '5'
#define JDY_BaundRate_19200     '6'

#define JDY40_SET_1                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); 
#define JDY40_SET_0              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
#define JDY40_CS_1                 HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
#define JDY40_CS_0               HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);

static const uint8_t BaundRate[] = {'A', 'T', '+', 'B', 'A', 'U', 'D'}; 
static const uint8_t C_SizeOf_BaundRate = 7;
static const uint8_t RFID[] = {'A', 'T', '+', 'R', 'F', 'I', 'D'}; 
static const uint8_t C_SizeOf_RFID = 7;
static const uint8_t DVID[] = {'A', 'T', '+', 'D', 'V', 'I', 'D'}; 
static const uint8_t C_SizeOf_DVID = 7;
static const uint8_t RFC[] = {'A', 'T', '+', 'R', 'F', 'C'}; 
static const uint8_t C_SizeOf_RFC = 6;
static const uint8_t POWE[] = {'A', 'T', '+', 'P', 'O', 'W', 'E'}; 
static const uint8_t C_SizeOf_POWE = 7;
static const uint8_t CLSS[] = {'A', 'T', '+', 'C', 'L', 'S', 'S'}; 
static const uint8_t C_SizeOf_CLSS = 7;

void JDY40_Init(void);


#endif
