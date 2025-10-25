#include "list.h"

//------------------------------------------------------------------------------------------

ListErr_t ListCtor(ListCtx_t* list_ctx, size_t capacity)
{
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

    list_ctx->data = (NodeCtx_t*) calloc(capacity, sizeof(elem_t));

    if (list_ctx->data == NULL)
    {
        PRINTERR("LIST_CALLOC_ERROR");
        return LIST_CALLOC_ERROR;
    }

    list_ctx->data->prev = 0;

    list_ctx->data->next = 0;

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListDtor(ListCtx_t* list_ctx)
{
    if (list_ctx == NULL)
    {
        return LIST_CTX_NULL;
    }

    free(list_ctx->data);

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------
