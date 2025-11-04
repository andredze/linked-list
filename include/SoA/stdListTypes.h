#ifndef STD_LIST_TYPES_H
#define STD_LIST_TYPES_H

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef STD_LIST_DEBUG
    #define DEBUG
#endif

//——————————————————————————————————————————————————————————————————————————————————————————

#include "common.h"
#include <stdint.h>

//——————————————————————————————————————————————————————————————————————————————————————————

typedef int elem_t;

#define SPEC "%d"

const elem_t STD_LIST_POISON = 0xAB0BA;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct StdNode
{
    elem_t value;
    int    next;
    int    prev;
} StdNode_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct StdList
{
    StdNode_t*  data;
    size_t   capacity;
    size_t   size;
    int      free;
    int      do_linear_realloc;
    int      is_sorted;
} StdList_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef enum StdListErr
{
    STD_LIST_SUCCESS,
    STD_LIST_DUMP_ERROR,
    STD_LIST_CALLOC_ERROR,
    STD_LIST_DATA_REALLOC_ERROR,
    STD_LIST_LOGFILE_OPEN_ERROR,
    STD_LIST_FILENAME_TOOBIG,
    STD_LIST_IMAGE_NAME_NULL,
    STD_LIST_CTX_NULL,
    STD_LIST_DATA_NULL,
    STD_LIST_CAPACITY_EXCEEDS_MAX,
    STD_LIST_HEAD_NEGATIVE,
    STD_LIST_HEAD_TOOBIG,
    STD_LIST_TAIL_NEGATIVE,
    STD_LIST_TAIL_TOOBIG,
    STD_LIST_FREE_NEGATIVE,
    STD_LIST_FREE_TOOBIG,
    STD_LIST_NEXT_NEGATIVE,
    STD_LIST_NEXT_TOOBIG,
    STD_LIST_PREV_NEGATIVE,
    STD_LIST_PREV_TOOBIG,
    STD_LIST_FREE_NEXT_NEGATIVE,
    STD_LIST_FREE_NEXT_TOOBIG,
    STD_LIST_FREE_PREV_NOT_NULL,
    STD_LIST_POSITION_NEGATIVE,
    STD_LIST_POSITION_TOO_BIG,
    STD_LIST_NO_SUCH_ELEMENT,
    STD_LIST_NEXT_IS_CYCLED,
    STD_LIST_PREV_IS_CYCLED,
    STD_LIST_FREE_IS_CYCLED,
    STD_LIST_FREE_VALUE_NOT_PZN,
    STD_LIST_FILLED_VALUE_IS_PZN,
    STD_LIST_SIZE_EXCEEDS_CAPACITY,
    STD_LIST_SIZE_IS_WRONG,
    STD_LIST_CAP_IS_WRONG,
    STD_LIST_NEXT_WRONG,
    STD_LIST_PREV_WRONG,
    STD_LIST_FLAG_IS_WRONG
} StdListErr_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef STD_LIST_DEBUG

//——————————————————————————————————————————————————————————————————————————————————————————

const char* const STD_LIST_STR_ERRORS[] =
{
    [STD_LIST_SUCCESS]               = "STD_LIST_SUCCESS",
    [STD_LIST_DUMP_ERROR]            = "STD_LIST_DUMP_ERROR",
    [STD_LIST_CALLOC_ERROR]          = "STD_LIST_CALLOC_ERROR",
    [STD_LIST_DATA_REALLOC_ERROR]    = "STD_LIST_DATA_REALLOC_ERROR",
    [STD_LIST_LOGFILE_OPEN_ERROR]    = "STD_LIST_LOGFILE_OPEN_ERROR",
    [STD_LIST_FILENAME_TOOBIG]       = "STD_LIST_FILENAME_TOOBIG",
    [STD_LIST_IMAGE_NAME_NULL]       = "STD_LIST_IMAGE_NAME_NULL",
    [STD_LIST_CTX_NULL]              = "STD_LIST_CTX_NULL",
    [STD_LIST_DATA_NULL]             = "STD_LIST_DATA_NULL",
    [STD_LIST_CAPACITY_EXCEEDS_MAX]  = "STD_LIST_CAPACITY_EXCEEDS_MAX",
    [STD_LIST_HEAD_NEGATIVE]         = "STD_LIST_HEAD_NEGATIVE",
    [STD_LIST_HEAD_TOOBIG]           = "STD_LIST_HEAD_TOOBIG",
    [STD_LIST_TAIL_NEGATIVE]         = "STD_LIST_TAIL_NEGATIVE",
    [STD_LIST_TAIL_TOOBIG]           = "STD_LIST_TAIL_TOOBIG",
    [STD_LIST_FREE_NEGATIVE]         = "STD_LIST_FREE_NEGATIVE",
    [STD_LIST_FREE_TOOBIG]           = "STD_LIST_FREE_TOOBIG",
    [STD_LIST_NEXT_NEGATIVE]         = "STD_LIST_NEXT_NEGATIVE",
    [STD_LIST_NEXT_TOOBIG]           = "STD_LIST_NEXT_TOOBIG",
    [STD_LIST_PREV_NEGATIVE]         = "STD_LIST_PREV_NEGATIVE",
    [STD_LIST_PREV_TOOBIG]           = "STD_LIST_PREV_TOOBIG",
    [STD_LIST_FREE_NEXT_NEGATIVE]    = "STD_LIST_FREE_NEXT_NEGATIVE",
    [STD_LIST_FREE_NEXT_TOOBIG]      = "STD_LIST_FREE_NEXT_TOOBIG",
    [STD_LIST_FREE_PREV_NOT_NULL]    = "STD_LIST_FREE_PREV_NOT_NULL",
    [STD_LIST_POSITION_NEGATIVE]     = "STD_LIST_POSITION_NEGATIVE",
    [STD_LIST_POSITION_TOO_BIG]      = "STD_LIST_POSITION_TOO_BIG",
    [STD_LIST_NO_SUCH_ELEMENT]       = "STD_LIST_NO_SUCH_ELEMENT",
    [STD_LIST_NEXT_IS_CYCLED]        = "STD_LIST_NEXT_IS_CYCLED",
    [STD_LIST_PREV_IS_CYCLED]        = "STD_LIST_PREV_IS_CYCLED",
    [STD_LIST_FREE_IS_CYCLED]        = "STD_LIST_FREE_IS_CYCLED",
    [STD_LIST_FREE_VALUE_NOT_PZN]    = "STD_LIST_FREE_VALUE_NOT_PZN",
    [STD_LIST_FILLED_VALUE_IS_PZN]   = "STD_LIST_FILLED_VALUE_IS_PZN",
    [STD_LIST_SIZE_EXCEEDS_CAPACITY] = "STD_LIST_SIZE_EXCEEDS_CAPACITY",
    [STD_LIST_SIZE_IS_WRONG]         = "STD_LIST_SIZE_IS_WRONG",
    [STD_LIST_CAP_IS_WRONG]          = "STD_LIST_CAP_IS_WRONG",
    [STD_LIST_NEXT_WRONG]            = "STD_LIST_NEXT_WRONG",
    [STD_LIST_PREV_WRONG]            = "STD_LIST_PREV_WRONG",
    [STD_LIST_FLAG_IS_WRONG]         = "STD_LIST_FLAG_IS_WRONG"
};

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct StdListDumpInfo
{
    StdListErr_t   error;
    const char* image_name;
    const char* reason;
    const char* func;
    const char* file;
    int         line;
    int         command_arg;
} StdListDumpInfo_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* STD_LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

const size_t STD_LIST_MIN_CAPACITY = 4;
const size_t STD_LIST_MAX_CAPACITY = 1024 * 1024 * 1024;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* STD_LIST_TYPES_H */
