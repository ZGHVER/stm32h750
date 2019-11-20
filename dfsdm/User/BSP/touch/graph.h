#ifndef __GRAPH_H_
#define __GRAPH_H_

#include "stm32h7xx.h"
#include "./lcd/bsp_lcd.h"

typedef struct graph{
    uint16_t** GraphRam;
    uint8_t    LineNum;
    uint16_t   BoardSize;
    uint16_t   ArrColIndMax;
    uint32_t*  ClolrList;
    uint16_t   Counter;

    uint16_t x0;
    uint16_t y0;
    uint16_t width;
    uint16_t height;
    uint32_t BackColor;
    uint32_t GridColor;
}GRAPH;

void GRAPH_Drow(GRAPH* graph);
void GRAPH_ClearGraph(GRAPH* graph);
void GRAPH_RefGraph(GRAPH* graph, uint8_t isClearShadow);
void GRAPH_DrowPoint(GRAPH* graph, uint16_t col, uint16_t row, uint8_t lineCode);
void GRAPH_AddVal(GRAPH* graph, uint16_t value, uint16_t LineCode, uint8_t isMove);
void GRAPH_SetGridOn(GRAPH* graph, uint32_t gridColor, uint32_t textColor, uint16_t interval, uint16_t realLength);
void GRAPH_Init(GRAPH* graph, uint16_t x0, uint16_t y0, 
                uint16_t width, uint16_t height, uint16_t boardSize,
                uint32_t backColor, uint32_t gridColor,
                uint8_t LineNum, uint32_t* LineColors
                );
#endif
