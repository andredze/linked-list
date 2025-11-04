#ifndef STD_LIST_COMMANDS_H
#define STD_LIST_COMMANDS_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "stdListTypes.h"
#include "stdListDebug.h"

//——————————————————————————————————————————————————————————————————————————————————————————

StdListErr_t StdListCtor        (StdList_t* list, StdNode_t** head_node);
StdListErr_t StdListDtor        (StdList_t* list);

StdListErr_t StdListInsertAfter (StdList_t*  list,
                                 StdNode_t*  node,
                                 elem_t      value,
                                 StdNode_t** insert_node);

StdListErr_t StdListInsertBefore(StdList_t*  list,
                                 StdNode_t*  node,
                                 elem_t      value,
                                 StdNode_t** insert_node);

StdListErr_t StdListErase       (StdList_t* list,
                                 StdNode_t* node);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* STD_LIST_COMMANDS_H */
