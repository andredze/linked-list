#ifndef LIST_DEBUG_H
#define LIST_DEBUG_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "common.h"
#include "listTypes.h"

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

//==========================================================================================

#define LIST_CALL_DUMP(list_ptr, name, message, arg)                                    \
        BEGIN                                                                           \
        ListDumpInfo_t dump_info = {LIST_SUCCESS, name, message, __PRETTY_FUNCTION__,   \
                                    __FILE__, __LINE__, arg};                           \
        if (ListDump(list_ptr, &dump_info))                                             \
        {                                                                               \
            ListDtor(list_ptr);                                                         \
            return LIST_DUMP_ERROR;                                                     \
        }                                                                               \
        END

#define DEBUG_LIST_CHECK(list, reason, arg)                                             \
        BEGIN                                                                           \
        ListErr_t verify_status = LIST_SUCCESS;                                         \
        if ((verify_status = ListVerify(list)))                                         \
        {                                                                               \
            PRINTERR("%s (ListVerify not passed! Check \"list_log.htm\")",              \
                     LIST_STR_ERRORS[verify_status]);                                   \
            ListDumpInfo_t dump_info = {verify_status, "err_dump", reason,              \
                                        __PRETTY_FUNCTION__, __FILE__, __LINE__, arg};  \
            if (ListDump(list, &dump_info))                                             \
            {                                                                           \
                return LIST_DUMP_ERROR;                                                 \
            }                                                                           \
            return verify_status;                                                       \
        }                                                                               \
        END

//==========================================================================================

#else

#define LIST_CALL_DUMP(list_ptr, name, message, arg)    ;
#define DEBUG_LIST_CHECK(list, reason, arg)             ;

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
    int         command_arg;
} ListDumpInfo_t;

//——————————————————————————————————————————————————————————————————————————————————————————

int LinearSearch(int* array, size_t size, int elem);

ListErr_t ListVerify          (List_t* list);
ListErr_t ListDump            (List_t* list, ListDumpInfo_t* dump_info);
ListErr_t ListCreateDumpGraph (List_t* list, const char* image_name);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_DEBUG_H */
