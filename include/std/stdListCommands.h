#ifndef STD_LIST_COMMANDS_H
#define STD_LIST_COMMANDS_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "stdListTypes.h"
#include "stdListDebug.h"

//——————————————————————————————————————————————————————————————————————————————————————————

StdListErr_t StdListCtor        (StdList_t* list, StdNode_t** root_node);
StdListErr_t StdListDtor        (StdList_t* list);

StdListErr_t StdListInsertAfter (StdList_t*  list,
                                 StdNode_t*  node,
                                 elem_t      value,
                                 StdNode_t** insert_node);

StdListErr_t StdListInsertBefore(StdList_t*  list,
                                 StdNode_t*  node,
                                 elem_t      value,
                                 StdNode_t** insert_node);

StdListErr_t StdListEraseElem   (StdList_t* list,
                                 StdNode_t* node);

StdListErr_t StdListGetHead     (StdList_t* list, StdNode_t** head);
StdListErr_t StdListGetTail     (StdList_t* list, StdNode_t** tail);
StdListErr_t StdListGetValue    (StdList_t* list, StdNode_t*  node, elem_t* value);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* STD_LIST_COMMANDS_H */
