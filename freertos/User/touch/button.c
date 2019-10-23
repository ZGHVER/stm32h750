#include "button.h"

static uint32_t _borderColor = LCD_COLOR_WHITE;

BUTTON*  root = 0;

static void _Push(BUTTON* button){
   // uint32_t tc = LCD_GetTextColor();
   // uint32_t tl = LCD_GetActiveLayer();

   // LCD_SelectLayer(0);
    //LCD_SetTextColor(button->BackColor * 0.8);
   // LCD_FillRect(button->x0 + 3, button->y0 + 3, button->width - 5, button->height - 5);
    button->PushCallback(button);

  //  LCD_SetTextColor(tc);
    //LCD_SelectLayer(tl);
}

static void _Uplift(BUTTON* button){
    uint32_t tc = LCD_GetTextColor();
    uint32_t tl = LCD_GetActiveLayer();

    LCD_SelectLayer(0);
    LCD_SetTextColor(button->BackColor);
    LCD_FillRect(button->x0 + 3, button->y0 + 3, button->width - 5, button->height - 5);

    LCD_SetTextColor(tc);
    LCD_SelectLayer(tl);
}

void BUTTON_SetBorderColor(uint32_t border_color){
    _borderColor = border_color;
}

void BUTTON_Init(BUTTON* button, uint32_t counter, uint32_t id,
                uint16_t x, uint16_t y,uint8_t width, uint8_t height, 
                uint32_t BacColor, uint32_t TextColor,
                char* Text, sFONT* TextFont,
                void     (*PushCallback)(BUTTON* but)
                ){
    BUTTON* curPtr = 0;
    if(x > 800 || y > 480){
        return;
    }
    button->x0 = x;
    button->y0 = y;
    button->width = width;
    button->height = height;
    button->id = id;
    button->x1 = x + width;
    button->y1 = y + height;
    button->Text = Text;
    button->touchID = -1;
    button->font = TextFont;
    button->BackColor = BacColor;
    button->TextColor = TextColor;
    button->isActive  = -1; 
    button->PushCallback = PushCallback;
    button->Counter = 0;
    button->MaxCounter = counter;

}

void BUTTON_ActPush(int32_t id, int32_t x,int32_t y){
    BUTTON* button = root;
    while(button != 0){
            BUTTON_Drow(button);
            button->PushCallback(button);
            button = button->next;
        }
  /*  while(button != 0){
        if(button->isActive == -1){
            button = button->next;
            continue;
        }
        if(x < button->x1 && x > button->x0 && y < button->y1 && y > button->y0){
            if(button->touchID == id){
                button->Counter ++;
                if(button->Counter == button->MaxCounter){
                   _Push(button);
                   button->Counter = 0;
                }
                return;
            }else{
              _Push(button);
             button->touchID = id;
             return;
            }
        }else if(button->touchID == id){
            _Uplift(button);
            button->Counter = 0;
            button->touchID = -1;
            return;
        }
        button = button->next;
    }*/
}

void BUTTON_ActUplift(int32_t id){
    BUTTON* button = root;
    while(button != 0){
        if(button->isActive == -1){
            button = button->next;
            continue;
        }
        if(button->touchID == id){
            _Uplift(button);
            button->Counter = 0;
            button->touchID = -1;
            return;
        }
        button = button->next;
    }
}

static int cont_str(char *s){
    int i = 0;      
    while ( s[i++] != '\0');
    return i;
}

void BUTTON_Drow(BUTTON* button){
    sFONT* temp = LCD_GetFont();
    uint32_t tc = LCD_GetTextColor();
    uint32_t tl = LCD_GetActiveLayer();
    LCD_SelectLayer(0);
    LCD_SetTextColor(_borderColor);
    LCD_DrawRect(button->x0 , button->y0 , button->width, button->height);
    LCD_DrawRect(button->x0 + 1, button->y0 + 1, button->width - 2, button->height - 2);
    LCD_DrawRect(button->x0 + 2, button->y0 + 2, button->width - 4, button->height - 4);
    LCD_SetTextColor(button->BackColor);
    LCD_FillRect(button->x0 + 3, button->y0 + 3, button->width - 5, button->height - 5);

    LCD_SelectLayer(1);
    LCD_SetFont(button->font);
    LCD_SetColors(button->TextColor, LCD_COLOR_LUCENCY);
    uint8_t size = cont_str((char*)button->Text) - 1;
    uint16_t xpos = button->x0 + button->width / 2 - ((double)size / 2.0 * button->font->Width);
    uint16_t ypos = button->y0 + button->height / 2 - button->font->Height / 2;
    LCD_DisplayStringAtABSPos(xpos, ypos, (uint8_t*)button->Text);

    LCD_SetFont(temp);
    LCD_SetTextColor(tc);
    LCD_SelectLayer(tl);
    button->isActive = 1;
}

void BUTTON_Disable(BUTTON* button){

}

void CallBack(BUTTON* but){
    char s[10];
    sprintf(s, "%d", but->id);
    LCD_DisplayStringAtABSPos(100, 50, s);
}

void Button_KeyBoard_Init(uint16_t x, uint16_t y, uint32_t size){
    BUTTON but[16];
    BUTTON* r = root;
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            BUTTON_Init(&but[i * 4 + j], 20, i * 4 + j,
                        x + i * size, y + j * size,
                        size, size, 
                        LCD_COLOR_YELLOW, LCD_COLOR_BLUE, 
                        "1", &Font16,
                         CallBack);
        }
        
    }    
    r = root;
    while(r != 0){
            BUTTON_Drow(r);
            r->PushCallback(r);
            r = r->next;
        }
}

