//
// Created by ZGH on 2019/10/17.
//

#include "AL_Array.h"
mem_getterType _mem_getter;
mem_putterType _mem_putter;

void AL_menSet(mem_getterType getter, mem_putterType putter){
    _mem_getter = getter;
    _mem_putter = putter;
}

void AL_Array_Init(AL_Array* array){
    array->Top = 0;
    array->Bottom = 0;
    array->length = 0;
}

uint32_t AL_Array_getLength(AL_Array* array){
    return array->length;
}

uint8_t AL_Array_isEmpty(AL_Array* array){
    return array->length == 0;
}

void AL_Array_Push(AL_Array* array, void* data){
    AR_NODE* Node = (AR_NODE*)_mem_getter(sizeof(AR_NODE));
    Node->data = data;
    if(array->Top == 0){
        array->Top = Node;
        array->Bottom = Node;
			
        Node->NextPtr = 0;
        Node->prePtr = 0;
        array->length ++;
    }else{
        Node->prePtr = array->Top;
        Node->NextPtr = 0;
        array->Top->NextPtr = Node;
        array->Top = Node;
        array->length ++;
    }
}

void* AL_Array_Top_POP(AL_Array *array){
    if(AL_Array_isEmpty(array)){
        return 0;
    }
    AR_NODE* temp = array->Top;
    void* data = temp->data;
    array->Top = array->Top->prePtr;
    if(array->Top != 0){
        array->Top->NextPtr = 0;
    }else{
        array->Bottom = 0;
    }
    array->length --;
    temp->NextPtr = 0;
    temp->prePtr = 0;
    _mem_putter(temp);
    return data;
}

void AL_Array_Enqueue(AL_Array* array, void* data){
    AR_NODE* Node = (AR_NODE*)_mem_getter(sizeof(AR_NODE));
    Node->data = data;
    if(array->Bottom == 0){
        array->Bottom = Node;
        array->Top = Node;
        Node->prePtr = 0;
        Node->NextPtr = 0;
    }else{
        Node->NextPtr = array->Bottom;
        Node->prePtr = 0;
        array->Bottom->prePtr = Node;
        array->Bottom = Node;
    }
    array->length ++;
}

void* AL_Array_Bottom_POP(AL_Array *array){
    if(AL_Array_isEmpty(array)){
        return 0;
    }
    AR_NODE* temp = array->Bottom;
    void* data = temp->data;
    array->Bottom = array->Bottom->NextPtr;
    if(array->Bottom != 0){
        array->Bottom->prePtr = 0;
    }else{
        array->Top = 0;
    }
    array->length --;
    temp->prePtr = 0;
    temp->NextPtr = 0;
    _mem_putter(temp);
    return data;
}

void AL_Array_Clear(AL_Array* array){
    while (!AL_Array_isEmpty(array)){
        _mem_putter(AL_Array_Top_POP(array));
    }
}