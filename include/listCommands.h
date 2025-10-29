#ifndef LIST_COMMANDS_H
#define LIST_COMMANDS_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "common.h"
#include "listTypes.h"
#include "listDebug.h"

//——————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListCtor            (List_t* list, size_t capacity);
ListErr_t ListDtor            (List_t* list);
ListErr_t ListErase           (List_t* list, int  pos);
ListErr_t ListCheckPos        (List_t* list, int  pos);
ListErr_t ListInsertAfter     (List_t* list, int  pos, elem_t value, int* insert_pos);
ListErr_t ListInsertBefore    (List_t* list, int  pos, elem_t value, int* insert_pos);
ListErr_t ListGetHead         (List_t* list, int* head);
ListErr_t ListGetTail         (List_t* list, int* tail);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_COMMANDS_H */
