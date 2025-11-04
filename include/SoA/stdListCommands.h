#ifndef STD_LIST_COMMANDS_H
#define STD_LIST_COMMANDS_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "stdListTypes.h"
#include "stdListDebug.h"

//——————————————————————————————————————————————————————————————————————————————————————————

typedef enum StdListInsertType
{
    STD_LIST_INSERT_AFTER,
    STD_LIST_INSERT_BEFORE
} StdListInsertType_t;

//——————————————————————————————————————————————————————————————————————————————————————————

StdListErr_t StdListCtor         (StdList_t* list, size_t capacity, int do_linear_realloc);
StdListErr_t StdListDtor         (StdList_t* list);
StdListErr_t StdListErase        (StdList_t* list, int  pos);
StdListErr_t StdListInsertAfter  (StdList_t* list, int  pos, elem_t  value, int* insert_pos);
StdListErr_t StdListInsertBefore (StdList_t* list, int  pos, elem_t  value, int* insert_pos);
StdListErr_t StdListGetHead      (StdList_t* list,           int*    head);
StdListErr_t StdListGetTail      (StdList_t* list,           int*    tail);
StdListErr_t StdListGetValue     (StdList_t* list, int  pos, elem_t* value);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* STD_LIST_COMMANDS_H */
