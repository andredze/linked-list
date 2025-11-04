#ifndef LIST_DEBUG_H
#define LIST_DEBUG_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "listTypes.h"
#include "listGraph.h"
#include <time.h>
#include <sys/stat.h>

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

//==========================================================================================

#define LIST_CALL_DUMP(list_ptr, name, message, arg)                                    \
        BEGIN                                                                           \
        ListDumpInfo_t dump_info = {LIST_SUCCESS, name, message, __PRETTY_FUNCTION__,   \
                                    __FILE__, __LINE__, arg};                           \
        if (ListDump(list_ptr, &dump_info))                                             \
        {                                                                               \
            return LIST_DUMP_ERROR;                                                     \
        }                                                                               \
        END

#define DEBUG_LIST_CHECK(list, message, arg)            \
        BEGIN                                           \
        ListErr_t ret = LIST_SUCCESS;                   \
        if ((ret = ListCheck(list, message,             \
                             __PRETTY_FUNCTION__,       \
                             __FILE__,                  \
                             __LINE__,                  \
                             arg)) != LIST_SUCCESS)     \
        {                                               \
            return ret;                                 \
        }                                               \
        END

//==========================================================================================

#else

#define LIST_CALL_DUMP(list_ptr, name, message, arg)    ;
#define DEBUG_LIST_CHECK(list, message, arg)            ;

//==========================================================================================

#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

int LinearSearch(int* array, size_t size, int elem);

int SetDirectories(char* log_filename, char* image_dir, char* dot_dir);

ListErr_t ListCheck(List_t*     list,
                    const char* message,
                    const char* func,
                    const char* file,
                    int         line,
                    int         arg);

ListErr_t ListVerify          (List_t* list);
ListErr_t ListVerifyNext      (List_t* list, size_t* next_count_ptr);
ListErr_t ListVerifyPrev      (List_t* list, size_t* prev_count_ptr);
ListErr_t ListVerifyFree      (List_t* list, size_t* free_count_ptr);
ListErr_t ListDump            (List_t* list, ListDumpInfo_t* dump_info);
int       ListDumpStruct      (List_t* list, ListDumpInfo_t* dump_info, FILE* fp);
int       ListDumpData        (List_t* list, ListDumpInfo_t* dump_info, FILE* fp);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_DEBUG_H */
