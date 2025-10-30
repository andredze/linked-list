#include "listCommands.h"

//------------------------------------------------------------------------------------------

static ListErr_t ListRealloc(List_t* list);

//------------------------------------------------------------------------------------------

ListErr_t ListCtor(List_t* list, size_t capacity)
{
    DPRINTF("> Start ListCtor(capacity = %zu)\n", capacity);

    if (list == NULL)
    {
        PRINTERR("LIST_CTX_NULL");
        return LIST_CTX_NULL;
    }
    if (capacity > LIST_MAX_CAPACITY)
    {
        PRINTERR("LIST_CAPACITY_EXCEEDS_MAX");
        return LIST_CAPACITY_EXCEEDS_MAX;
    }

    if (capacity < LIST_MIN_CAPACITY)
    {
        capacity = LIST_MIN_CAPACITY;
    }

    list->data = (Node_t*) calloc(capacity, sizeof(Node_t));

    if (list->data == NULL)
    {
        PRINTERR("LIST_CALLOC_ERROR");
        return LIST_CALLOC_ERROR;
    }

    list->capacity = capacity;
    list->free     =  1;

    /* Filling the null element */
    list->data[0].prev  = 0; /* tail */
    list->data[0].next  = 0; /* head */
    list->data[0].value = LIST_POISON;

    /* Filling the free list */
    for (int i = 1; i < (int) capacity - 1; i++)
    {
        list->data[i].prev  = -1;
        list->data[i].value = LIST_POISON;
        list->data[i].next  = i + 1;
    }

    /* Last free element addresses to null */
    list->data[capacity - 1].prev  = -1;
    list->data[capacity - 1].value = LIST_POISON;
    list->data[capacity - 1].next  = 0;

    LIST_CALL_DUMP(list, "ctor", "DUMP_CTOR_CAP=", (int) capacity);

    DPRINTF("> End   ListCtor\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListCheckPos(List_t* list, int pos)
{
    assert(list != NULL);

    if (pos < 0)
    {
        PRINTERR("List position is negative");
        return LIST_POSITION_NEGATIVE;
    }
    if ((size_t) pos >= list->capacity)
    {
        PRINTERR("List position is too big");
        return LIST_POSITION_TOO_BIG;
    }

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListInsertAfter(List_t* list, int pos, elem_t value, int* insert_pos)
{
    DPRINTF("> Start ListInsertAfter(pos = %d, value = " SPEC ")\n", pos, value);

    DEBUG_LIST_CHECK(list, "START_INSERT_AFTER_", pos);

    ListErr_t error = LIST_SUCCESS;
    if ((error = ListCheckPos(list, pos)) != LIST_SUCCESS)
    {
        return error;
    }

    /* Check that pos is in list */
    if (list->data[pos].prev == -1)
    {
        PRINTERR("List doesn't have an element with pos = %d", pos);
        return LIST_NO_SUCH_ELEMENT;
    }

    if (list->free == 0)
    {
        if ((error = ListRealloc(list)) != LIST_SUCCESS)
        {
            return error;
        }
    }

    int cur_index = list->free;
    int pos_next  = list->data[pos].next;

    /* set new free element */
    list->free = list->data[list->free].next;

    /* connect previous element to current */
    list->data[pos].next        = cur_index;

    /* connect current element to previous and next */
    list->data[cur_index].prev  = pos;
    list->data[cur_index].value = value;
    list->data[cur_index].next  = pos_next;

    /* connect next element to current */
    list->data[pos_next].prev   = cur_index;

    *insert_pos = cur_index;

    DEBUG_LIST_CHECK(list, "END_INSERT_AFTER_", pos);

    LIST_CALL_DUMP(list, "insert", "DUMP_INSERT_AFTER_", pos);

    DPRINTF("> End   ListInsertAfter\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListInsertBefore(List_t* list, int pos, elem_t value, int* insert_pos)
{
    DPRINTF("> Start ListInsertBefore(pos = %d, value = " SPEC ")\n", pos, value);

    DEBUG_LIST_CHECK(list, "START_INSERT_BEFORE_", pos);

    ListErr_t error = LIST_SUCCESS;
    if ((error = ListCheckPos(list, pos)) != LIST_SUCCESS)
    {
        return error;
    }

    /* Check that pos is in list */
    if (list->data[pos].prev == -1)
    {
        PRINTERR("List doesn't have an element with pos = %d", pos);
        return LIST_NO_SUCH_ELEMENT;
    }

    if (list->free == 0)
    {
        if ((error = ListRealloc(list)) != LIST_SUCCESS)
        {
            return error;
        }
    }

    int cur_index = list->free;
    int pos_prev  = list->data[pos].prev;

    /* set new free element */
    list->free = list->data[list->free].next;

    /* connect previous element to current */
    list->data[pos_prev].next   = cur_index;

    /* connect current element to previous and next */
    list->data[cur_index].prev  = pos_prev;
    list->data[cur_index].value = value;
    list->data[cur_index].next  = pos;

    /* connect next element to current */
    list->data[pos].prev        = cur_index;

    *insert_pos = cur_index;

    DEBUG_LIST_CHECK(list, "END_INSERT_BEFORE_", pos);

    LIST_CALL_DUMP(list, "insert", "DUMP_INSERT_BEFORE_", pos);

    DPRINTF("> End   ListInsertBefore\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static ListErr_t ListRealloc(List_t* list)
{
    DPRINTF("\t> Start ListRealloc()\n");

    DEBUG_LIST_CHECK(list, "REALLOC_START_OLDCAP=", (int) list->capacity);

    size_t new_size = sizeof(list->data[0]) * (list->capacity * 2 + 1);

    Node_t* new_data = (Node_t*) realloc(list->data, new_size);

    if (new_data == NULL)
    {
        PRINTERR("List data realloc failed");
        return LIST_DATA_REALLOC_ERROR;
    }

    size_t old_capacity = list->capacity;
    list->capacity      = list->capacity * 2 + 1;
    list->data          = new_data;

    list->free = (int) old_capacity;

    /* Filling the free list */
    for (int i = (int) old_capacity; i < (int) list->capacity - 1; i++)
    {
        list->data[i].prev  = -1;
        list->data[i].value = LIST_POISON;
        list->data[i].next  = i + 1;
    }

    /* Last free element addresses to null */
    list->data[list->capacity - 1].prev  = -1;
    list->data[list->capacity - 1].value = LIST_POISON;
    list->data[list->capacity - 1].next  = 0;

    DEBUG_LIST_CHECK(list, "REALLOC_END_OLDCAP=", (int) list->capacity);

    DPRINTF("\t> End   ListRealloc\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListErase(List_t* list, int pos)
{
    DPRINTF("> Start ListErase(pos = %d)\n", pos);

    DEBUG_LIST_CHECK(list, "START_ERASE_", pos);

    ListErr_t error = LIST_SUCCESS;
    if ((error = ListCheckPos(list, pos)) != LIST_SUCCESS)
    {
        return error;
    }

    int next_ind = list->data[pos].next;
    int prev_ind = list->data[pos].prev;

    /* Connect pos to free and set poisons */
    list->data[pos].prev  = -1;
    list->data[pos].value = LIST_POISON;
    list->data[pos].next  = list->free;

    list->free = pos;

    DPRINTF("\tprev_ind = %d;\n"
            "\tnext_ind = %d;\n"
            "\thead     = %d;\n"
            "\ttail     = %d;\n"
            "\tpos      = %d;\n",
            prev_ind,
            next_ind,
            list->data[0].next,
            list->data[0].prev,
            pos);

    list->data[prev_ind].next = next_ind;
    list->data[next_ind].prev = prev_ind;

    DEBUG_LIST_CHECK(list, "END_ERASE_", pos);

    LIST_CALL_DUMP(list, "erase", "DUMP_ERASE_", pos);

    DPRINTF("> End   ListErase\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListGetHead(List_t* list, int* head)
{
    DPRINTF("> Start ListGetHead()\n");

    DEBUG_LIST_CHECK(list, "START_GET_HEAD_", 0);

    *head = list->data[0].next;

    DEBUG_LIST_CHECK(list, "END_GET_HEAD_IND=", *head);

    LIST_CALL_DUMP(list, "get_head", "END_GET_HEAD_IND=", *head);

    DPRINTF("> End   ListGetHead\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListGetTail(List_t* list, int* tail)
{
    DPRINTF("> Start ListGetTail()\n");

    DEBUG_LIST_CHECK(list, "START_GET_TAIL_", 0);

    *tail = list->data[0].prev;

    DEBUG_LIST_CHECK(list, "END_GET_TAIL_IND=", *tail);

    LIST_CALL_DUMP(list, "get_tail", "END_GET_TAIL_IND=", *tail);

    DPRINTF("> End   ListGetTail\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListDtor(List_t* list)
{
    DPRINTF("> Start ListDtor()\n");

    if (list == NULL)
    {
        PRINTERR("LIST_CTX_NULL");
        return LIST_CTX_NULL;
    }

    free(list->data);

    DPRINTF("> End   ListDtor\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------
