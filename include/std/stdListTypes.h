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
    struct StdNode* prev;
    elem_t          value;
    struct StdNode* next;
} StdNode_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct StdList
{
    StdNode_t* root;
    size_t     size;
} StdList_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef enum StdListErr
{
    STD_LIST_SUCCESS,
    STD_LIST_NULL,
    STD_LIST_NULL_NODE,
    STD_LIST_DUMP_ERROR,
    STD_LIST_CALLOC_ERROR,
    STD_LIST_HEAD_NULL,
    STD_LIST_SIZE_EXCEEDS_MAX,
    STD_LIST_LOOP,
    STD_NODE_PREV_NULL,
    STD_NODE_NEXT_NULL,
    STD_NODE_WRONG_PREV,
    STD_LIST_LOGFILE_OPEN_ERROR,
    STD_LIST_FILENAME_TOOBIG,
    STD_LIST_IMAGE_NAME_NULL,
    STD_LIST_NO_SUCH_ELEMENT,
    STD_LIST_SIZE_IS_WRONG,
} StdListErr_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef STD_LIST_DEBUG

//——————————————————————————————————————————————————————————————————————————————————————————

const char* const STD_LIST_STR_ERRORS[] =
{
    [STD_LIST_SUCCESS]            = "STD_LIST_SUCCESS",
    [STD_LIST_NULL]               = "STD_LIST_NULL",
    [STD_LIST_NULL_NODE]          = "STD_LIST_NULL_NODE",
    [STD_LIST_DUMP_ERROR]         = "STD_LIST_DUMP_ERROR",
    [STD_LIST_CALLOC_ERROR]       = "STD_LIST_CALLOC_ERROR",
    [STD_LIST_HEAD_NULL]          = "STD_LIST_HEAD_NULL",
    [STD_LIST_SIZE_EXCEEDS_MAX]   = "STD_LIST_SIZE_EXCEEDS_MAX",
    [STD_LIST_LOOP]               = "STD_LIST_LOOP",
    [STD_NODE_PREV_NULL]          = "STD_NODE_PREV_NULL",
    [STD_NODE_NEXT_NULL]          = "STD_NODE_NEXT_NULL",
    [STD_NODE_WRONG_PREV]         = "STD_NODE_WRONG_PREV",
    [STD_LIST_LOGFILE_OPEN_ERROR] = "STD_LIST_LOGFILE_OPEN_ERROR",
    [STD_LIST_FILENAME_TOOBIG]    = "STD_LIST_FILENAME_TOOBIG",
    [STD_LIST_IMAGE_NAME_NULL]    = "STD_LIST_IMAGE_NAME_NULL",
    [STD_LIST_NO_SUCH_ELEMENT]    = "STD_LIST_NO_SUCH_ELEMENT",
    [STD_LIST_SIZE_IS_WRONG]      = "STD_LIST_SIZE_IS_WRONG",
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
    size_t      command_arg;
} StdListDumpInfo_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* STD_LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

const size_t STD_LIST_MAX_SIZE = 1024 * 1024 * 1024;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* STD_LIST_TYPES_H */
