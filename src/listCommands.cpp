#include "listCommands.h"

//------------------------------------------------------------------------------------------

static ListErr_t ListRealloc(ListCtx_t* list_ctx);

//------------------------------------------------------------------------------------------

ListErr_t ListCtor(ListCtx_t* list_ctx, size_t capacity)
{
    DPRINTF("> Start ListCtor(capacity = %zu)\n", capacity);

    if (list_ctx == NULL)
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

    list_ctx->data = (NodeCtx_t*) calloc(capacity, sizeof(NodeCtx_t));

    if (list_ctx->data == NULL)
    {
        PRINTERR("LIST_CALLOC_ERROR");
        return LIST_CALLOC_ERROR;
    }

    list_ctx->capacity = capacity;
    list_ctx->free     =  1;

    /* Filling the null element */
    list_ctx->data[0].prev = 0; /* tail */
    list_ctx->data[0].next = 0; /* head */
    list_ctx->data[0].node = LIST_POISON;

    /* Filling the free list */
    for (int i = 1; i < (int) capacity - 1; i++)
    {
        list_ctx->data[i].prev = -1;
        list_ctx->data[i].node = LIST_POISON; // TODO: не заполнять вообще? или только в дебажной версии?
        list_ctx->data[i].next = i + 1;
    }

    /* Last free element addresses to null */
    list_ctx->data[capacity - 1].prev = -1;
    list_ctx->data[capacity - 1].node = LIST_POISON;
    list_ctx->data[capacity - 1].next = 0;

    DPRINTF("> End   ListCtor\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListCheckPos(ListCtx_t* list_ctx, int pos)
{
    assert(list_ctx != NULL);

    if (pos < 0)
    {
        PRINTERR("List position is negative");
        return LIST_POSITION_NEGATIVE;
    }
    if ((size_t) pos >= list_ctx->capacity)
    {
        PRINTERR("List position is too big");
        return LIST_POSITION_TOO_BIG;
    }

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListInsertAfter(ListCtx_t* list_ctx, int pos, elem_t value)
{
    DPRINTF("> Start ListInsertAfter(pos = %d, value = " SPEC ")\n", pos, value);

    DEBUG_LIST_CHECK(list_ctx, "INSERT_AFTER_START");

    if (list_ctx->data[0].next == 0 || list_ctx->data[0].prev == 0)
    {
        PRINTERR("Given list to insert is empty");
        return LIST_IS_EMPTY;
    }

    SAFE_CALL(ListCheckPos(list_ctx, pos));

    /* Check that pos is in list */
    if (list_ctx->data[pos].prev == -1)
    {
        PRINTERR("List doesn't have an element with pos = %d", pos);
        return LIST_NO_SUCH_ELEMENT;
    }

    if (list_ctx->free == 0)
    {
        SAFE_CALL(ListRealloc(list_ctx));
    }

    int cur_index  = list_ctx->free;
    int pos_next   = list_ctx->data[pos].next;

    /* set new free element */
    list_ctx->free = list_ctx->data[list_ctx->free].next;

    /* connect previous element to current */
    list_ctx->data[pos].next       = cur_index;

    /* connect current element to previous and next */
    list_ctx->data[cur_index].prev = pos;
    list_ctx->data[cur_index].node = value;
    list_ctx->data[cur_index].next = pos_next;

    /* connect next element to current */
    list_ctx->data[pos_next].prev  = cur_index;

    DEBUG_LIST_CHECK(list_ctx, "INSERT_AFTER_END");

    DPRINTF("> End   ListInsertAfter\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListInsertBefore(ListCtx_t* list_ctx, int pos, elem_t value)
{
    DPRINTF("> Start ListInsertBefore(pos = %d, value = " SPEC ")\n", pos, value);

    DEBUG_LIST_CHECK(list_ctx, "INSERT_BEFORE_START");

    if (list_ctx->data[0].next == 0 || list_ctx->data[0].prev == 0)
    {
        PRINTERR("Given list to insert is empty");
        return LIST_IS_EMPTY;
    }

    SAFE_CALL(ListCheckPos(list_ctx, pos));

    /* Check that pos is in list */
    if (list_ctx->data[pos].prev == -1)
    {
        PRINTERR("List doesn't have an element with pos = %d", pos);
        return LIST_NO_SUCH_ELEMENT;
    }

    if (list_ctx->free == 0)
    {
        SAFE_CALL(ListRealloc(list_ctx));
    }

    int cur_index  = list_ctx->free;
    int pos_prev   = list_ctx->data[pos].prev;

    /* set new free element */
    list_ctx->free = list_ctx->data[list_ctx->free].next;

    /* connect previous element to current */
    list_ctx->data[pos_prev].next  = cur_index;

    /* connect current element to previous and next */
    list_ctx->data[cur_index].prev = pos_prev;
    list_ctx->data[cur_index].node = value;
    list_ctx->data[cur_index].next = pos;

    /* connect next element to current */
    list_ctx->data[pos].prev       = cur_index;

    DEBUG_LIST_CHECK(list_ctx, "INSERT_BEFORE_END");

    DPRINTF("> End   ListInsertBefore\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListPushFront(ListCtx_t* list_ctx, elem_t value)
{
    DPRINTF("> Start ListPushFront(value = " SPEC ")\n", value);

    DEBUG_LIST_CHECK(list_ctx, "PUSH_FRONT_START");

    if (list_ctx->free == 0)
    {
        SAFE_CALL(ListRealloc(list_ctx));
    }

    int cur_index  = list_ctx->free;
    int head       = list_ctx->data[0].next;

    /* set new free element */
    list_ctx->free = list_ctx->data[list_ctx->free].next;

    /* set new head */
    list_ctx->data[0].next = cur_index;

    /* connect new element to null element and prev head */
    list_ctx->data[cur_index].prev = 0;
    list_ctx->data[cur_index].node = value;
    list_ctx->data[cur_index].next = head;

    /* connect prev head to new element */
    list_ctx->data[head].prev = cur_index;

    DEBUG_LIST_CHECK(list_ctx, "PUSH_FRONT_END");

    DPRINTF("> End   ListPushFront\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListPushBack(ListCtx_t* list_ctx, elem_t value)
{
    DPRINTF("> Start ListPushBack(value = " SPEC ")\n", value);

    DEBUG_LIST_CHECK(list_ctx, "PUSH_BACK_START");

    if (list_ctx->free == 0)
    {
        SAFE_CALL(ListRealloc(list_ctx));
    }

    int cur_index  = list_ctx->free;
    int tail       = list_ctx->data[0].prev;

    /* set new free element */
    list_ctx->free = list_ctx->data[list_ctx->free].next;

    /* set new tail */
    list_ctx->data[0].prev = cur_index;

    /* connect new element to null element and prev tail */
    list_ctx->data[cur_index].prev = tail;
    list_ctx->data[cur_index].node = value;
    list_ctx->data[cur_index].next = 0;

    /* connect prev tail to new element */
    list_ctx->data[tail].next = cur_index;

    DEBUG_LIST_CHECK(list_ctx, "PUSH_BACK_END");

    DPRINTF("> End   ListPushBack\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static ListErr_t ListRealloc(ListCtx_t* list_ctx)
{
    DPRINTF("\t> Start ListRealloc()\n");

    DEBUG_LIST_CHECK(list_ctx, "REALLOC_START");

    size_t new_size = sizeof(list_ctx->data[0]) * (list_ctx->capacity * 2 + 1);

    NodeCtx_t* new_data = (NodeCtx_t*) realloc(list_ctx->data, new_size);

    if (new_data == NULL)
    {
        PRINTERR("List data realloc failed");
        return LIST_DATA_REALLOC_ERROR;
    }

    size_t old_capacity = list_ctx->capacity;
    list_ctx->capacity  = list_ctx->capacity * 2 + 1;
    list_ctx->data      = new_data;

    list_ctx->free = (int) old_capacity;

    /* Filling the free list */
    for (int i = (int) old_capacity; i < (int) list_ctx->capacity - 1; i++)
    {
        list_ctx->data[i].prev = -1;
        list_ctx->data[i].node = LIST_POISON;
        list_ctx->data[i].next = i + 1;
    }

    /* Last free element addresses to null */
    list_ctx->data[list_ctx->capacity - 1].prev = -1;
    list_ctx->data[list_ctx->capacity - 1].node = LIST_POISON;
    list_ctx->data[list_ctx->capacity - 1].next = 0;

    DEBUG_LIST_CHECK(list_ctx, "REALLOC_END");

    DPRINTF("\t> End   ListRealloc\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListErase(ListCtx_t* list_ctx, int pos)
{
    DPRINTF("> Start ListErase(pos = %d)\n", pos);

    DEBUG_LIST_CHECK(list_ctx, "ERASE_START");

    SAFE_CALL(ListCheckPos(list_ctx, pos));

    int next_ind = list_ctx->data[pos].next;
    int prev_ind = list_ctx->data[pos].prev;

    /* Connect pos to free and set poisons */
    list_ctx->data[pos].prev = -1;
    list_ctx->data[pos].node = LIST_POISON;
    list_ctx->data[pos].next = list_ctx->free;

    list_ctx->free = pos;

    DPRINTF("\thead = %d;\n"
            "\ttail = %d;\n"
            "\tpos = %d;\n"
            "\tprev_ind = %d;\n"
            "\tnext_ind = %d;\n",
            list_ctx->data[0].next,
            list_ctx->data[0].prev,
            pos,
            prev_ind,
            next_ind);

    list_ctx->data[prev_ind].next = next_ind;
    list_ctx->data[next_ind].prev = prev_ind;

    DEBUG_LIST_CHECK(list_ctx, "ERASE_END");

    DPRINTF("> End   ListErase\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListDtor(ListCtx_t* list_ctx)
{
    DPRINTF("> Start ListDtor()\n");

    if (list_ctx == NULL)
    {
        PRINTERR("LIST_CTX_NULL");
        return LIST_CTX_NULL;
    }

    free(list_ctx->data);

    DPRINTF("> End   ListDtor\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------
