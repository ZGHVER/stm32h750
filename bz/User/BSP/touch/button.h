#ifndef __BUTTON_H_
#define __BUTTON_H_

#include "./lcd/bsp_lcd.h"
#include "stm32h7xx.h"

typedef struct button{
    int8_t  isActive;

    uint16_t x0;
    uint16_t y0;
    uint16_t x1;
    uint16_t y1;
    uint8_t  width;
    uint8_t  height;
    uint32_t id;
    int32_t  touchID;
    char*    Text;
    uint32_t BackColor;
    uint32_t TextColor;
    sFONT*   font;
    uint32_t Counter;
    uint32_t MaxCounter;
    void     (*PushCallback)(struct button* butto);

    struct button* next;
}BUTTON;

void BUTTON_SetBorderColor(uint32_t border_color);
void BUTTON_Drow(BUTTON* button);
void BUTTON_ActPush(int32_t id, int32_t x,int32_t y);
void BUTTON_ActUplift(int32_t id);
void BUTTON_Disable(BUTTON* button);
void Button_KeyBoard_Init(BUTTON* but,uint16_t x, uint16_t y, uint32_t size);
void BUTTON_Init(BUTTON* button, uint32_t counter, uint32_t id,
                uint16_t x, uint16_t y,uint8_t width, uint8_t height, 
                uint32_t BacColor, uint32_t TextColor,
                char* Text, sFONT* TextFont,
                void     (*PushCallback)(BUTTON* button)
                );
#endif
