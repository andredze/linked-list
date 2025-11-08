#include "stdListCommands.h"

//------------------------------------------------------------------------------------------

StdListErr_t StdListCtor(StdList_t* list, StdNode_t** root_node)
{
    DPRINTF("> Start StdListCtor()\n");

    if (list == NULL)
    {
        PRINTERR("STD_LIST_NULL");
        return STD_LIST_NULL;
    }

    list->root = (StdNode_t*) calloc(1, sizeof(StdNode_t));

    if (list->root == NULL)
    {
        PRINTERR("STD_LIST_CALLOC_ERROR");
        return STD_LIST_CALLOC_ERROR;
    }

    list->size = 0;

    /* Filling the null element */
    (*list->root).value = STD_LIST_POISON;
    (*list->root).next  = list->root;
    (*list->root).prev  = list->root;

    if (root_node != NULL)
    {
        *root_node = list->root;
    }

    STD_LIST_CALL_DUMP(list, "ctor", "DUMP_CTOR_CAP=", 0);

    DPRINTF("> End   StdListCtor\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListInsertAfter(StdList_t*  list,
                                StdNode_t*  node,
                                elem_t      value,
                                StdNode_t** insert_node)
{
    assert(list != NULL);

    DPRINTF("> Start StdListInsertAfter(node = %p, value = " SPEC ")\n", node, value);

    DEBUG_STD_LIST_CHECK(list, "START_INSERT_AFTER_", 0);

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

    if (insert_node != NULL)
    {
        *insert_node = new_node;
    }

    list->size++;

    DEBUG_STD_LIST_CHECK(list, "END_INSERT_AFTER_", 0);

    STD_LIST_CALL_DUMP(list, "insert", "DUMP_INSERT_AFTER_", (size_t) node);

    DPRINTF("> End   StdListInsertAfter\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListInsertBefore(StdList_t*  list,
                                 StdNode_t*  node,
                                 elem_t      value,
                                 StdNode_t** insert_node)
{
    DPRINTF("> Start StdListInsertBefore(node = %p, value = " SPEC ")\n", node, value);

    StdListErr_t error = STD_LIST_SUCCESS;

    if ((error = StdListInsertAfter(list, node->prev, value, insert_node)))
    {
        return error;
    }

    DPRINTF("> End   StdListInsertBefore\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListEraseElem(StdList_t* list, StdNode_t* node)
{
    DPRINTF("> Start StdListErase(node = %p)\n", node);

    DEBUG_STD_LIST_CHECK(list, "START_ERASE_", 0);

    if (node == NULL)
    {
        return STD_LIST_NULL_NODE;
    }

    StdNode_t* next = node->next;
    StdNode_t* prev = node->prev;

    next->prev = prev;
    prev->next = next;

    node->value = STD_LIST_POISON;
    node->prev  = NULL;
    node->next  = NULL;

    free(node);

    list->size--;

    DEBUG_STD_LIST_CHECK(list, "END_ERASE_", 0);

    STD_LIST_CALL_DUMP(list, "erase", "DUMP_ERASE_", 0);

    DPRINTF("> End   StdListErase\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListGetHead(StdList_t* list, StdNode_t** head)
{
    DPRINTF("> Start StdListGetHead()\n");

    DEBUG_STD_LIST_CHECK(list, "START_GET_HEAD_", 0);

    *head = list->root->next;

    DPRINTF("> End   StdListGetHead\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListGetTail(StdList_t* list, StdNode_t** tail)
{
    DPRINTF("> Start StdListGetTail()\n");

    DEBUG_STD_LIST_CHECK(list, "START_GET_TAIL_", 0);

    *tail = list->root->prev;

    DPRINTF("> End   StdListGetTail\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

#ifdef STD_LIST_DEBUG
StdListErr_t StdListGetValue(StdList_t* list, StdNode_t* node, elem_t* value)
#else
StdListErr_t StdListGetValue(StdNode_t* node, elem_t* value)
#endif
{
    DPRINTF("> Start StdListGetValue()\n");

    DEBUG_STD_LIST_CHECK(list, "START_GET_VALUE_", 0);

    if (node == NULL)
    {
        PRINTERR("Given node is a nullptr");
        return STD_LIST_NULL_NODE;
    }

    *value = node->value;

    DPRINTF("> End   StdListGetValue\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListDtor(StdList_t* list)
{
    DPRINTF("> Start StdListDtor()\n");

    if (list == NULL)
    {
        PRINTERR("STD_LIST_NULL");
        return STD_LIST_NULL;
    }

    StdNode_t* node = list->root;
    StdNode_t* next = NULL;

    while (next != list->root)
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
