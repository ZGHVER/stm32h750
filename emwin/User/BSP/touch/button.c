#include "button.h"

static uint32_t _borderColor = MLCD_COLOR_WHITE;

static uint16_t _B_MaxX = 0;
static uint16_t _B_MaxY = 0;
static uint16_t _B_MinX = 800;
static uint16_t _B_MinY = 480;

BUTTON* root = 0;
BUTTON* curptr;

static void _Push(BUTTON* button){
    uint32_t tc = MLCD_GetTextColor();
    uint32_t tl = MLCD_GetActiveLayer();

    MLCD_SelectLayer(0);
    MLCD_SetTextColor(button->BackColor * 0.8);
    MLCD_FillRect(button->x0 + 3, button->y0 + 3, button->width - 5, button->height - 5);
    button->PushCallback(button);

    MLCD_SetTextColor(tc);
    MLCD_SelectLayer(tl);
}

static void _Uplift(BUTTON* button){
    uint32_t tc = MLCD_GetTextColor();
    uint32_t tl = MLCD_GetActiveLayer();

    MLCD_SelectLayer(0);
    MLCD_SetTextColor(button->BackColor);
    MLCD_FillRect(button->x0 + 3, button->y0 + 3, button->width - 5, button->height - 5);

    MLCD_SetTextColor(tc);
    MLCD_SelectLayer(tl);
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

    _B_MinX = _B_MinX < x ? _B_MinX : x;                
    _B_MinY = _B_MinY < y ? _B_MinY : y;

    _B_MaxX = _B_MaxX > x + width ? _B_MaxX : x + width;
    _B_MaxY = _B_MaxY > y + width ? _B_MaxY : y + height;

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
    if(root == 0){
        root = button;
        curptr = button;
    }else {
        curptr->next = button;
        curptr = curptr->next;
        curptr->next = 0;
    }
}

void BUTTON_ActPush(int32_t id, int32_t x,int32_t y){
    if(!(x < _B_MaxX && x > _B_MinX && y < _B_MaxY && y > _B_MinY))
        return;
    BUTTON* button;
    button = root;
    while(button != 0){
        if(x < button->x1 && x > button->x0 && y < button->y1 && y > button->y0){
            if(button->isActive == -1)
                return;
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
            }
        }else if(button->touchID == id){
            if(button->isActive == -1)
                return;
            _Uplift(button);
            button->Counter = 0;
            button->touchID = -1;
            return;
        }
        button = button->next;
    }
}

void BUTTON_ActUplift(int32_t id){
    BUTTON* button;
    button = root;
    while(button != 0){
        if(button->touchID == id){
            if(button->isActive == -1)
                return;
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
    sFONT* temp = MLCD_GetFont();
    uint32_t tc = MLCD_GetTextColor();
    uint32_t tl = MLCD_GetActiveLayer();
    MLCD_SelectLayer(0);
    MLCD_SetTextColor(_borderColor);
    MLCD_FillRect(button->x0 , button->y0 , button->width, button->height);
    MLCD_SetTextColor(button->BackColor);
    MLCD_FillRect(button->x0 + 3, button->y0 + 3, button->width - 5, button->height - 5);

    MLCD_SelectLayer(1);
    MLCD_SetFont(button->font);
    MLCD_SetColors(button->TextColor, MLCD_COLOR_LUCENCY);
    uint8_t size = cont_str((char*)button->Text) - 1;
    uint16_t xpos = button->x0 + button->width / 2 - ((double)size / 2.0 * button->font->Width);
    uint16_t ypos = button->y0 + button->height / 2 - button->font->Height / 2;
    MLCD_DisplayStringAtABSPos(xpos, ypos, (uint8_t*)button->Text);

    MLCD_SetFont(temp);
    MLCD_SetTextColor(tc);
    MLCD_SelectLayer(tl);
    button->isActive = 1;
}

void BUTTON_Disable(BUTTON* button){

}

void Button_KeyBoard_Init(BUTTON* but ,uint8_t* idList, char(*ptsr)[10], void (*callback)(BUTTON*),
                          uint16_t x, uint16_t y, uint32_t size){
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            BUTTON_Init(&but[i * 4 + j], 20, idList[i * 4 + j],
                        x + j * size, y + i * size,
                        size, size, 
                        MLCD_COLOR_YELLOW, MLCD_COLOR_BLUE, 
                        *(ptsr + i * 4 + j), &Font16,
                         callback);
        }
    }   

    for (uint8_t j = 0; j < 16; j++)
    {
        BUTTON_Drow(&but[j]);
    }
}

