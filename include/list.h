#ifndef LIST_H
#define LIST_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "common.h"
#include <stdint.h>

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

#define DEBUG_LIST_CHECK(list_ctx, reason)                                              \
        BEGIN                                                                           \
        ListErr_t error = LIST_SUCCESS;                                                 \
        if ((error = ListVerify(list_ctx)))                                             \
        {                                                                               \
            ListDumpInfo_t dump_info = {error, "err_dump.png", reason,                  \
                                        __PRETTY_FUNCTION__, __FILE__, __LINE__};       \
            if (ListDump(list_ctx, &dump_info))                                         \
            {                                                                           \
                return LIST_DUMP_ERROR;                                                 \
            }                                                                           \
            return error;                                                               \
        }                                                                               \
        END

#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

typedef int elem_t;

#define SPEC "%d"

const elem_t LIST_POISON = 0xAB0BA;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct NodeCtx
{
    elem_t node;
    int    next;
    int    prev;
} NodeCtx_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct ListCtx
{
    int         head;
    int         tail;
    NodeCtx_t*  data;
    size_t      capacity;
    int         free;
} ListCtx_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef enum ListErr
{
    LIST_SUCCESS,
    LIST_DUMP_ERROR,
    LIST_CALLOC_ERROR,
    LIST_LOGFILE_OPEN_ERROR,
    LIST_CTX_NULL,
    LIST_DATA_NULL,
    LIST_CAPACITY_EXCEEDS_MAX,
    LIST_HEAD_NEGATIVE,
    LIST_HEAD_TOOBIG,
    LIST_TAIL_NEGATIVE,
    LIST_TAIL_TOOBIG
} ListErr_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

//==========================================================================================

typedef struct ListDumpInfo
{
    ListErr_t   error;
    const char* image_name;
    const char* reason;
    const char* func;
    const char* file;
    int         line;
} ListDumpInfo_t;

//——————————————————————————————————————————————————————————————————————————————————————————

const char* const LIST_STR_ERRORS[] =
    {
    [LIST_SUCCESS]              = "LIST_SUCCESS",
    [LIST_DUMP_ERROR]           = "LIST_DUMP_ERROR",
    [LIST_CALLOC_ERROR]         = "LIST_CALLOC_ERROR",
    [LIST_LOGFILE_OPEN_ERROR]   = "LIST_LOGFILE_OPEN_ERROR",
    [LIST_CTX_NULL]             = "LIST_CTX_NULL",
    [LIST_DATA_NULL]            = "LIST_DATA_NULL",
    [LIST_CAPACITY_EXCEEDS_MAX] = "LIST_CAPACITY_EXCEEDS_MAX",
    [LIST_HEAD_NEGATIVE]        = "LIST_HEAD_NEGATIVE",
    [LIST_HEAD_TOOBIG]          = "LIST_HEAD_TOOBIG",
    [LIST_TAIL_NEGATIVE]        = "LIST_TAIL_NEGATIVE",
    [LIST_TAIL_TOOBIG]          = "LIST_TAIL_TOOBIG"
    };

//==========================================================================================

#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

const size_t LIST_MIN_CAPACITY = 64;
const size_t LIST_MAX_CAPACITY = 1024 * 1024 * 1024;

//——————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListCtor   (ListCtx_t* list_ctx, size_t capacity);
ListErr_t ListDtor   (ListCtx_t* list_ctx);

#ifdef LIST_DEBUG
ListErr_t ListVerify (ListCtx_t* list_ctx);
ListErr_t ListDump   (ListCtx_t* list_ctx, ListDumpInfo_t* dump_info);
#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_H */
