#include "stdListCommands.h"

//------------------------------------------------------------------------------------------

static StdListErr_t StdListInsert  (StdList_t* list, int pos, elem_t value,
                              int* insert_pos, StdListInsertType_t type);

static StdListErr_t StdListCheckPos(StdList_t* list, int pos);

static StdListErr_t StdListRealloc       (StdList_t* list);
static StdListErr_t StdListReallocLinear (StdList_t* list);

//------------------------------------------------------------------------------------------

StdListErr_t StdListCtor(StdList_t* list, size_t capacity, int do_linear_realloc)
{
    DPRINTF("> Start StdListCtor(capacity = %zu)\n", capacity);

    if (list == NULL)
    {
        PRINTERR("STD_LIST_CTX_NULL");
        return STD_LIST_CTX_NULL;
    }
    if (capacity > STD_LIST_MAX_CAPACITY)
    {
        PRINTERR("STD_LIST_CAPACITY_EXCEEDS_MAX");
        return STD_LIST_CAPACITY_EXCEEDS_MAX;
    }

    if (capacity < STD_LIST_MIN_CAPACITY)
    {
        capacity = STD_LIST_MIN_CAPACITY;
    }

    list->data = (StdNode_t*) calloc(capacity, sizeof(StdNode_t));

    if (list->data == NULL)
    {
        PRINTERR("STD_LIST_CALLOC_ERROR");
        return STD_LIST_CALLOC_ERROR;
    }

    list->do_linear_realloc = do_linear_realloc;
    list->is_sorted         = 1;

    list->capacity = capacity;
    list->size     = 0;
    list->free     = 1;

    /* Filling the null element */
    list->data[0].prev  = 0; /* tail */
    list->data[0].next  = 0; /* head */
    list->data[0].value = STD_LIST_POISON;

    /* Filling the free list */
    for (int i = 1; i < (int) capacity; i++)
    {
        list->data[i].prev  = -1;
        list->data[i].value = STD_LIST_POISON;
        list->data[i].next  = i + 1;
    }

    /* Last free element addresses to null */
    list->data[capacity - 1].next = -1;

    STD_LIST_CALL_DUMP(list, "ctor", "DUMP_CTOR_CAP=", (int) capacity);

    DPRINTF("> End   StdListCtor\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static StdListErr_t StdListCheckPos(StdList_t* list, int pos)
{
    assert(list != NULL);

    if (pos < 0)
    {
        PRINTERR("StdList position is negative");
        return STD_LIST_POSITION_NEGATIVE;
    }
    if ((size_t) pos >= list->capacity)
    {
        PRINTERR("StdList position is too big");
        return STD_LIST_POSITION_TOO_BIG;
    }

    /* Check that pos is in list */
    if (list->data[pos].prev == -1)
    {
        PRINTERR("StdList doesn't have an element with pos = %d", pos);
        return STD_LIST_NO_SUCH_ELEMENT;
    }

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListInsertAfter(StdList_t* list, int pos, elem_t value, int* insert_pos)
{
    DPRINTF("> Start StdListInsertAfter(pos = %d, value = " SPEC ")\n", pos, value);

    DEBUG_STD_LIST_CHECK(list, "START_INSERT_AFTER_", pos);

    StdListErr_t error = STD_LIST_SUCCESS;
    if (StdListInsert(list, pos, value, insert_pos, STD_LIST_INSERT_AFTER))
    {
        return error;
    }

    DEBUG_STD_LIST_CHECK(list, "END_INSERT_AFTER_", pos);

    STD_LIST_CALL_DUMP(list, "insert", "DUMP_INSERT_AFTER_", pos);

    DPRINTF("> End   StdListInsertAfter\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListInsertBefore(StdList_t* list, int pos, elem_t value, int* insert_pos)
{
    DPRINTF("> Start StdListInsertBefore(pos = %d, value = " SPEC ")\n", pos, value);

    DEBUG_STD_LIST_CHECK(list, "START_INSERT_BEFORE_", pos);

    StdListErr_t error = STD_LIST_SUCCESS;
    if (StdListInsert(list, pos, value, insert_pos, STD_LIST_INSERT_BEFORE))
    {
        return error;
    }

    DEBUG_STD_LIST_CHECK(list, "END_INSERT_BEFORE_", pos);

    STD_LIST_CALL_DUMP(list, "insert", "DUMP_INSERT_BEFORE_", pos);

    DPRINTF("> End   StdListInsertBefore\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static StdListErr_t StdListInsert(StdList_t* list, int pos, elem_t value,
                            int* insert_pos, StdListInsertType_t type)
{
    assert(list       != NULL);
    assert(insert_pos != NULL);

    StdListErr_t error = STD_LIST_SUCCESS;
    if ((error = StdListCheckPos(list, pos)) != STD_LIST_SUCCESS)
    {
        return error;
    }

    if (list->free == -1)
    {
        error = list->do_linear_realloc ? StdListReallocLinear(list) : StdListRealloc(list);
        if (error != STD_LIST_SUCCESS)
        {
            return error;
        }
    }

    if (pos != list->data[0].prev)
    {
        list->is_sorted = 0;
    }

    int cur_index = list->free;
    int pos_prev = list->data[pos].prev;
    int pos_next = list->data[pos].next;

    /* set new free element */
    list->free = list->data[list->free].next;

    if (type == STD_LIST_INSERT_BEFORE)
    {
        /* connect previous element to current */
        list->data[pos_prev].next = cur_index;

        /* connect next element to current */
        list->data[pos].prev      = cur_index;
    }
    else
    {
        /* connect previous element to current */
        list->data[pos].next      = cur_index;

        /* connect next element to current */
        list->data[pos_next].prev = cur_index;
    }

    /* connect current element to previous and next */
    list->data[cur_index].prev  = type == STD_LIST_INSERT_BEFORE ? pos_prev : pos;
    list->data[cur_index].value = value;
    list->data[cur_index].next  = type == STD_LIST_INSERT_AFTER  ? pos_next : pos;

    *insert_pos = cur_index;

    list->size++;

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static StdListErr_t StdListRealloc(StdList_t* list)
{
    DPRINTF("\t> Start StdListRealloc()\n");

    DEBUG_STD_LIST_CHECK(list, "REALLOC_START_OLDCAP=", (int) list->capacity);

    size_t  capacity = list->capacity * 2 + 1;

    StdNode_t* new_data = (StdNode_t*) realloc(list->data, capacity * sizeof(list->data[0]));

    if (new_data == NULL)
    {
        PRINTERR("StdList data realloc failed");
        return STD_LIST_DATA_REALLOC_ERROR;
    }

    list->capacity = capacity;
    list->data     = new_data;

    list->free = (int) list->capacity;

    /* Filling the free list */
    for (int i = list->free; i < (int) list->capacity; i++)
    {
        list->data[i].prev  = -1;
        list->data[i].value = STD_LIST_POISON;
        list->data[i].next  = i + 1;
    }

    /* Last free element addresses to nothing */
    list->data[list->capacity - 1].next = -1;

    DEBUG_STD_LIST_CHECK(list, "REALLOC_END_OLDCAP=", (int) list->capacity);

    DPRINTF("\t> End   StdListRealloc\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static StdListErr_t StdListReallocLinear(StdList_t* list)
{
    DPRINTF("\t> Start StdListReallocLinear()\n");

    DEBUG_STD_LIST_CHECK(list, "REALLOC_LINEAR_START_OLDCAP=", (int) list->capacity);

    size_t  capacity = list->capacity * 2 + 1;

    StdNode_t* new_data = (StdNode_t*) calloc(capacity, sizeof(list->data[0]));

    if (new_data == NULL)
    {
        PRINTERR("StdList data realloc failed");
        return STD_LIST_DATA_REALLOC_ERROR;
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
        new_data[ind].value = STD_LIST_POISON;
        new_data[ind].next  = ind + 1;
    }

    new_data[capacity - 1].next = -1;

    free(list->data);

    list->capacity  = capacity;
    list->data      = new_data;
    list->is_sorted = 1;

    DEBUG_STD_LIST_CHECK(list, "REALLOC_LINEAR_END_OLDCAP=", (int) list->capacity);

    DPRINTF("\t> End   StdListReallocLinear\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListErase(StdList_t* list, int pos)
{
    DPRINTF("> Start StdListErase(pos = %d)\n", pos);

    DEBUG_STD_LIST_CHECK(list, "START_ERASE_", pos);

    StdListErr_t error = STD_LIST_SUCCESS;
    if ((error = StdListCheckPos(list, pos)) != STD_LIST_SUCCESS)
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
    list->data[pos].value = STD_LIST_POISON;
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

    DEBUG_STD_LIST_CHECK(list, "END_ERASE_", pos);

    STD_LIST_CALL_DUMP(list, "erase", "DUMP_ERASE_", pos);

    DPRINTF("> End   StdListErase\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListGetHead(StdList_t* list, int* head)
{
    DPRINTF("> Start StdListGetHead()\n");

    DEBUG_STD_LIST_CHECK(list, "START_GET_HEAD_", 0);

    *head = list->data[0].next;

    DEBUG_STD_LIST_CHECK(list, "END_GET_HEAD_IND=", *head);

    STD_LIST_CALL_DUMP(list, "get_head", "END_GET_HEAD_IND=", *head);

    DPRINTF("> End   StdListGetHead\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListGetTail(StdList_t* list, int* tail)
{
    DPRINTF("> Start StdListGetTail()\n");

    DEBUG_STD_LIST_CHECK(list, "START_GET_TAIL_", 0);

    *tail = list->data[0].prev;

    DEBUG_STD_LIST_CHECK(list, "END_GET_TAIL_IND=", *tail);

    STD_LIST_CALL_DUMP(list, "get_tail", "END_GET_TAIL_IND=", *tail);

    DPRINTF("> End   StdListGetTail\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListGetValue(StdList_t* list, int pos, elem_t* value)
{
    DPRINTF("> Start StdListGetValue()\n");

    DEBUG_STD_LIST_CHECK(list, "START_GET_VALUE_", pos);

    StdListErr_t error = STD_LIST_SUCCESS;
    if ((error = StdListCheckPos(list, pos)) != STD_LIST_SUCCESS)
    {
        return error;
    }

    *value = list->data[pos].value;

    DEBUG_STD_LIST_CHECK(list, "END_GET_VALUE_IND=", pos);

    STD_LIST_CALL_DUMP(list, "get_value", "END_GET_VALUE_IND=", pos);

    DPRINTF("> End   StdListGetValue\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListDtor(StdList_t* list)
{
    DPRINTF("> Start StdListDtor()\n");

    if (list == NULL)
    {
        PRINTERR("STD_LIST_CTX_NULL");
        return STD_LIST_CTX_NULL;
    }

    free(list->data);

    DPRINTF("> End   StdListDtor\n\n");

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------
