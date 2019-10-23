//
// Created by ZGH on 2019/10/17.
//

#ifndef UNTITLED3_AL_ARRAY_H
#define UNTITLED3_AL_ARRAY_H


#include <stdint.h>
#define CAST(__TYPE__, __DATA__) ((__TYPE__*)__DATA__)

typedef void*(*mem_getterType)(uint32_t size);
typedef void(*mem_putterType)(void*);

typedef struct array_node{
    void*              data;
    struct array_node* prePtr;
    struct array_node* NextPtr;
}AR_NODE;

typedef struct al_array{
    struct array_node* Top;
    struct array_node* Bottom;
    uint32_t           length;
}AL_Array;

void AL_menSet(mem_getterType getter, mem_putterType putter);
void AL_Array_Init(AL_Array* array);
uint32_t AL_Array_getLength(AL_Array* array);
uint8_t AL_Array_isEmpty(AL_Array* array);
void AL_Array_Push(AL_Array* array, void* data);
void* AL_Array_Top_POP(AL_Array *array);
void AL_Array_Enqueue(AL_Array* array, void* data);
void* AL_Array_Bottom_POP(AL_Array *array);
void AL_Array_Clear(AL_Array* array);

#endif //UNTITLED3_AL_QUEUE_H
