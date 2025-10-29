#ifndef LIST_COMMANDS_H
#define LIST_COMMANDS_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "common.h"
#include "listTypes.h"
#include "listDebug.h"

//——————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListCtor            (ListCtx_t* list_ctx, size_t capacity);
ListErr_t ListDtor            (ListCtx_t* list_ctx);
ListErr_t ListErase           (ListCtx_t* list_ctx, int pos);
ListErr_t ListCheckPos        (ListCtx_t* list_ctx, int pos);
ListErr_t ListInsertAfter     (ListCtx_t* list_ctx, int pos, elem_t value);
ListErr_t ListInsertBefore    (ListCtx_t* list_ctx, int pos, elem_t value);
ListErr_t ListPushFront       (ListCtx_t* list_ctx,          elem_t value);
ListErr_t ListPushBack        (ListCtx_t* list_ctx,          elem_t value);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_COMMANDS_H */
