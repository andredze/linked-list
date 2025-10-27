#ifndef LIST_H
#define LIST_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "common.h"
#include <stdint.h>

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

//==========================================================================================

#define LIST_CALL_DUMP(list_ctx_ptr, name, message)                                         \
        BEGIN                                                                               \
        ListDumpInfo_t dump_info = {LIST_SUCCESS, name, message, __PRETTY_FUNCTION__,       \
                                    __FILE__, __LINE__};                                    \
        SAFE_CALL(ListDump(list_ctx_ptr, &dump_info));                                      \
        END

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

//==========================================================================================

#else

#define LIST_CALL_DUMP(list_ctx_ptr, name, message)    ;
#define DEBUG_LIST_CHECK(list_ctx, reason)             ;

//==========================================================================================

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
    LIST_DATA_REALLOC_ERROR,
    LIST_LOGFILE_OPEN_ERROR,
    LIST_GRAPH_ERROR,
    LIST_CTX_NULL,
    LIST_DATA_NULL,
    LIST_CAPACITY_EXCEEDS_MAX,
    LIST_HEAD_NEGATIVE,
    LIST_HEAD_TOOBIG,
    LIST_TAIL_NEGATIVE,
    LIST_TAIL_TOOBIG,
    LIST_POSITION_NEGATIVE,
    LIST_POSITION_TOO_BIG,
    LIST_NO_SUCH_ELEMENT
} ListErr_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

//==========================================================================================

const char* const LIST_STR_ERRORS[] =
    {
    [LIST_SUCCESS]              = "LIST_SUCCESS",
    [LIST_DUMP_ERROR]           = "LIST_DUMP_ERROR",
    [LIST_CALLOC_ERROR]         = "LIST_CALLOC_ERROR",
    [LIST_DATA_REALLOC_ERROR]   = "LIST_DATA_REALLOC_ERROR",
    [LIST_LOGFILE_OPEN_ERROR]   = "LIST_LOGFILE_OPEN_ERROR",
    [LIST_GRAPH_ERROR]          = "LIST_GRAPH_ERROR",
    [LIST_CTX_NULL]             = "LIST_CTX_NULL",
    [LIST_DATA_NULL]            = "LIST_DATA_NULL",
    [LIST_CAPACITY_EXCEEDS_MAX] = "LIST_CAPACITY_EXCEEDS_MAX",
    [LIST_HEAD_NEGATIVE]        = "LIST_HEAD_NEGATIVE",
    [LIST_HEAD_TOOBIG]          = "LIST_HEAD_TOOBIG",
    [LIST_TAIL_NEGATIVE]        = "LIST_TAIL_NEGATIVE",
    [LIST_TAIL_TOOBIG]          = "LIST_TAIL_TOOBIG",
    [LIST_POSITION_NEGATIVE]    = "LIST_POSITION_NEGATIVE",
    [LIST_POSITION_TOO_BIG]     = "LIST_POSITION_TOO_BIG",
    [LIST_NO_SUCH_ELEMENT]      = "LIST_NO_SUCH_ELEMENT"
    };

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct ListDumpInfo
{
    ListErr_t   error;
    const char* image_name;
    const char* reason;
    const char* func;
    const char* file;
    int         line;
} ListDumpInfo_t;

//==========================================================================================

#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

const size_t LIST_MIN_CAPACITY = 64;
const size_t LIST_MAX_CAPACITY = 1024 * 1024 * 1024;

//——————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListCtor            (ListCtx_t* list_ctx, size_t capacity);
ListErr_t ListDtor            (ListCtx_t* list_ctx);
ListErr_t ListRealloc         (ListCtx_t* list_ctx);
ListErr_t ListInsertAfter     (ListCtx_t* list_ctx, int pos, elem_t value);
ListErr_t ListInsertBefore    (ListCtx_t* list_ctx, int pos, elem_t value);
ListErr_t ListPushFront       (ListCtx_t* list_ctx,          elem_t value);
ListErr_t ListPushBack        (ListCtx_t* list_ctx,          elem_t value);

#ifdef LIST_DEBUG
ListErr_t ListVerify          (ListCtx_t* list_ctx);
ListErr_t ListDump            (ListCtx_t* list_ctx, ListDumpInfo_t* dump_info);
ListErr_t ListCreateDumpGraph (ListCtx_t* list_ctx, const char* image_name);
#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_H */
