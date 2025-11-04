#ifndef STD_LIST_COMMANDS_H
#define STD_LIST_COMMANDS_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "stdListTypes.h"
// #include "stdListDebug.h"

//——————————————————————————————————————————————————————————————————————————————————————————

StdListErr_t StdListCtor        (StdListData_t* list_data, StdNode_t** head_node);
StdListErr_t StdListDtor        (StdListData_t* list_data);

StdListErr_t StdListInsertAfter (StdListData_t* list_data,
                                 StdNode_t*     node,
                                 elem_t         value,
                                 StdNode_t**    insert_node);

StdListErr_t StdListInsertBefore(StdListData_t* list_data,
                                 StdNode_t*     node,
                                 elem_t         value,
                                 StdNode_t**    insert_node);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* STD_LIST_COMMANDS_H */
