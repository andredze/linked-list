#ifndef STD_LIST_DEBUG_H
#define STD_LIST_DEBUG_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "stdListTypes.h"
#include "stdListGraph.h"
#include <time.h>
#include <sys/stat.h>

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef STD_LIST_DEBUG

//==========================================================================================

#define STD_LIST_CALL_DUMP(list_ptr, name, message, arg)                                        \
        BEGIN                                                                                   \
        StdListDumpInfo_t dump_info = {STD_LIST_SUCCESS, name, message, __PRETTY_FUNCTION__,    \
                                    __FILE__, __LINE__, arg};                                   \
        if (StdListDump(list_ptr, &dump_info))                                                  \
        {                                                                                       \
            return STD_LIST_DUMP_ERROR;                                                         \
        }                                                                                       \
        END

#define DEBUG_STD_LIST_CHECK(list, message, arg)            \
        BEGIN                                               \
        StdListErr_t ret = STD_LIST_SUCCESS;                \
        if ((ret = StdListCheck(list, message,              \
                             __PRETTY_FUNCTION__,           \
                             __FILE__,                      \
                             __LINE__,                      \
                             arg)) != STD_LIST_SUCCESS)     \
        {                                                   \
            return ret;                                     \
        }                                                   \
        END

//==========================================================================================

#else

#define STD_LIST_CALL_DUMP(list_ptr, name, message, arg)    ;
#define DEBUG_STD_LIST_CHECK(list, message, arg)            ;

//==========================================================================================

#endif /* STD_LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef STD_LIST_DEBUG

int StdListSetDirectories(char* log_filename, char* image_dir, char* dot_dir);

StdListErr_t StdListCheck(StdList_t*     list,
                          const char* message,
                          const char* func,
                          const char* file,
                          int         line,
                          size_t         arg);

StdListErr_t StdListVerify          (StdList_t* list);
StdListErr_t StdListVerifyNext      (StdList_t* list, size_t* next_count_ptr);
StdListErr_t StdListVerifyPrev      (StdList_t* list, size_t* prev_count_ptr);
StdListErr_t StdListVerifyFree      (StdList_t* list, size_t* free_count_ptr);
StdListErr_t StdListDump            (StdList_t* list, StdListDumpInfo_t* dump_info);
int          StdListDumpStruct      (StdList_t* list, StdListDumpInfo_t* dump_info, FILE* fp);
int          StdListDumpData        (StdList_t* list, StdListDumpInfo_t* dump_info, FILE* fp);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* STD_LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* STD_LIST_DEBUG_H */
