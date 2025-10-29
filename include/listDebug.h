#ifndef LIST_DEBUG_H
#define LIST_DEBUG_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "common.h"
#include "listTypes.h"

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

//==========================================================================================

#define LIST_CALL_DUMP(list_ctx_ptr, name, message)                                         \
        BEGIN                                                                               \
        ListDumpInfo_t dump_info = {LIST_SUCCESS, name, message, __PRETTY_FUNCTION__,       \
                                    __FILE__, __LINE__};                                    \
        if (ListDump(list_ctx_ptr, &dump_info))                                             \
        {                                                                                   \
            ListDtor(list_ctx_ptr);                                                         \
            return EXIT_FAILURE;                                                            \
        }                                                                                   \
        END

#define DEBUG_LIST_CHECK(list_ctx, reason)                                              \
        BEGIN                                                                           \
        ListErr_t error = LIST_SUCCESS;                                                 \
        if ((error = ListVerify(list_ctx)))                                             \
        {                                                                               \
            PRINTERR("ListVerify not passed! Check \"list_log.htm\"");                  \
            ListDumpInfo_t dump_info = {error, "err_dump", reason,                      \
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

typedef enum ListErr
{
    LIST_SUCCESS,
    LIST_DUMP_ERROR,
    LIST_CALLOC_ERROR,
    LIST_DATA_REALLOC_ERROR,
    LIST_LOGFILE_OPEN_ERROR,
    LIST_FILENAME_TOOBIG,
    LIST_IMAGE_NAME_NULL,
    LIST_CTX_NULL,
    LIST_DATA_NULL,
    LIST_CAPACITY_EXCEEDS_MAX,
    LIST_HEAD_NEGATIVE,
    LIST_HEAD_TOOBIG,
    LIST_TAIL_NEGATIVE,
    LIST_TAIL_TOOBIG,
    LIST_FREE_NEGATIVE,
    LIST_FREE_TOOBIG,
    LIST_NEXT_NEGATIVE,
    LIST_NEXT_TOOBIG,
    LIST_PREV_NEGATIVE,
    LIST_PREV_TOOBIG,
    LIST_FREE_NEXT_NEGATIVE,
    LIST_FREE_NEXT_TOOBIG,
    LIST_FREE_PREV_NOT_NULL,
    LIST_POSITION_NEGATIVE,
    LIST_POSITION_TOO_BIG,
    LIST_NO_SUCH_ELEMENT,
    LIST_IS_EMPTY,
    LIST_MAIN_IS_CYCLED,
    LIST_FREE_IS_CYCLED,
    LIST_FREE_VALUE_NOT_PZN,
    LIST_FILLED_VALUE_IS_PZN
} ListErr_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

//——————————————————————————————————————————————————————————————————————————————————————————

const char* const LIST_STR_ERRORS[] =
{
    [LIST_SUCCESS]              = "LIST_SUCCESS",
    [LIST_DUMP_ERROR]           = "LIST_DUMP_ERROR",
    [LIST_CALLOC_ERROR]         = "LIST_CALLOC_ERROR",
    [LIST_DATA_REALLOC_ERROR]   = "LIST_DATA_REALLOC_ERROR",
    [LIST_LOGFILE_OPEN_ERROR]   = "LIST_LOGFILE_OPEN_ERROR",
    [LIST_FILENAME_TOOBIG]      = "LIST_FILENAME_TOOBIG",
    [LIST_IMAGE_NAME_NULL]      = "LIST_IMAGE_NAME_NULL",
    [LIST_CTX_NULL]             = "LIST_CTX_NULL",
    [LIST_DATA_NULL]            = "LIST_DATA_NULL",
    [LIST_CAPACITY_EXCEEDS_MAX] = "LIST_CAPACITY_EXCEEDS_MAX",
    [LIST_HEAD_NEGATIVE]        = "LIST_HEAD_NEGATIVE",
    [LIST_HEAD_TOOBIG]          = "LIST_HEAD_TOOBIG",
    [LIST_TAIL_NEGATIVE]        = "LIST_TAIL_NEGATIVE",
    [LIST_TAIL_TOOBIG]          = "LIST_TAIL_TOOBIG",
    [LIST_FREE_NEGATIVE]        = "LIST_FREE_NEGATIVE",
    [LIST_FREE_TOOBIG]          = "LIST_FREE_TOOBIG",
    [LIST_NEXT_NEGATIVE]        = "LIST_NEXT_NEGATIVE",
    [LIST_NEXT_TOOBIG]          = "LIST_NEXT_TOOBIG",
    [LIST_PREV_NEGATIVE]        = "LIST_PREV_NEGATIVE",
    [LIST_PREV_TOOBIG]          = "LIST_PREV_TOOBIG",
    [LIST_FREE_NEXT_NEGATIVE]   = "LIST_FREE_NEXT_NEGATIVE",
    [LIST_FREE_NEXT_TOOBIG]     = "LIST_FREE_NEXT_TOOBIG",
    [LIST_FREE_PREV_NOT_NULL]   = "LIST_FREE_PREV_NOT_NULL",
    [LIST_POSITION_NEGATIVE]    = "LIST_POSITION_NEGATIVE",
    [LIST_POSITION_TOO_BIG]     = "LIST_POSITION_TOO_BIG",
    [LIST_NO_SUCH_ELEMENT]      = "LIST_NO_SUCH_ELEMENT",
    [LIST_IS_EMPTY]             = "LIST_IS_EMPTY",
    [LIST_MAIN_IS_CYCLED]       = "LIST_MAIN_IS_CYCLED",
    [LIST_FREE_IS_CYCLED]       = "LIST_FREE_IS_CYCLED",
    [LIST_FREE_VALUE_NOT_PZN]   = "LIST_FREE_VALUE_NOT_PZN",
    [LIST_FILLED_VALUE_IS_PZN]  = "LIST_FILLED_VALUE_IS_PZN"
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

//——————————————————————————————————————————————————————————————————————————————————————————

int LinearSearch(int* array, size_t size, int elem);

ListErr_t ListVerify          (ListCtx_t* list_ctx);
ListErr_t ListDump            (ListCtx_t* list_ctx, ListDumpInfo_t* dump_info);
ListErr_t ListCreateDumpGraph (ListCtx_t* list_ctx, const char* image_name);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_DEBUG_H */
