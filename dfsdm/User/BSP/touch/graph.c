#include "./touch/graph.h"

__STATIC_INLINE uint16_t** createArray(uint16_t i, uint16_t j){
    uint16_t **e = (uint16_t **)malloc((unsigned int)sizeof(uint16_t *) * i);
    for (uint16_t d = 0; d < i; d++)
        e[d] = (uint16_t *) malloc(sizeof(uint16_t) * j);
    return e;
}

void GRAPH_Init(GRAPH* graph, uint16_t x0, uint16_t y0, 
                uint16_t width, uint16_t height, uint16_t BoardSize,
                uint32_t backColor, uint32_t gridColor,
                uint8_t LineNum, uint32_t* LineColors
                ){

    graph->x0 = x0;
    graph->y0 = y0;
    graph->width = width;
    graph->height = height;
    graph->BoardSize = BoardSize;
    graph->BackColor = backColor;
    graph->GridColor = gridColor;
    graph->LineNum = LineNum;
    graph->ClolrList = LineColors;
    graph->ArrColIndMax = graph->width -  2 * graph->BoardSize;
    graph->GraphRam = createArray(graph->ArrColIndMax, graph->LineNum);

    for(uint16_t i = 0; i < graph->ArrColIndMax; i++)
        for(uint16_t j = 0; j < graph->LineNum; j++)
            *(*(graph->GraphRam + i) + j) = 0;

    GRAPH_Drow(graph);
}

void GRAPH_Drow(GRAPH* graph){

    LCD_SelectLayer(0);
    LCD_SetTextColor(graph->BackColor);
    LCD_FillRect(graph->x0, graph->y0, graph->width, graph->height);

    LCD_SetTextColor(graph->GridColor);
    LCD_DrawHLine(graph->x0 + graph->BoardSize, 
                  graph->y0 + graph->height - graph->BoardSize, 
                  graph->width - 2 * graph->BoardSize
                  );
    LCD_DrawVLine(graph->x0 + graph->BoardSize, 
                  graph->y0 + graph->BoardSize, 
                  graph->height - 2 * graph->BoardSize
                  );

}

static int cont_str(char *s){
    int i = 0;      
    while ( s[i++] != '\0');
    return i;
}

void GRAPH_SetGridOn(GRAPH* graph, uint32_t gridColor, uint32_t textColor,uint16_t interval, uint16_t realLength){
    LCD_SelectLayer(0);
    gridColor *= 0.8; 
    uint32_t imagLength = interval - realLength;
    uint32_t TopBoard = graph->y0 + graph->BoardSize;
    uint32_t BottomBoard = graph->y0 + graph->height - graph->BoardSize;
    uint32_t LeftBoard = graph->x0 + graph->BoardSize;
    uint32_t RightBoard = graph->x0 + graph->width - graph->BoardSize;
    uint32_t LineLength = imagLength + realLength;
    uint16_t Hcounter = interval;
    uint16_t Vcounter = interval;
//    uint16_t Vpos = LeftBoard;
    char s[10];

    LCD_SetFont(&Font12);
    LCD_SetBackColor(graph->BackColor);
    for(uint16_t i = BottomBoard - interval; i > TopBoard; i -= interval){
        LCD_SetTextColor(textColor);
        sprintf(s, "%d", Hcounter);
        Hcounter += interval;
        LCD_DisplayStringAtABSPos(LeftBoard - cont_str(s) * 7 + 2, i - 6,(uint8_t*)s); 
        LCD_SetTextColor(gridColor); 
        for(uint16_t j = LeftBoard; j < RightBoard; j += LineLength){
            if(j + realLength > RightBoard)
                LCD_DrawHLine(j, i, j + realLength - RightBoard);
            else
                LCD_DrawHLine(j, i, realLength);
        }
    }

    for(uint16_t j = LeftBoard + realLength + imagLength / 2; j < RightBoard ; j += interval){
        LCD_SetTextColor(textColor);
        sprintf(s, "%d", Vcounter);
        Vcounter += interval;
        LCD_DisplayStringAtABSPos(j - cont_str(s) / 2 * 7 + 2, BottomBoard + 4, (uint8_t*)s);
        LCD_SetTextColor(gridColor); 
        for(uint16_t i = TopBoard + interval - realLength - imagLength / 2; i < BottomBoard; i += LineLength){  
            if(i + realLength > BottomBoard)
                LCD_DrawVLine(j, i, i + realLength - BottomBoard);
            else
                LCD_DrawVLine(j, i, realLength);
        }
    }

    LCD_SetTextColor(graph->GridColor);
    LCD_DrawHLine(graph->x0 + graph->BoardSize, 
                  graph->y0 + graph->height - graph->BoardSize, 
                  graph->width - 2 * graph->BoardSize
                  );
    LCD_DrawVLine(graph->x0 + graph->BoardSize, 
                  graph->y0 + graph->BoardSize, 
                  graph->height - 2 * graph->BoardSize
                  );
}

void GRAPH_DrowPoint(GRAPH* graph, uint16_t col, uint16_t row, uint8_t lineCode){
    graph->GraphRam[col][lineCode] = row;
}

void GRAPH_AddVal(GRAPH* graph, uint16_t value, uint16_t LineCode, uint8_t isMove){
    if(graph->Counter < graph->ArrColIndMax){
        graph->GraphRam[graph->Counter][LineCode] = value;
        if(isMove)
            graph->Counter++;
        return;
    }
    if(isMove){
        uint16_t* temp = 0;
        temp = graph->GraphRam[0];
        for(uint16_t i = 0; i < graph->ArrColIndMax - 1; i ++){
            graph->GraphRam[i] = graph->GraphRam[i + 1];
        }
        
        for(uint8_t i = 0; i < graph->LineNum; i ++){
            temp[i] = 0;
        }
        graph->GraphRam[graph->ArrColIndMax - 1] = temp;
    }
    graph->GraphRam[graph->ArrColIndMax - 1][LineCode] = value;
}

void GRAPH_ClearGraph(GRAPH* graph){
    LCD_SelectLayer(1);
    for(uint16_t i = 0; i < graph->ArrColIndMax; i ++){
        for(uint8_t j = 0; j < graph->LineNum; j ++){
            int q = i + graph->BoardSize + graph->x0;
            int w = graph->y0  + graph->height - graph->BoardSize - graph->GraphRam[i][j];
            //int e = graph->ClolrList[j];
            LCD_DrawPixel(q,w ,LCD_COLOR_LUCENCY );

        }
    }
}

void GRAPH_RefGraph(GRAPH* graph, uint8_t isClearShadow){
    LCD_SelectLayer(1);
    int q = graph->BoardSize + graph->x0;
    int w = graph->y0  + graph->height - graph->BoardSize;

    for(uint16_t i = 0; i < graph->Counter - 1; i ++){
        for(uint8_t j = 0; j < graph->LineNum; j ++){
            int z = w - graph->GraphRam[i][j];
            if(graph->GraphRam[i][j] != graph->GraphRam[i + 1][j] && isClearShadow){
                //LCD_DrawPixel(q + 1, w, LCD_COLOR_LUCENCY); 
                LCD_SetTextColor(LCD_COLOR_LUCENCY);
                LCD_DrawCircle(q + i + 1, z, 2);                       
            }
            LCD_SetTextColor(graph->ClolrList[j]);
            LCD_FillCircle(q + i, z, 2);
            //LCD_DrawPixel(q, w, e);
        }
    }
}
