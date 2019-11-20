#ifndef __OLED_H
#define __OLED_H
#include "stm32h7xx.h"

#define OLED_ADDRESS 0x78
#define OLED_IIC_SPEED 1000000

void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);	 
void OLED_ADDval(uint8_t value);

__STATIC_INLINE void MX_I2C1_Init(void);
__STATIC_INLINE void HAL_I2C1_MspInit(void);

#endif  
	 



