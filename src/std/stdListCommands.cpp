#include "stdListCommands.h"

//------------------------------------------------------------------------------------------

StdListErr_t StdListCtor(StdListData_t* list_data, StdNode_t** head_node)
{
    assert(head_node != NULL);

    DPRINTF("> Start StdListCtor()\n");

    if (list_data == NULL)
    {
        PRINTERR("STD_LIST_CTX_NULL");
        return STD_LIST_CTX_NULL;
    }

    list_data->head = (StdNode_t*) calloc(1, sizeof(StdNode_t));

    if (list_data->head == NULL)
    {
        PRINTERR("STD_LIST_CALLOC_ERROR");
        return STD_LIST_CALLOC_ERROR;
    }

    list_data->size = 0;

    /* Filling the null element */
    (*list_data->head).value = STD_LIST_POISON;
    (*list_data->head).next  = list_data->head;
    (*list_data->head).prev  = list_data->head;

    *head_node = list_data->head;

    // STD_LIST_CALL_DUMP(list, "ctor", "DUMP_CTOR_CAP=");

    DPRINTF("> End   StdListCtor\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListInsertAfter(StdListData_t* list_data,
                                StdNode_t*     node,
                                elem_t         value,
                                StdNode_t**    insert_node)
{
    assert(list_data   != NULL);
    assert(insert_node != NULL);

    DPRINTF("> Start StdListInsertAfter(node = %p, value = " SPEC ")\n", node, value);

    // DEBUG_STD_LIST_CHECK(list, "START_INSERT_AFTER_", pos);

    if (node == NULL)
    {
        PRINTERR("Given node nullptr");
        return STD_LIST_NULL_NODE;
    }

    StdNode_t* new_node = (StdNode_t*) calloc(1, sizeof(StdNode_t));
    if (new_node == NULL)
    {
        PRINTERR("Node memory allocation failed");
        return STD_LIST_CALLOC_ERROR;
    }

    new_node->value = value;
    new_node->prev  = node;
    new_node->next  = node->next;

    (*node->next).prev = new_node;
    node->next         = new_node;

    *insert_node = new_node;

    // DEBUG_STD_LIST_CHECK(list, "END_INSERT_AFTER_", pos);

    // STD_LIST_CALL_DUMP(list, "insert", "DUMP_INSERT_AFTER_", pos);

    DPRINTF("> End   StdListInsertAfter\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListInsertBefore(StdListData_t* list_data,
                                 StdNode_t*     node,
                                 elem_t         value,
                                 StdNode_t**    insert_node)
{
    DPRINTF("> Start StdListInsertBefore(node = %p, value = " SPEC ")\n", node, value);

    StdListErr_t error = STD_LIST_SUCCESS;

    if ((error = StdListInsertAfter(list_data, node->prev, value, insert_node)))
    {
        return error;
    }

    DPRINTF("> End   StdListInsertBefore\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

// static StdListErr_t StdListRealloc(StdListData_t* list)
// {
//     DPRINTF("\t> Start StdListRealloc()\n");
//
//     DEBUG_STD_LIST_CHECK(list, "REALLOC_START_OLDCAP=", (int) list->capacity);
//
//     size_t  capacity = list->capacity * 2 + 1;
//
//     StdNode_t* new_data = (StdNode_t*) realloc(list->data, capacity * sizeof(list->data[0]));
//
//     if (new_data == NULL)
//     {
//         PRINTERR("StdList data realloc failed");
//         return STD_LIST_DATA_REALLOC_ERROR;
//     }
//
//     list->capacity = capacity;
//     list->data     = new_data;
//
//     list->free = (int) list->capacity;
//
//     /* Filling the free list */
//     for (int i = list->free; i < (int) list->capacity; i++)
//     {
//         list->data[i].prev  = -1;
//         list->data[i].value = STD_LIST_POISON;
//         list->data[i].next  = i + 1;
//     }
//
//     /* Last free element addresses to nothing */
//     list->data[list->capacity - 1].next = -1;
//
//     DEBUG_STD_LIST_CHECK(list, "REALLOC_END_OLDCAP=", (int) list->capacity);
//
//     DPRINTF("\t> End   StdListRealloc\n");
//
//     return STD_LIST_SUCCESS;
// }
//
// //------------------------------------------------------------------------------------------
//
// static StdListErr_t StdListReallocLinear(StdListData_t* list)
// {
//     DPRINTF("\t> Start StdListReallocLinear()\n");
//
//     DEBUG_STD_LIST_CHECK(list, "REALLOC_LINEAR_START_OLDCAP=", (int) list->capacity);
//
//     size_t  capacity = list->capacity * 2 + 1;
//
//     StdNode_t* new_data = (StdNode_t*) calloc(capacity, sizeof(list->data[0]));
//
//     if (new_data == NULL)
//     {
//         PRINTERR("StdList data realloc failed");
//         return STD_LIST_DATA_REALLOC_ERROR;
//     }
//
//     new_data[0].next = 1;
//     new_data[0].prev = (int) list->size;
//
//     int ind = 1;
//
//     for (int pos = list->data[0].next; pos > 0; pos = list->data[pos].next)
//     {
//         DPRINTF("ind = %d;\n", ind);
//         new_data[ind].prev  = ind - 1;
//         new_data[ind].value = list->data[pos].value;
//         new_data[ind].next  = ind + 1;
//         ind++;
//     }
//
//     new_data[list->size].next = 0;
//
//     list->free = ind;
//
//     for (; (size_t) ind < capacity; ind++)
//     {
//         new_data[ind].prev  = -1;
//         new_data[ind].value = STD_LIST_POISON;
//         new_data[ind].next  = ind + 1;
//     }
//
//     new_data[capacity - 1].next = -1;
//
//     free(list->data);
//
//     list->capacity  = capacity;
//     list->data      = new_data;
//     list->is_sorted = 1;
//
//     DEBUG_STD_LIST_CHECK(list, "REALLOC_LINEAR_END_OLDCAP=", (int) list->capacity);
//
//     DPRINTF("\t> End   StdListReallocLinear\n");
//
//     return STD_LIST_SUCCESS;
// }
//
// //------------------------------------------------------------------------------------------
//
// StdListErr_t StdListErase(StdListData_t* list, int pos)
// {
//     DPRINTF("> Start StdListErase(pos = %d)\n", pos);
//
//     DEBUG_STD_LIST_CHECK(list, "START_ERASE_", pos);
//
//     StdListErr_t error = STD_LIST_SUCCESS;
//     if ((error = StdListCheckPos(list, pos)) != STD_LIST_SUCCESS)
//     {
//         return error;
//     }
//
//     if (pos != list->data[0].prev)
//     {
//         list->is_sorted = 0;
//     }
//
//     int next_ind = list->data[pos].next;
//     int prev_ind = list->data[pos].prev;
//
//     /* Connect pos to free and set poisons */
//     list->data[pos].prev  = -1;
//     list->data[pos].value = STD_LIST_POISON;
//     list->data[pos].next  = list->free;
//
//     list->free = pos;
//
//     DPRINTF(R"(    prev_ind = %d;
//     next_ind = %d;
//     head     = %d;
//     tail     = %d;
//     pos      = %d;)",
//     prev_ind,
//     next_ind,
//     list->data[0].next,
//     list->data[0].prev,
//     pos);
//
//     list->data[prev_ind].next = next_ind;
//     list->data[next_ind].prev = prev_ind;
//
//     list->size--;
//
//     DEBUG_STD_LIST_CHECK(list, "END_ERASE_", pos);
//
//     STD_LIST_CALL_DUMP(list, "erase", "DUMP_ERASE_", pos);
//
//     DPRINTF("> End   StdListErase\n\n");
//
//     return STD_LIST_SUCCESS;
// }
//
// //------------------------------------------------------------------------------------------
//
// StdListErr_t StdListGetHead(StdListData_t* list, int* head)
// {
//     DPRINTF("> Start StdListGetHead()\n");
//
//     DEBUG_STD_LIST_CHECK(list, "START_GET_HEAD_", 0);
//
//     *head = list->data[0].next;
//
//     DEBUG_STD_LIST_CHECK(list, "END_GET_HEAD_IND=", *head);
//
//     STD_LIST_CALL_DUMP(list, "get_head", "END_GET_HEAD_IND=", *head);
//
//     DPRINTF("> End   StdListGetHead\n");
//
//     return STD_LIST_SUCCESS;
// }
//
// //------------------------------------------------------------------------------------------
//
// StdListErr_t StdListGetTail(StdListData_t* list, int* tail)
// {
//     DPRINTF("> Start StdListGetTail()\n");
//
//     DEBUG_STD_LIST_CHECK(list, "START_GET_TAIL_", 0);
//
//     *tail = list->data[0].prev;
//
//     DEBUG_STD_LIST_CHECK(list, "END_GET_TAIL_IND=", *tail);
//
//     STD_LIST_CALL_DUMP(list, "get_tail", "END_GET_TAIL_IND=", *tail);
//
//     DPRINTF("> End   StdListGetTail\n");
//
//     return STD_LIST_SUCCESS;
// }
//
// //------------------------------------------------------------------------------------------
//
// StdListErr_t StdListGetValue(StdListData_t* list, int pos, elem_t* value)
// {
//     DPRINTF("> Start StdListGetValue()\n");
//
//     DEBUG_STD_LIST_CHECK(list, "START_GET_VALUE_", pos);
//
//     StdListErr_t error = STD_LIST_SUCCESS;
//     if ((error = StdListCheckPos(list, pos)) != STD_LIST_SUCCESS)
//     {
//         return error;
//     }
//
//     *value = list->data[pos].value;
//
//     DEBUG_STD_LIST_CHECK(list, "END_GET_VALUE_IND=", pos);
//
//     STD_LIST_CALL_DUMP(list, "get_value", "END_GET_VALUE_IND=", pos);
//
//     DPRINTF("> End   StdListGetValue\n");
//
//     return STD_LIST_SUCCESS;
// }

//------------------------------------------------------------------------------------------

StdListErr_t StdListDtor(StdListData_t* list_data)
{
    DPRINTF("> Start StdListDtor()\n");

    if (list_data == NULL)
    {
        PRINTERR("STD_LIST_CTX_NULL");
        return STD_LIST_CTX_NULL;
    }

    StdNode_t* node = list_data->head;
    StdNode_t* next = NULL;

    while (next != list_data->head)
    {
        DPRINTF("free node at %p with value = " SPEC ";\n", node, (*node).value);
        next = node->next;
        free(node);
        node = next;
    }

    DPRINTF("> End   StdListDtor\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------
