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

    list_ctx->capacity = capacity;

    list_ctx->data->prev = 0;

    list_ctx->data->next = 0;

    list_ctx->free = 1;

    list_ctx->data[0].node = 0;

    for (size_t i = 0; i < capacity; i++)
    {
        // list_ctx->data-> = LIST_POISON;
    }

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

#ifdef LIST_DEBUG

//==========================================================================================

ListErr_t ListVerify(ListCtx_t* list_ctx)
{
    if (list_ctx == NULL)
    {
        return LIST_CTX_NULL;
    }
    if (list_ctx->data == NULL)
    {
        return LIST_DATA_NULL;
    }
    if (list_ctx->head < 0)
    {
        return LIST_HEAD_NEGATIVE;
    }
    if ((size_t) list_ctx->head >= list_ctx->capacity)
    {
        return LIST_HEAD_TOOBIG;
    }
    if (list_ctx->tail < 0)
    {
        return LIST_TAIL_NEGATIVE;
    }
    if ((size_t) list_ctx->tail >= list_ctx->capacity)
    {
        return LIST_TAIL_TOOBIG;
    }

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListDump(ListCtx_t* list_ctx, ListDumpInfo_t* dump_info)
{
    static int is_first_dump = 1;
    FILE* log_stream = NULL;

    if (is_first_dump)
    {
        log_stream    = fopen("list_log.htm", "w");
        is_first_dump = 0;
    }
    else
    {
        log_stream = fopen("list_log.htm", "a");
    }

    if (log_stream == NULL)
    {
        return LIST_LOGFILE_OPEN_ERROR;
    }

    fprintf(log_stream, "<pre>\n<h3><font color=blue>%s</font></h3>\n", dump_info->reason);

    fprintf(log_stream, "LIST DUMP called from %s at %s:%d\n",
            dump_info->func, dump_info->file, dump_info->line);

    fprintf(log_stream, "<font color=red>ERROR: %s (code %d)</font>\n\n",
                        LIST_STR_ERRORS[dump_info->error], dump_info->error);

    if (dump_info->error == LIST_CTX_NULL)
    {
        fprintf(log_stream, "list_ctx is a null pointer\n");
        return LIST_SUCCESS;
    }

    fprintf(log_stream,
            "list_ctx [%p]:\n\n"
            "capacity = %zu;\n"
            "head     = %d;\n"
            "tail     = %d;\n"
            "free     = %d;\n",
            list_ctx, list_ctx->capacity,
            list_ctx->head, list_ctx->tail, list_ctx->free);

    fprintf(log_stream, "\n<img src = %s width = 400px>\n\n", dump_info->image_name);

    fclose(log_stream);

    return LIST_SUCCESS;
}

//==========================================================================================

#endif /* LIST_DEBUG */

//------------------------------------------------------------------------------------------
