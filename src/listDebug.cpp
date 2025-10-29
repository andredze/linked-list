#include "listDebug.h"

#ifdef LIST_DEBUG

//==========================================================================================

ListErr_t ListVerify(ListCtx_t* list_ctx)
{
    if (list_ctx == NULL)
    {
        return LIST_CTX_NULL;
    }
    if (list_ctx->capacity > LIST_MAX_CAPACITY)
    {
        return LIST_CAPACITY_EXCEEDS_MAX;
    }
    if (list_ctx->data == NULL)
    {
        return LIST_DATA_NULL;
    }
    if (list_ctx->data[0].next < 0)
    {
        return LIST_HEAD_NEGATIVE;
    }
    if ((size_t) list_ctx->data[0].next >= list_ctx->capacity)
    {
        return LIST_HEAD_TOOBIG;
    }
    if (list_ctx->data[0].prev < 0)
    {
        return LIST_TAIL_NEGATIVE;
    }
    if ((size_t) list_ctx->data[0].prev >= list_ctx->capacity)
    {
        return LIST_TAIL_TOOBIG;
    }
    if (list_ctx->free < 0)
    {
        return LIST_FREE_NEGATIVE;
    }
    if ((size_t) list_ctx->free >= list_ctx->capacity)
    {
        return LIST_FREE_TOOBIG;
    }

    int* nodes = (int*) calloc(list_ctx->capacity, sizeof(list_ctx->data[0].next));
    int  count = 0;

    for (int i = list_ctx->data[0].next; i != 0; i = list_ctx->data[i].next)
    {
        if (list_ctx->data[i].node == LIST_POISON)
        {
            return LIST_FILLED_VALUE_IS_PZN;
        }
        if (LinearSearch(nodes, list_ctx->capacity, i) != -1)
        {
            return LIST_MAIN_IS_CYCLED;
        }
        if (list_ctx->data[i].next < 0)
        {
            return LIST_NEXT_NEGATIVE;
        }
        if ((size_t) list_ctx->data[i].next >= list_ctx->capacity)
        {
            return LIST_NEXT_TOOBIG;
        }
        if (list_ctx->data[i].prev < 0)
        {
            return LIST_PREV_NEGATIVE;
        }
        if ((size_t) list_ctx->data[i].prev >= list_ctx->capacity)
        {
            return LIST_PREV_TOOBIG;
        }

        nodes[count++] = i;
    }

    free(nodes);

    int* free_nodes = (int*) calloc(list_ctx->capacity, sizeof(list_ctx->data[0].next));
    int  free_count = 0;

    for (int i = list_ctx->free; i != 0; i = list_ctx->data[i].next)
    {
        if (list_ctx->data[i].node != LIST_POISON)
        {
            return LIST_FREE_VALUE_NOT_PZN;
        }
        if (LinearSearch(free_nodes, list_ctx->capacity, i) != -1)
        {
            return LIST_FREE_IS_CYCLED;
        }
        if (list_ctx->data[i].next < 0)
        {
            return LIST_FREE_NEXT_NEGATIVE;
        }
        if ((size_t) list_ctx->data[i].next >= list_ctx->capacity)
        {
            return LIST_FREE_NEXT_TOOBIG;
        }
        if (list_ctx->data[i].prev != -1)
        {
            return LIST_FREE_PREV_NOT_NULL;
        }

        free_nodes[free_count++] = i;
    }

    free(free_nodes);

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

int LinearSearch(int* array, size_t size, int elem)
{
    assert(array != NULL);

    for (int i = 0; i < (int) size; i++)
    {
        if (array[i] == elem)
        {
            return i;
        }
    }

    return -1;
}

//------------------------------------------------------------------------------------------

ListErr_t ListDump(ListCtx_t* list_ctx, ListDumpInfo_t* dump_info)
{

// TODO: test dump with all errors
// TODO: fix dump when capacity is bad
// TODO: fix dump when LIST_MAIN_IS_CYCLED

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
            "free     = %d;\n",
            list_ctx->data,
            list_ctx->capacity,
            list_ctx->free);

    if (dump_info->error == LIST_DATA_NULL)
    {
        fclose(log_stream);
        return LIST_SUCCESS;
    }

    fprintf(log_stream,
            "head     = %d;\n"
            "tail     = %d;\n",
            list_ctx->data[0].next,
            list_ctx->data[0].prev);

    if (dump_info->error == LIST_HEAD_NEGATIVE ||
        dump_info->error == LIST_HEAD_TOOBIG)
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

    fprintf(log_stream, "\n<img src = graphs/svg/%s.svg width = 100%%>\n\n", dump_info->image_name);

    fclose(log_stream);

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListCreateDumpGraph(ListCtx_t* list_ctx, const char* image_name)
{
    assert(list_ctx   != NULL);

    if (image_name == NULL)
    {
        PRINTERR("Given image name is null\n");
        return LIST_IMAGE_NAME_NULL;
    }
    if (strlen(image_name) > MAX_FILENAME_LEN / 2)
    {
        PRINTERR("Too big filename for graph image");
        return LIST_FILENAME_TOOBIG;
    }

// TODO: static переменная в дампе - номера картинок не пересекаются -
// папка с названием в дату и время - log.html и svg/ dot/

    char filename[MAX_FILENAME_LEN] = {};
    strcpy(filename, "graphs/dot/");
    strcat(filename, image_name);
    strcat(filename, ".dot");

    FILE* fp = fopen(filename, "w");
    if (fp == NULL)
    {
        PRINTERR("Opening graph logfile failed");
        return LIST_LOGFILE_OPEN_ERROR;
    }

    fprintf(fp,
            "digraph GG\n"
            "{\n"
	        "\tgraph [splines=ortho];\n"
	        "\tnode [fontname=\"Arial\", "
            "shape=\"Mrecord\", "
            "style=\"filled\", "
            "color = \"#3E3A22\", "
            "fillcolor=\"#E3DFC9\", "
            "fontcolor = \"#3E3A22\"];\n"
            "\t");

    for (size_t i = 0; i < list_ctx->capacity - 1; i++)
    {
        fprintf(fp, "node%zu->", i);
    }
    if (list_ctx->capacity >= 1)
    {
        fprintf(fp, "node%zu [style=\"invis\"];\n", list_ctx->capacity - 1);
    }

    /* add null node */
    fprintf(fp,
            "\tnode0["
            "color = \"#3E3A22\", "
            "fillcolor = \"#ecede8\", "
            "fontcolor = \"#3E3A22\", "
            "label=\"{ idx = 0 | value = ");
    if (list_ctx->data[0].node)
    {
        fprintf(fp, "PZN");
    }
    else
    {
        fprintf(fp, SPEC, list_ctx->data[0].node);
    }
    fprintf(fp, " | { prev = %d | next = %d }}\"];\n",
            list_ctx->data[0].prev,
            list_ctx->data[0].next);

    int skip_main_dump = 0;

    if (!(list_ctx->data[0].next >= 0))
    {
        skip_main_dump = 1;
    }

// TODO: придумать решение получше
    if (!(skip_main_dump))
    {
    /* make main list nodes */
        for (int i = list_ctx->data[0].next; i != 0; i = list_ctx->data[i].next)
        {
            fprintf(fp, "\tnode%d[label=\"{ idx = %d | value = ", i, i);
            if (list_ctx->data[i].node == LIST_POISON)
            {
                fprintf(fp, "PZN");
            }
            else
            {
                fprintf(fp, SPEC, list_ctx->data[i].node);
            }
            fprintf(fp, " | { prev = %d | next = %d }}\"];\n",
                    list_ctx->data[i].prev,
                    list_ctx->data[i].next);
        }
        fprintf(fp, "\t");

    /* make main list next edges */
        int i = list_ctx->data[0].next;
        if (list_ctx->data[i].next != 0)
        {
            for (; list_ctx->data[i].next != 0; i = list_ctx->data[i].next)
            {
                fprintf(fp, "node%d->", i);
            }
            fprintf(fp, "node%d [color = \"#640000\"];\n\t", i);
        }

    /* make main list prev edges */
        if (list_ctx->data[i].prev != 0)
        {
            for (; list_ctx->data[i].prev > 0; i = list_ctx->data[i].prev)
            {
                fprintf(fp, "node%d->", i);
            }
            fprintf(fp, "node%d [color = \"#000064\"];\n", i);
        }
    }

    /* make free list nodes */
    for (int j = list_ctx->free; j != 0; j = list_ctx->data[j].next)
    {
        fprintf(fp,
                "\tnode%d"
                "[fillcolor=\"#C0FFC0\", "
                "color=\"#006400\","
                "fontcolor = \"#005300\", "
                "label=\"{ idx = %d | value = ",
                j, j);
        if (list_ctx->data[j].node == LIST_POISON)
        {
            fprintf(fp, "PZN");
        }
        else
        {
            fprintf(fp, SPEC, list_ctx->data[j].node);
        }
        fprintf(fp, " | { prev = %d | next = %d }}\"];\n",
                list_ctx->data[j].prev,
                list_ctx->data[j].next);
    }
    fprintf(fp, "\t");

    /* make free list edges */
    int j = list_ctx->free;
    if (j != 0 && list_ctx->data[j].next != 0)
    {
        for (; list_ctx->data[j].next != 0; j = list_ctx->data[j].next)
        {
            fprintf(fp, "node%d->", j);
        }
        if (j != 0)
        {
            fprintf(fp, "node%d [color = \"#006400\", style=dashed];\n", j);
        }
    }

    /* make head, tail, free nodes */
    fprintf(fp, "\tnode [shape=\"box\", "
                "color=\"#70421A\", "
                "fontcolor=\"#70421A\", "
                "fillcolor=\"#DEB887\"];\n"
                "\ttail; head; free;\n"
                "\tedge["
                "color=\"#70421A\", "
                "arrowhead=none]");

    /* make head, tail and free edges to the elements */
    if (list_ctx->data[0].prev >= 0)
    {
        fprintf(fp, "\ttail->node%d;\n", list_ctx->data[0].prev);
    }
    if (list_ctx->data[0].next >= 0)
    {
        fprintf(fp, "\thead->node%d;\n", list_ctx->data[0].next);
    }
    fprintf(fp, "\tfree->node%d;\n"
                "\t{ rank=same; tail; head; free; }\n"
                "\t{ rank=same; ",
                list_ctx->free);

    /* place all nodes in one rank */
    for (size_t ind = 0; ind < list_ctx->capacity; ind++)
    {
        fprintf(fp, "node%zu; ", ind);
    }

    fprintf(fp, "}\n}\n");
    fclose(fp);

    return LIST_SUCCESS;
}

//==========================================================================================

#endif /* LIST_DEBUG */

//------------------------------------------------------------------------------------------
