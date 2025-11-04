#include "listCommands.h"

//------------------------------------------------------------------------------------------

static ListErr_t ListInsert(List_t* list,
                            int     pos,
                            elem_t  value,
                            int*    insert_pos);

static ListErr_t ListCheckPos(List_t* list, int pos);

static ListErr_t ListRealloc       (List_t* list);
static ListErr_t ListReallocLinear (List_t* list);

//------------------------------------------------------------------------------------------

ListErr_t ListCtor(List_t* list, size_t capacity, int do_linear_realloc)
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

    list->do_linear_realloc = do_linear_realloc;
    list->is_sorted         = 1;

    list->capacity = capacity;
    list->size     = 0;
    list->free     = 1;

    /* Filling the null element */
    list->data[0].prev  = 0; /* tail */
    list->data[0].next  = 0; /* head */
    list->data[0].value = LIST_POISON;

    /* Filling the free list */
    for (int i = 1; i < (int) capacity; i++)
    {
        list->data[i].prev  = -1;
        list->data[i].value = LIST_POISON;
        list->data[i].next  = i + 1;
    }

    /* Last free element addresses to null */
    list->data[capacity - 1].next = -1;

    LIST_CALL_DUMP(list, "ctor", "DUMP_CTOR_CAP=", (int) capacity);

    DPRINTF("> End   ListCtor\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static ListErr_t ListCheckPos(List_t* list, int pos)
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

    /* Check that pos is in list */
    if (list->data[pos].prev == -1)
    {
        PRINTERR("List doesn't have an element with pos = %d", pos);
        return LIST_NO_SUCH_ELEMENT;
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

    if ((error = ListInsert(list, pos, value, insert_pos)))
    {
        return error;
    }

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

    /* Insert before position is equivalent to inserting after previous position */
    if ((error = ListInsert(list, list->data[pos].prev, value, insert_pos)))
    {
        return error;
    }

    DEBUG_LIST_CHECK(list, "END_INSERT_BEFORE_", pos);

    LIST_CALL_DUMP(list, "insert", "DUMP_INSERT_BEFORE_", pos);

    DPRINTF("> End   ListInsertBefore\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static ListErr_t ListInsert(List_t* list,
                            int     pos,
                            elem_t  value,
                            int*    insert_pos)
{
    assert(list       != NULL);
    assert(insert_pos != NULL);

    ListErr_t error = LIST_SUCCESS;

    if (list->free == -1)
    {
        error = list->do_linear_realloc ? ListReallocLinear(list) : ListRealloc(list);
        if (error != LIST_SUCCESS)
        {
            return error;
        }
    }

    if (pos != list->data[0].prev)
    {
        list->is_sorted = 0;
    }

    int cur_index = list->free;
    int pos_next  = list->data[pos].next;

    /* set new free element */
    list->free = list->data[list->free].next;

    /* connect previous element to current */
    list->data[pos].next      = cur_index;

    /* connect next element to current */
    list->data[pos_next].prev = cur_index;

    /* connect current element to previous and next */
    list->data[cur_index].prev  = pos;
    list->data[cur_index].value = value;
    list->data[cur_index].next  = pos_next;

    *insert_pos = cur_index;

    list->size++;

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static ListErr_t ListRealloc(List_t* list)
{
    DPRINTF("\t> Start ListRealloc()\n");

    DEBUG_LIST_CHECK(list, "REALLOC_START_OLDCAP=", (int) list->capacity);

    size_t  capacity = list->capacity * 2 + 1;

    Node_t* new_data = (Node_t*) realloc(list->data, capacity * sizeof(list->data[0]));

    if (new_data == NULL)
    {
        PRINTERR("List data realloc failed");
        return LIST_DATA_REALLOC_ERROR;
    }

    list->capacity = capacity;
    list->data     = new_data;

    list->free = (int) list->capacity;

    /* Filling the free list */
    for (int i = list->free; i < (int) list->capacity; i++)
    {
        list->data[i].prev  = -1;
        list->data[i].value = LIST_POISON;
        list->data[i].next  = i + 1;
    }

    /* Last free element addresses to nothing */
    list->data[list->capacity - 1].next = -1;

    DEBUG_LIST_CHECK(list, "REALLOC_END_OLDCAP=", (int) list->capacity);

    DPRINTF("\t> End   ListRealloc\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static ListErr_t ListReallocLinear(List_t* list)
{
    DPRINTF("\t> Start ListReallocLinear()\n");

    DEBUG_LIST_CHECK(list, "REALLOC_LINEAR_START_OLDCAP=", (int) list->capacity);

    size_t  capacity = list->capacity * 2 + 1;

    Node_t* new_data = (Node_t*) calloc(capacity, sizeof(list->data[0]));

    if (new_data == NULL)
    {
        PRINTERR("List data realloc failed");
        return LIST_DATA_REALLOC_ERROR;
    }

    new_data[0].next = 1;
    new_data[0].prev = (int) list->size;

    int ind = 1;

    for (int pos = list->data[0].next; pos > 0; pos = list->data[pos].next)
    {
        DPRINTF("ind = %d;\n", ind);
        new_data[ind].prev  = ind - 1;
        new_data[ind].value = list->data[pos].value;
        new_data[ind].next  = ind + 1;
        ind++;
    }

    new_data[list->size].next = 0;

    list->free = ind;

    for (; (size_t) ind < capacity; ind++)
    {
        new_data[ind].prev  = -1;
        new_data[ind].value = LIST_POISON;
        new_data[ind].next  = ind + 1;
    }

    new_data[capacity - 1].next = -1;

    free(list->data);

    list->capacity  = capacity;
    list->data      = new_data;
    list->is_sorted = 1;

    DEBUG_LIST_CHECK(list, "REALLOC_LINEAR_END_OLDCAP=", (int) list->capacity);

    DPRINTF("\t> End   ListReallocLinear\n");

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

    if (pos != list->data[0].prev)
    {
        list->is_sorted = 0;
    }

    int next_ind = list->data[pos].next;
    int prev_ind = list->data[pos].prev;

    /* Connect pos to free and set poisons */
    list->data[pos].prev  = -1;
    list->data[pos].value = LIST_POISON;
    list->data[pos].next  = list->free;

    list->free = pos;

    DPRINTF(R"(    prev_ind = %d;
    next_ind = %d;
    head     = %d;
    tail     = %d;
    pos      = %d;)",
    prev_ind,
    next_ind,
    list->data[0].next,
    list->data[0].prev,
    pos);

    list->data[prev_ind].next = next_ind;
    list->data[next_ind].prev = prev_ind;

    list->size--;

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

ListErr_t ListGetValue(List_t* list, int pos, elem_t* value)
{
    DPRINTF("> Start ListGetValue()\n");

    DEBUG_LIST_CHECK(list, "START_GET_VALUE_", pos);

    ListErr_t error = LIST_SUCCESS;
    if ((error = ListCheckPos(list, pos)) != LIST_SUCCESS)
    {
        return error;
    }

    *value = list->data[pos].value;

    DEBUG_LIST_CHECK(list, "END_GET_VALUE_IND=", pos);

    LIST_CALL_DUMP(list, "get_value", "END_GET_VALUE_IND=", pos);

    DPRINTF("> End   ListGetValue\n");

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
