#ifndef LIST_H
#define LIST_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "colors.h"

//——————————————————————————————————————————————————————————————————————————————————————————

#define PRINTERR(error) fprintf(stderr, RED "ERROR: %s in %s:%d from %s\n" RESET_CLR, \
                                error, __FILE__, __LINE__, __PRETTY_FUNCTION__);

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG
    #define DPRINTF(text, ...) fprintf(stderr, YELLOW text RESET_CLR, __VA_ARGS__);
#else
    #define DPRINTF(...) ;
#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

typedef int elem_t;

#define SPEC "%d"

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct NodeCtx
{
    elem_t* next;
    elem_t node;
    elem_t* prev;
} NodeCtx_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct ListCtx
{
    elem_t* head;
    NodeCtx_t* data;
    elem_t* tail;
} ListCtx_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef enum ListErr
{
    LIST_SUCCESS,
    LIST_CTX_NULL,
    LIST_CAPACITY_EXCEEDS_MAX,
    LIST_CALLOC_ERROR
} ListErr_t;

//——————————————————————————————————————————————————————————————————————————————————————————

const size_t LIST_MIN_CAPACITY = 64;
const size_t LIST_MAX_CAPACITY = 1024 * 1024 * 1024;

//——————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListCtor   (ListCtx_t* list_ctx, size_t capacity);
// ListErr_t ListVerify (ListCtx_t* list_ctx);
// ListErr_t ListDump   (ListCtx_t* list_ctx);
ListErr_t ListDtor   (ListCtx_t* list_ctx);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_H */
