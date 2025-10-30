#include "listDebug.h"

#ifdef LIST_DEBUG

//==========================================================================================

ListErr_t ListVerify(List_t* list)
{
    if (list == NULL)
    {
        return LIST_CTX_NULL;
    }
    if (list->capacity > LIST_MAX_CAPACITY)
    {
        return LIST_CAPACITY_EXCEEDS_MAX;
    }
    if (list->size >= list->capacity)
    {
        return LIST_SIZE_EXCEEDS_CAPACITY;
    }
    if (list->data == NULL)
    {
        return LIST_DATA_NULL;
    }
    if (list->data[0].next < 0)
    {
        return LIST_HEAD_NEGATIVE;
    }
    if ((size_t) list->data[0].next >= list->capacity)
    {
        return LIST_HEAD_TOOBIG;
    }
    if (list->data[0].prev < 0)
    {
        return LIST_TAIL_NEGATIVE;
    }
    if ((size_t) list->data[0].prev >= list->capacity)
    {
        return LIST_TAIL_TOOBIG;
    }
    if (list->free < 0)
    {
        return LIST_FREE_NEGATIVE;
    }
    if ((size_t) list->free >= list->capacity)
    {
        return LIST_FREE_TOOBIG;
    }

    int*   next_nodes = (int*) calloc(list->capacity, sizeof(list->data[0].next));
    size_t next_count = 0;

    for (int i = list->data[0].next; i != 0; i = list->data[i].next)
    {
        if (list->data[i].value == LIST_POISON)
        {
            free(next_nodes);
            return LIST_FILLED_VALUE_IS_PZN;
        }
        if (LinearSearch(next_nodes, list->capacity, i) != -1)
        {
            free(next_nodes);
            return LIST_NEXT_IS_CYCLED;
        }
        if (list->data[i].next < 0)
        {
            free(next_nodes);
            return LIST_NEXT_NEGATIVE;
        }
        if ((size_t) list->data[i].next >= list->capacity)
        {
            free(next_nodes);
            return LIST_NEXT_TOOBIG;
        }

        next_nodes[next_count++] = i;
    }

    free(next_nodes);

    if (next_count != list->size)
    {
        return LIST_SIZE_IS_WRONG;
    }

    int*   prev_nodes = (int*) calloc(list->capacity, sizeof(list->data[0].prev));
    size_t prev_count = 0;

    for (int i = list->data[0].prev; i != 0; i = list->data[i].prev)
    {
        if (LinearSearch(prev_nodes, list->capacity, i) != -1)
        {
            free(prev_nodes);
            return LIST_PREV_IS_CYCLED;
        }
        if (list->data[i].prev < 0)
        {
            free(prev_nodes);
            return LIST_PREV_NEGATIVE;
        }
        if ((size_t) list->data[i].prev >= list->capacity)
        {
            free(prev_nodes);
            return LIST_PREV_TOOBIG;
        }

        prev_nodes[prev_count++] = i;
    }

    free(prev_nodes);

    int*   free_nodes = (int*) calloc(list->capacity, sizeof(list->data[0].next));
    size_t free_count = 0;

    for (int i = list->free; i != 0; i = list->data[i].next)
    {
        if (list->data[i].value != LIST_POISON)
        {
            free(free_nodes);
            return LIST_FREE_VALUE_NOT_PZN;
        }
        if (LinearSearch(free_nodes, list->capacity, i) != -1)
        {
            free(free_nodes);
            return LIST_FREE_IS_CYCLED;
        }
        if (list->data[i].next < 0)
        {
            free(free_nodes);
            return LIST_FREE_NEXT_NEGATIVE;
        }
        if ((size_t) list->data[i].next >= list->capacity)
        {
            free(free_nodes);
            return LIST_FREE_NEXT_TOOBIG;
        }
        if (list->data[i].prev != -1)
        {
            free(free_nodes);
            return LIST_FREE_PREV_NOT_NULL;
        }

        free_nodes[free_count++] = i;
    }

    free(free_nodes);

    if (next_count + free_count + 1 != list->capacity)
    {
        return LIST_CAP_IS_WRONG;
    }

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

ListErr_t ListDump(List_t* list, ListDumpInfo_t* dump_info)
{
    static int calls_count = 1;

    char image_name[MAX_FILENAME_LEN] = {};
    sprintf(image_name, "%04d_%s", calls_count, dump_info->image_name);

    FILE* log_stream = NULL;

    log_stream = fopen("list_log.htm", calls_count == 1 ? "w" : "a");

    calls_count += 1;

    if (log_stream == NULL)
    {
        PRINTERR("Opening logfile failed");
        return LIST_LOGFILE_OPEN_ERROR;
    }

    fprintf(log_stream, "<pre>\n<h3><font color=blue>%s%d</font></h3>",
            dump_info->reason, dump_info->command_arg);

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

    fprintf(log_stream, "LIST DUMP called from %s at %s:%d\n\nlist [%p]:\n\n",
            dump_info->func, dump_info->file, dump_info->line, list);

    if (dump_info->error == LIST_CTX_NULL)
    {
        fclose(log_stream);
        return LIST_SUCCESS;
    }

    fprintf(log_stream,
            "data     = %p;\n"
            "capacity = %zu;\n"
            "free     = %d;\n",
            list->data,
            list->capacity,
            list->free);

    if (dump_info->error == LIST_DATA_NULL)
    {
        fclose(log_stream);
        return LIST_SUCCESS;
    }

    fprintf(log_stream,
            "head     = %d;\n"
            "tail     = %d;\n",
            list->data[0].next,
            list->data[0].prev);

    if (dump_info->error == LIST_HEAD_NEGATIVE        ||
        dump_info->error == LIST_HEAD_TOOBIG          ||
        dump_info->error == LIST_CAPACITY_EXCEEDS_MAX)
    {
        fclose(log_stream);
        return LIST_SUCCESS;
    }

    ListErr_t graph_error = LIST_SUCCESS;
    if ((graph_error = ListCreateDumpGraph(list, image_name)))
    {
        fclose(log_stream);
        return graph_error;
    }

    fprintf(log_stream, "\n<img src = graphs/svg/%s.svg width = 100%%>\n\n", image_name);

    fclose(log_stream);

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListCreateDumpGraph(List_t* list, const char* image_name)
{
    assert(list != NULL);

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

// TODO: папка с названием в дату и время - log.html и svg/ dot/

    char filename[MAX_FILENAME_LEN] = {};
    sprintf(filename, "graphs/dot/%s.dot", image_name);

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
            "\tedge [constraint=false]; ");

    /* Create invisible edges for all nodes */
    for (size_t i = 0; i < list->capacity - 1; i++)
    {
        fprintf(fp, "node%zu->", i);
    }
    if (list->capacity >= 1)
    {
        fprintf(fp, "node%zu [style=\"invis\"];\n", list->capacity - 1);
    }

    /* add null node */
    fprintf(fp, "\tnode0[color = \"#3E3A22\", fillcolor = \"#ecede8\", fontcolor = \"#3E3A22\", "
            "label=\"{ idx = 0 | value = ");
    if (list->data[0].value == LIST_POISON)
    {
        fprintf(fp, "PZN");
    }
    else
    {
        fprintf(fp, SPEC, list->data[0].value);
    }
    fprintf(fp, " | { prev = %d | next = %d }}\"];\n", list->data[0].prev, list->data[0].next);

    /* make list nodes */
    for (int i = 1; (size_t) i < list->capacity; i++)
    {
        /* if free */
        if (list->data[i].prev == -1)
        {
            fprintf(fp, "\tnode%d [fillcolor=\"#C0FFC0\", color=\"#006400\", fontcolor = \"#005300\", label=\"{ idx = %d | value = ", i, i);
            if (list->data[i].value == LIST_POISON)
            {
                fprintf(fp, "PZN");
            }
            else
            {
                fprintf(fp, SPEC, list->data[i].value);
            }
            fprintf(fp, " | { prev = %d | next = %d }}\"];\n", list->data[i].prev, list->data[i].next);

            continue;
        }
        /* else */
        fprintf(fp, "\tnode%d[label=\"{ idx = %d | value = ", i, i);
        if (list->data[i].value == LIST_POISON)
        {
            fprintf(fp, "PZN");
        }
        else
        {
            fprintf(fp, SPEC, list->data[i].value);
        }
        fprintf(fp, " | { prev = %d | next = %d }}\"];\n",
                list->data[i].prev,
                list->data[i].next);
    }

    /* make all edges */
    for (size_t pos = 1; pos < list->capacity; pos++)
    {
        int next = list->data[pos].next;
        int prev = list->data[pos].prev;

        if (prev == -1 && next == 0)
        {
            continue;
        }

        if (prev == -1)
        {
            if ((size_t) next < list->capacity && next >= 0)
            {
                fprintf(fp, "\tnode%zu->node%d [color = \"#006400\", style=dashed];\n", pos, next);
            }
            else
            {
                fprintf(fp, "\twrong_next%zu[shape = \"octagon\", color = \"#640000\", fillcolor = \"#FFC0C0\", fontcolor = \"#640000\", label=\"idx = %d\"];\n", pos, next);
                fprintf(fp, "\tnode%zu->wrong_next%zu [color = \"#640000\", constraint=true];\n", pos, pos);
            }

            continue;
        }

        if (next != 0)
        {
            if ((size_t) next < list->capacity && next >= 0)
            {
                fprintf(fp, "\tnode%zu->node%d [color = \"#000064\"];\n", pos, next);
            }
            else
            {
                fprintf(fp, "\twrong_next%zu[shape = \"octagon\", color = \"#640000\", fillcolor = \"#FFC0C0\", fontcolor = \"#640000\", label=\"idx = %d\"];\n", pos, next);
                fprintf(fp, "\tnode%zu->wrong_next%zu [color = \"#640000\", constraint=true];\n", pos, pos);
            }
        }

        if (prev != 0)
        {
            if ((size_t) prev < list->capacity && prev >= 0)
            {
                fprintf(fp, "\tnode%zu->node%d [color = \"#F4A460\"];\n", pos, prev);
            }
            else
            {
                fprintf(fp, "\twrong_prev%zu[shape = \"octagon\", color = \"#640000\", fillcolor = \"#FFC0C0\", fontcolor = \"#640000\", label=\"idx = %d\"];\n", pos, prev);
                fprintf(fp, "\tnode%zu->wrong_prev%zu [color = \"#640000\", constraint=true];\n", pos, pos);
            }
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
                "arrowhead=none]\n"
                "edge [constraint=true];\n");

    /* make head, tail and free edges to the elements */
    if (list->data[0].prev >= 0)
    {
        fprintf(fp, "\ttail->node%d;\n", list->data[0].prev);
    }
    if (list->data[0].next >= 0)
    {
        fprintf(fp, "\thead->node%d;\n", list->data[0].next);
    }

    fprintf(fp, "\tfree->node%d;\n"
                "\t{ rank=same; tail; head; free; }\n"
                "\t{ rank=same; ",
                list->free);

    /* place all nodes in one rank */
    for (size_t ind = 0; ind < list->capacity; ind++)
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
