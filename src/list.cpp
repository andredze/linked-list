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

    list_ctx->data = (NodeCtx_t*) calloc(capacity, sizeof(NodeCtx_t));

    if (list_ctx->data == NULL)
    {
        PRINTERR("LIST_CALLOC_ERROR");
        return LIST_CALLOC_ERROR;
    }

    list_ctx->capacity = capacity;
    list_ctx->free     =  1;
    list_ctx->head     = -1;
    list_ctx->tail     = -1;

    /* Filling first element with nulls */
    list_ctx->data[0].prev = 0;
    list_ctx->data[0].next = 0; // TODO: LIST_NULL?
    list_ctx->data[0].node = LIST_POISON;

    /* Filling the free list */
    for (int i = 1; i < (int) capacity - 1; i++)
    {
        list_ctx->data[i].prev = -1; // TODO: LIST_PREV_POISON?
        list_ctx->data[i].node = LIST_POISON;
        list_ctx->data[i].next = i + 1;
    }

    /* Last free element addresses to null */
    list_ctx->data[capacity - 1].prev = -1;
    list_ctx->data[capacity - 1].node = LIST_POISON;
    list_ctx->data[capacity - 1].next = 0;

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListInsertAfter(ListCtx_t* list_ctx, int pos, elem_t value)
{
    DEBUG_LIST_CHECK(list_ctx, "INSERT_AFTER_START");

    if (pos < 0)
    {
        PRINTERR("List position to insert is negative");
        return LIST_POSITION_NEGATIVE;
    }
    if ((size_t) pos >= list_ctx->capacity)
    {
        PRINTERR("List position to insert is too big");
        return LIST_POSITION_TOO_BIG;
    }

    /* Check that pos is in list */
    if (list_ctx->data[pos].node == LIST_POISON)
    {
        PRINTERR("List doesn't have an element with pos = %d", pos);
        return LIST_NO_SUCH_ELEMENT;
    }

    if (list_ctx->free == 0)
    {
        SAFE_CALL(ListRealloc(list_ctx));
    }

    int cur_index = list_ctx->free;

    if (list_ctx->head == -1)
    {
        list_ctx->head = cur_index;
    }
    if (list_ctx->tail == -1)
    {
        list_ctx->tail = cur_index;
    }
    if (pos == list_ctx->tail)
    {
        list_ctx->tail = cur_index;
    }

    list_ctx->free = list_ctx->data[list_ctx->free].next;

    list_ctx->data[cur_index].node = value;
    list_ctx->data[cur_index].prev = pos;
    int pos_next = list_ctx->data[pos].next;
    list_ctx->data[cur_index].next = pos_next;

    list_ctx->data[pos_next].prev  = cur_index;
    list_ctx->data[pos].next       = cur_index;

    DEBUG_LIST_CHECK(list_ctx, "INSERT_AFTER_END");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListInsertBefore(ListCtx_t* list_ctx, int pos, elem_t value)
{
    DEBUG_LIST_CHECK(list_ctx, "INSERT_BEFORE_START");

    if (pos < 0)
    {
        PRINTERR("List position to insert is negative");
        return LIST_POSITION_NEGATIVE;
    }
    if ((size_t) pos >= list_ctx->capacity)
    {
        PRINTERR("List position to insert is too big");
        return LIST_POSITION_TOO_BIG;
    }

    /* Check that pos is in list */
    if (list_ctx->data[pos].node == LIST_POISON)
    {
        PRINTERR("List doesn't have an element with pos = %d", pos);
        return LIST_NO_SUCH_ELEMENT;
    }

    if (list_ctx->free == 0)
    {
        SAFE_CALL(ListRealloc(list_ctx));
    }

    int cur_index = list_ctx->free;

    if (list_ctx->head == -1)
    {
        list_ctx->head = cur_index;
    }
    if (list_ctx->tail == -1)
    {
        list_ctx->tail = cur_index;
    }
    if (pos == list_ctx->head)
    {
        list_ctx->head = cur_index;
    }

    list_ctx->free = list_ctx->data[list_ctx->free].next;

    list_ctx->data[cur_index].node = value;
    list_ctx->data[cur_index].next = pos;
    int pos_prev = list_ctx->data[pos].prev;
    list_ctx->data[cur_index].prev = pos_prev;

    list_ctx->data[pos_prev].next  = cur_index;
    list_ctx->data[pos].prev       = cur_index;

    DEBUG_LIST_CHECK(list_ctx, "INSERT_BEFORE_END");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListPushFront(ListCtx_t* list_ctx, elem_t value)
{
    DEBUG_LIST_CHECK(list_ctx, "PUSH_FRONT_START");

    if (list_ctx->free == 0)
    {
        SAFE_CALL(ListRealloc(list_ctx));
    }

    int cur_index  = list_ctx->free;
    list_ctx->free = list_ctx->data[list_ctx->free].next;

    if (list_ctx->head == -1)
    {
        list_ctx->head = cur_index;
        list_ctx->tail = cur_index;
        list_ctx->data[cur_index].prev = 0;
        list_ctx->data[cur_index].node = value;
        list_ctx->data[cur_index].next = 0;

        DEBUG_LIST_CHECK(list_ctx, "PUSH_FRONT_END");

        return LIST_SUCCESS;
    }

    list_ctx->data[list_ctx->head].prev = cur_index;

    list_ctx->data[cur_index].prev = 0;
    list_ctx->data[cur_index].node = value;
    list_ctx->data[cur_index].next = list_ctx->head;

    list_ctx->head = cur_index;

    DEBUG_LIST_CHECK(list_ctx, "PUSH_FRONT_END");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListPushBack(ListCtx_t* list_ctx, elem_t value)
{
    DEBUG_LIST_CHECK(list_ctx, "PUSH_BACK_START");

    if (list_ctx->free == 0)
    {
        SAFE_CALL(ListRealloc(list_ctx));
    }

    int cur_index  = list_ctx->free;
    list_ctx->free = list_ctx->data[list_ctx->free].next;

    if (list_ctx->tail == -1)
    {
        list_ctx->head = cur_index;
        list_ctx->tail = cur_index;
        list_ctx->data[cur_index].prev = 0;
        list_ctx->data[cur_index].node = value;
        list_ctx->data[cur_index].next = 0;

        DEBUG_LIST_CHECK(list_ctx, "PUSH_BACK_END");

        return LIST_SUCCESS;
    }

    list_ctx->data[list_ctx->tail].next = cur_index;

    list_ctx->data[cur_index].prev = list_ctx->tail;
    list_ctx->data[cur_index].node = value;
    list_ctx->data[cur_index].next = 0;

    list_ctx->tail = cur_index;

    DEBUG_LIST_CHECK(list_ctx, "PUSH_BACK_END");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListRealloc(ListCtx_t* list_ctx)
{
    DEBUG_LIST_CHECK(list_ctx, "REALLOC_START");

    size_t new_size = sizeof(list_ctx->data[0]) * (list_ctx->capacity * 2 + 1);

    NodeCtx_t* new_data = (NodeCtx_t*) realloc(list_ctx->data, new_size);

    if (new_data == NULL)
    {
        PRINTERR("List data realloc failed");
        return LIST_DATA_REALLOC_ERROR;
    }

    list_ctx->data      = new_data;
    size_t old_capacity = list_ctx->capacity;
    list_ctx->capacity  = list_ctx->capacity * 2 + 1;

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

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListErase(ListCtx_t* list_ctx, int pos)
{
    DEBUG_LIST_CHECK(list_ctx, "ERASE_START");

    int next_ind = list_ctx->data[pos].next;
    int prev_ind = list_ctx->data[pos].prev;

    list_ctx->data[pos].prev = -1;
    list_ctx->data[pos].node = LIST_POISON;
    list_ctx->data[pos].next = list_ctx->free;

    list_ctx->free = pos;

    if (list_ctx->head == pos && list_ctx->tail == pos)
    {
        DEBUG_LIST_CHECK(list_ctx, "ERASE_END_CASE_ONE_ELEM");
        return LIST_SUCCESS;
    }
    else if (list_ctx->head == pos)
    {
        list_ctx->head = next_ind;
        list_ctx->data[next_ind].prev = 0;

        DEBUG_LIST_CHECK(list_ctx, "ERASE_END_CASE_FIRST_ELEM");
        return LIST_SUCCESS;
    }
    else if (list_ctx->tail == pos)
    {
        list_ctx->tail = prev_ind;
        list_ctx->data[prev_ind].next = 0;

        DEBUG_LIST_CHECK(list_ctx, "ERASE_END_CASE_LAST_ELEM");
        return LIST_SUCCESS;
    }

    DPRINTF(">Erasing data[%d]:\n", pos);
    DPRINTF("\tprev_ind = %d\n", prev_ind);
    DPRINTF("\tnext_ind = %d\n\n", next_ind);

    list_ctx->data[prev_ind].next = next_ind;
    list_ctx->data[next_ind].prev = prev_ind;

    DEBUG_LIST_CHECK(list_ctx, "ERASE_END");

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
    if (list_ctx->head < -1)
    {
        return LIST_HEAD_NEGATIVE;
    }
    if (list_ctx->head != -1 && (size_t) list_ctx->head >= list_ctx->capacity)
    {
        return LIST_HEAD_TOOBIG;
    }
    if (list_ctx->tail < -1)
    {
        return LIST_TAIL_NEGATIVE;
    }
    if (list_ctx->tail != -1 && (size_t) list_ctx->tail >= list_ctx->capacity)
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
        PRINTERR("Opening logfile failed");
        return LIST_LOGFILE_OPEN_ERROR;
    }

    fprintf(log_stream, "<pre>\n<h3><font color=blue>%s</font></h3>", dump_info->reason);

    if (dump_info->error == LIST_SUCCESS)
    {
        fprintf(log_stream, "<font color=green><b>%s (code %d)</b></font>\n",
                            LIST_STR_ERRORS[dump_info->error], dump_info->error);
    }
    else
    {
        fprintf(log_stream, "<font color=red><b>ERROR: %s (code %d)</b></font>\n",
                            LIST_STR_ERRORS[dump_info->error], dump_info->error);
    }

    fprintf(log_stream, "LIST DUMP called from %s at %s:%d\n\n",
            dump_info->func, dump_info->file, dump_info->line);

    fprintf(log_stream, "list_ctx [%p]:\n\n", list_ctx);

    if (dump_info->error == LIST_CTX_NULL)
    {
        fclose(log_stream);
        return LIST_SUCCESS;
    }

    fprintf(log_stream,
            "data     = %p;\n"
            "capacity = %zu;\n"
            "head     = %d;\n"
            "tail     = %d;\n"
            "free     = %d;\n",
            list_ctx->data, list_ctx->capacity, list_ctx->head,
            list_ctx->tail, list_ctx->free);

    if (!(list_ctx->head >= 0 && (size_t) list_ctx->head < list_ctx->capacity))
    {
        fclose(log_stream);
        return LIST_SUCCESS;
    }
    if (dump_info->error == LIST_DATA_NULL)
    {
        fclose(log_stream);
        return LIST_SUCCESS;
    }

    ListErr_t graph_error = LIST_SUCCESS;
    if ((graph_error = ListCreateDumpGraph(list_ctx, dump_info->image_name)))
    {
        fclose(log_stream);
        return graph_error;
    }

    fprintf(log_stream, "\n<img src = graphs/png/%s.png width = 600px>\n\n", dump_info->image_name);

    fclose(log_stream);

    return LIST_SUCCESS;
}

ListErr_t ListCreateDumpGraph(ListCtx_t* list_ctx, const char* image_name)
{
    assert(list_ctx   != NULL);
    assert(image_name != NULL);

    if (strlen(image_name) > MAX_FILENAME_LEN / 2)
    {
        PRINTERR("Too big filename for graph image");
        return LIST_GRAPH_ERROR;
    }

    char filename[MAX_FILENAME_LEN] = {};
    strcpy(filename, "graphs/dot/");
    strcat(filename, image_name);
    strcat(filename, ".dot");

    FILE* stream = fopen(filename, "w");
    if (stream == NULL)
    {
        PRINTERR("Opening graph logfile failed");
        return LIST_LOGFILE_OPEN_ERROR;
    }

    fprintf(stream,
            "digraph GG\n"
            "{\n"
	        "\tgraph [splines=ortho];\n"
	        "\tnode [fontname=\"Arial\", "
            "shape=\"Mrecord\", "
            "style=\"filled\", "
            "color = \"#000064\", "
            "fillcolor=\"#C0C0FF\", "
            "fontcolor = \"#000053\"];\n");


    fprintf(stream, "\t");

    for (size_t i = 1; i < list_ctx->capacity - 1; i++)
    {
        fprintf(stream, "node%zu->", i);
    }
    if (list_ctx->capacity >= 1)
    {
        fprintf(stream, "node%zu [style=\"invis\"];\n", list_ctx->capacity - 1);
    }

    for (int i = list_ctx->head; i != 0; i = list_ctx->data[i].next)
    {
        fprintf(stream,
                "\tnode%d[label=\"{ idx = %d | value = " SPEC " | { prev = %d | next = %d }}\"];\n",
                i, i, list_ctx->data[i].node, list_ctx->data[i].prev, list_ctx->data[i].next);
    }
    fprintf(stream, "\t");

    int i = list_ctx->head;
    if (list_ctx->data[i].next != 0)
    {
        for (; list_ctx->data[i].next != 0; i = list_ctx->data[i].next)
        {
            fprintf(stream, "node%d->", i);
        }
        fprintf(stream, "node%d [color = \"#000064\"];\n\t", i);
    }
    if (list_ctx->data[i].prev != 0)
    {
        for (; list_ctx->data[i].prev != 0; i = list_ctx->data[i].prev)
        {
            fprintf(stream, "node%d->", i);
        }
        fprintf(stream, "node%d [color = \"#640000\"];\n", i);
    }

    /* free list */
    for (int j = list_ctx->free; j != 0; j = list_ctx->data[j].next)
    {
        fprintf(stream,
                "\tnode%d[fillcolor=\"#C0FFC0\", "
                "color=\"#006400\","
                "fontcolor = \"#005300\", "
                "label=\"{ idx = %d | value = " SPEC " | { prev = %d | next = %d }}\"];\n",
                j, j, list_ctx->data[j].node, list_ctx->data[j].prev, list_ctx->data[j].next);
    }
    fprintf(stream, "\t");

    int j = list_ctx->free;
    for (; list_ctx->data[j].next != 0; j = list_ctx->data[j].next)
    {
        fprintf(stream, "node%d->", j);
    }
    if (j != 0)
    {
        fprintf(stream, "node%d [color = \"#006400\"];\n", j);
    }

    fprintf(stream, "\tnode [shape=\"box\", color=\"#70421A\", fontcolor=\"#70421A\", fillcolor=\"#DEB887\"];\n"
                    "\ttail; head; free;\n"
                    "\tedge[color=\"#70421A\"]"
                    "\ttail->node%d;\n"
                    "\thead->node%d;\n"
                    "\tfree->node%d;\n"
                    "\t{ rank=same; tail; head; free; }\n"
                    "\t{ rank=same; ",
                    list_ctx->tail,
                    list_ctx->head,
                    list_ctx->free);
    for (size_t ind = 1; ind < list_ctx->capacity; ind++)
    {
        fprintf(stream, "node%zu; ", ind);
    }

    fprintf(stream, "}\n}\n");
    fclose(stream);

    return LIST_SUCCESS;
}

//==========================================================================================

#endif /* LIST_DEBUG */

//------------------------------------------------------------------------------------------
