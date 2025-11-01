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
    if (list->free < -1)
    {
        return LIST_FREE_NEGATIVE;
    }
    if (list->free != -1 && (size_t) list->free >= list->capacity)
    {
        return LIST_FREE_TOOBIG;
    }
    if (list->is_sorted != 0 && list->is_sorted != 1)
    {
        return LIST_FLAG_IS_WRONG;
    }
    if (list->do_linear_realloc != 0 && list->do_linear_realloc != 1)
    {
        return LIST_FLAG_IS_WRONG;
    }

    size_t next_count = 0;
    size_t prev_count = 0;
    size_t free_count = 0;

    ListErr_t error = LIST_SUCCESS;

    if ((error = ListVerifyNext(list, &next_count)) != LIST_SUCCESS)
    {
        return error;
    }
    if ((error = ListVerifyPrev(list, &prev_count)) != LIST_SUCCESS)
    {
        return error;
    }
    if ((error = ListVerifyFree(list, &free_count)) != LIST_SUCCESS)
    {
        return error;
    }

    if (next_count != list->size || prev_count != list->size)
    {
        return LIST_SIZE_IS_WRONG;
    }
    if (next_count + free_count + 1 != list->capacity)
    {
        return LIST_CAP_IS_WRONG;
    }

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListVerifyNext(List_t* list, size_t* next_count_ptr)
{
    assert(list       != NULL);
    assert(list->data != NULL);

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
        if (list->data[list->data[i].next].prev != i)
        {
            free(next_nodes);
            return LIST_PREV_WRONG;
        }
        if (list->data[list->data[i].prev].next != i)
        {
            free(next_nodes);
            return LIST_NEXT_WRONG;
        }

        next_nodes[next_count++] = i;
    }

    free(next_nodes);

    *next_count_ptr = next_count;

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListVerifyPrev(List_t* list, size_t* prev_count_ptr)
{
    assert(list       != NULL);
    assert(list->data != NULL);

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
        if (list->data[list->data[i].next].prev != i)
        {
            free(prev_nodes);
            return LIST_PREV_WRONG;
        }
        if (list->data[list->data[i].prev].next != i)
        {
            free(prev_nodes);
            return LIST_NEXT_WRONG;
        }

        prev_nodes[prev_count++] = i;
    }

    free(prev_nodes);

    *prev_count_ptr = prev_count;

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListVerifyFree(List_t* list, size_t* free_count_ptr)
{
    assert(list       != NULL);
    assert(list->data != NULL);

    int*   free_nodes = (int*) calloc(list->capacity, sizeof(list->data[0].next));
    size_t free_count = 0;

    for (int i = list->free; i > 0; i = list->data[i].next)
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
        if (list->data[i].next < -1)
        {
            free(free_nodes);
            return LIST_FREE_NEXT_NEGATIVE;
        }
        if (list->data[i].next != -1 && (size_t) list->data[i].next >= list->capacity)
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

    *free_count_ptr = free_count;

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

    static char log_filename[MAX_FILENAME_LEN] = "";
    static char image_dir   [MAX_FILENAME_LEN] = "";
    static char dot_dir     [MAX_FILENAME_LEN] = "";

    if (calls_count == 1)
    {
        time_t rawtime = time(NULL);

        struct tm* info = localtime(&rawtime);

        char time_dir[MAX_FILENAME_LEN] = "";
        char dir[MAX_FILENAME_LEN] = "";

        strftime(time_dir, sizeof(time_dir), "%d%m%y_%H%M%S", info);

        sprintf(dir, "log/%s", time_dir);
        mkdir(dir, 0777);

        sprintf(image_dir, "log/%s/svg", time_dir);

        DPRINTF("image_dir = %s;\n", image_dir);
        mkdir(image_dir, 0777);

        sprintf(dot_dir, "log/%s/dot", time_dir);

        DPRINTF("dot_dir   = %s;\n", dot_dir);
        mkdir(dot_dir, 0777);

        sprintf(log_filename, "log/%s/list_log.html", time_dir);
    }

    FILE* fp = fopen(log_filename, calls_count == 1 ? "w" : "a");

    if (fp == NULL)
    {
        PRINTERR("Opening logfile %s failed", log_filename);
        return LIST_LOGFILE_OPEN_ERROR;
    }

    calls_count++;

    fprintf(fp, "<pre>\n<h3><font color=blue>%s%d</font></h3>",
                dump_info->reason,
                dump_info->command_arg);

    fprintf(fp, dump_info->error == LIST_SUCCESS ?
                "<font color=green><b>" :
                "<font color=red><b>ERROR: ");

    fprintf(fp, "%s (code %d)</b></font>\n"
                "LIST DUMP called from %s at %s:%d\n\n",
                LIST_STR_ERRORS[dump_info->error],
                dump_info->error,
                dump_info->func,
                dump_info->file,
                dump_info->line);

    if (ListDumpStruct(list, dump_info, fp))
    {
        fclose(fp);
        return LIST_SUCCESS;
    }

    if (dump_info->error == LIST_CAPACITY_EXCEEDS_MAX ||
        dump_info->error == LIST_HEAD_NEGATIVE        ||
        dump_info->error == LIST_HEAD_TOOBIG)
    {
        fclose(fp);
        return LIST_SUCCESS;
    }

    ListErr_t graph_error = LIST_SUCCESS;
    if ((graph_error = ListCreateDumpGraph(list, image_name, dot_dir)))
    {
        fclose(fp);
        return graph_error;
    }

    fprintf(fp, "\n<img src = svg/%s.svg width = 100%%>\n\n"
                "============================================================="
                "=============================================================\n\n",
                image_name);

    fclose(fp);

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

int ListDumpStruct(List_t* list, ListDumpInfo_t* dump_info, FILE* fp)
{
    assert(dump_info != NULL);
    assert(fp        != NULL);

    fprintf(fp, "list [%p]:\n\n"
                "—————————————————————————————————————————————————————————————"
                "—————————————————————————————————————————————————————————————\n\n",
                list);

    if (dump_info->error == LIST_CTX_NULL)
    {
        return 0;
    }

    fprintf(fp, "do_linear_realloc = %d;\n"
                "is_sorted = %d;\n"
                "capacity  = %zu;\n"
                "size = %zu;\n"
                "free = %d;\n",
                list->do_linear_realloc,
                list->is_sorted,
                list->capacity,
                list->size,
                list->free);

    if (dump_info->error == LIST_DATA_NULL)
    {
        return 0;
    }

    fprintf(fp, "head = %d;\n"
                "tail = %d;\n",
                list->data[0].next,
                list->data[0].prev);

    ListDumpData(list, dump_info, fp);

    fprintf(fp, "—————————————————————————————————————————————————————————————"
                "—————————————————————————————————————————————————————————————\n\n");

    return 0;
}

//------------------------------------------------------------------------------------------

int ListDumpData(List_t* list, ListDumpInfo_t* dump_info, FILE* fp)
{
    assert(list      != NULL);
    assert(dump_info != NULL);
    assert(fp        != NULL);

    fprintf(fp, "data [%p]:\n[\n", list->data);

    if (dump_info->error == LIST_CAPACITY_EXCEEDS_MAX)
    {
        return 0;
    }

    fprintf(fp, "\tindex  ");

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(fp, "%4zu ", i);
    }

    fprintf(fp, "\n\tvalue [");

    for (size_t i = 0; i < list->capacity; i++)
    {
        if (list->data[i].value == LIST_POISON)
        {
            fprintf(fp, " PZN ");
        }
        else
        {
            fprintf(fp, "%4d ", list->data[i].value);
        }
    }

    fprintf(fp, "];\n\tnext  [");

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(fp, "%4d ", list->data[i].next);
    }

    fprintf(fp, "];\n\tprev  [");

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(fp, "%4d ", list->data[i].prev);
    }

    fprintf(fp, "];\n];\n");

    return 0;
}

//------------------------------------------------------------------------------------------

ListErr_t ListCreateDumpGraph(List_t* list,
                              const char* image_name,
                              const char* dot_dir)
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

// NOTE: папка с именем в дату и время

    char filename[MAX_FILENAME_LEN] = {};

    sprintf(filename, "%s/%s.dot", dot_dir, image_name);

    FILE* fp = fopen(filename, "w");

    if (fp == NULL)
    {
        PRINTERR("Opening graph logfile failed");
        // fix double free here
        return LIST_LOGFILE_OPEN_ERROR;
    }

    fprintf(fp, "digraph GG\n{\n\t"
    R"(graph [splines=ortho];
    ranksep=0.75;
    nodesep=0.5;
    node [
        fontname  = "Arial",
        shape     = "Mrecord",
        style     = "filled",
        color     = "#3E3A22",
        fillcolor = "#E3DFC9",
        fontcolor = "#3E3A22"
    ];
    edge [constraint=false];)""\n");

    MakeListNodes(list, fp);

    /* make all edges */
    for (int pos = 1; pos < (int) list->capacity; pos++)
    {
        MakeListEdge(pos, list, fp);
    }

    MakeHeadTailFree(list, fp);

    fprintf(fp, "}\n");

    fclose(fp);

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

int MakeListNodes(List_t* list, FILE* fp)
{
    assert(list != NULL);
    assert(fp   != NULL);

    /* place all nodes in one rank */
    fprintf(fp, "\t{ rank=same; ");

    for (size_t ind = 0; ind < list->capacity; ind++)
    {
        fprintf(fp, "node%zu; ", ind);
    }

    fprintf(fp, "}\n");

    /* Create invisible edges for all nodes */
    for (int i = 0; i < (int) list->capacity - 1; i++)
    {
        MakeDefaultEdge(i, i + 1, NULL, NULL, NULL, "invis", NULL, NULL, fp);
    }

    MakeDefaultNode(0, "#3E3A22", "#ecede8", "#3E3A22", "record", list, fp);

    for (int i = 1; (size_t) i < list->capacity; i++)
    {
        /* if free */
        if (list->data[i].prev == -1)
        {
            MakeDefaultNode(i, "#006400", "#C0FFC0", "#005300", NULL, list, fp);
        }
        else
        {
            MakeDefaultNode(i, NULL, NULL, NULL, NULL, list, fp);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeListEdge(int pos, List_t* list, FILE* fp)
{
    assert(list != NULL);
    assert(fp   != NULL);

    int next = list->data[pos].next;
    int prev = list->data[pos].prev;

    if (prev == -1 && (next == 0 || next == -1))
    {
        return 0;
    }

    int next_limits_cross = (((size_t) next < list->capacity && next >= 0) == 0);
    int prev_limits_cross = (((size_t) prev < list->capacity && prev >= 0) == 0);

    if (ProcessFreeEdgeCase(pos, prev, next, next_limits_cross, fp) == 1)
    {
        return 0;
    }

    if (!(prev_limits_cross) && list->data[prev].next != pos)
    {
        if (list->data[list->data[prev].next].prev != prev)
        {
            MakeDefaultNode(prev, "#530000", "#FFC0C0", "#400000", NULL, list, fp);
        }
        MakeDefaultEdge(pos, prev, "#640000", NULL, NULL, "dashed", "dot", NULL, fp);
    }

    if (ProcessUncrossedLimitsEdge(pos, next,
                                   prev_limits_cross,
                                   next_limits_cross,
                                   list, fp) == 1)
    {
        return 0;
    }

    if (next_limits_cross)
    {
        MakeWrongNode(pos, next, "next", fp);
        MakeWrongEdge(pos,       "next", fp);
    }
    else
    {
        if (next != 0)
        {
            MakeDefaultEdge(pos, next, "#000064", NULL, NULL, NULL, NULL, NULL, fp);
        }
    }
    if (prev_limits_cross)
    {
        MakeWrongNode(pos, prev, "prev", fp);
        MakeWrongEdge(pos,       "prev", fp);
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int ProcessFreeEdgeCase(int   pos,
                        int   prev,
                        int   next,
                        int   next_limits_cross,
                        FILE* fp)
{
    assert(fp != NULL);

    if (prev != -1)
    {
        return 0;
    }

    if (next_limits_cross)
    {
        MakeWrongNode(pos, next, "next", fp);
        MakeWrongEdge(pos,       "next", fp);

        return 0;
    }

    MakeDefaultEdge(pos, next, "#006400", NULL, NULL, "dashed", NULL, NULL, fp);

    return 1;
}

//------------------------------------------------------------------------------------------

int ProcessUncrossedLimitsEdge(int     pos,
                               int     next,
                               int     prev_limits_cross,
                               int     next_limits_cross,
                               List_t* list,
                               FILE*   fp)
{
    assert(list != NULL);
    assert(fp   != NULL);

    if (next_limits_cross || prev_limits_cross)
    {
        return 0;
    }

    if (next == 0)
    {
        return 1;
    }

    if (list->data[next].prev == pos)
    {
        // NOTE: next and prev with different arrow heads
        MakeDefaultEdge(pos, next, "#000064", NULL, "both", NULL, "normal", "normal", fp);

        return 1;
    }

    DPRINTF("next.prev = %d\n", list->data[next].prev);

    if (list->data[next].prev < (int) list->capacity &&
        list->data[list->data[next].prev].next != next)
    {
        MakeDefaultNode(next, "#530000", "#FFC0C0", "#400000", NULL, list, fp);
    }

    MakeDefaultEdge(pos, next, "#640000", NULL, NULL, "dashed", NULL, NULL, fp);

    return 1;
}

//------------------------------------------------------------------------------------------

int MakeDefaultNode(int index,
                    const char* color,
                    const char* fillcolor,
                    const char* fontcolor,
                    const char* shape,
                    List_t* list,
                    FILE* fp)
{
    assert(fp        != NULL);
    assert(list      != NULL);

    char name[MAX_NODE_NAME_LEN] = "";

    sprintf(name, "node%d", index);

    char label[MAX_LABEL_LEN] = "";
    int  current_pos = 0;

    current_pos += sprintf(label, "{ idx = %d | value = ", index);

    if (list->data[index].value == LIST_POISON)
    {
        current_pos += sprintf(current_pos + label, "PZN");
    }
    else
    {
        current_pos += sprintf(current_pos + label, SPEC, list->data[index].value);
    }

    sprintf(current_pos + label, " | { prev = %d | next = %d }}",
                                 list->data[index].prev,
                                 list->data[index].next);

    MakeNode(name, label, color, fillcolor, fontcolor, shape, fp);

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeNode(const char* name,
             const char* label,
             const char* color,
             const char* fillcolor,
             const char* fontcolor,
             const char* shape,
             FILE* fp)
{
    assert(name != NULL);
    assert(fp   != NULL);

    fprintf(fp, "\t%s", name);

    int is_first_arg = 1;

    PrintArg("label",     label,     &is_first_arg, fp);
    PrintArg("color",     color,     &is_first_arg, fp);
    PrintArg("fillcolor", fillcolor, &is_first_arg, fp);
    PrintArg("fontcolor", fontcolor, &is_first_arg, fp);
    PrintArg("shape",     shape,     &is_first_arg, fp);

    if (is_first_arg == 0)
    {
        fprintf(fp, "]");
    }

    fprintf(fp, ";\n");

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeWrongNode(int         pos,
                  int         value,
                  const char* connection,
                  FILE*       fp)
{
    assert(connection != NULL);
    assert(fp         != NULL);

    char name [MAX_NODE_NAME_LEN] = "";
    char label[MAX_LABEL_LEN]     = "";

    sprintf (name,  "wrong_%s_%d", connection, pos);
    sprintf (label, "idx = %d",    value);

    MakeNode(name, label, "#640000", "#FFC0C0", "#640000", "octagon", fp);

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeDefaultEdge(int         index1,
                    int         index2,
                    const char* color,
                    const char* constraint,
                    const char* dir,
                    const char* style,
                    const char* arrowhead,
                    const char* arrowtail,
                    FILE*       fp)
{
    assert(fp != NULL);

    char node1[MAX_NODE_NAME_LEN] = "";
    char node2[MAX_NODE_NAME_LEN] = "";

    sprintf (node1, "node%d", index1);
    sprintf (node2, "node%d", index2);

    MakeEdge(node1, node2, color, constraint, dir, style, arrowhead, arrowtail, fp);

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeWrongEdge(int         pos,
                  const char* connection,
                  FILE*       fp)
{
    assert(connection != NULL);
    assert(fp         != NULL);

    char node1[MAX_NODE_NAME_LEN] = "";
    char node2[MAX_NODE_NAME_LEN] = "";

    sprintf(node1, "node%d",                  pos);
    sprintf(node2, "wrong_%s_%d", connection, pos);

    if (strcmp(connection, "prev"))
    {
        MakeEdge(node1, node2, "#640000", "true", NULL, NULL, "dot", NULL, fp);
    }
    else
    {
        MakeEdge(node1, node2, "#640000", "true", NULL, NULL, NULL, NULL, fp);
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeEdge(const char* node1,
             const char* node2,
             const char* color,
             const char* constraint,
             const char* dir,
             const char* style,
             const char* arrowhead,
             const char* arrowtail,
             FILE*       fp)
{
    assert(node1 != NULL);
    assert(node2 != NULL);

    fprintf(fp, "\t%s->%s", node1, node2);

    int is_first_arg = 1;

    PrintArg("color",      color,      &is_first_arg, fp);
    PrintArg("constraint", constraint, &is_first_arg, fp);
    PrintArg("dir",        dir,        &is_first_arg, fp);
    PrintArg("style",      style,      &is_first_arg, fp);
    PrintArg("arrowhead",  arrowhead,  &is_first_arg, fp);
    PrintArg("arrowtail",  arrowtail,  &is_first_arg, fp);

    if (is_first_arg == 0)
    {
        fprintf(fp, "]");
    }

    fprintf(fp, ";\n");

    return 0;
}

//------------------------------------------------------------------------------------------

int PrintArg(const char* arg_name,
             const char* arg_value,
             int*        is_first_arg,
             FILE*       fp)
{
    assert(arg_name     != NULL);
    assert(is_first_arg != NULL);

    if (arg_value != NULL)
    {
        if (*is_first_arg)
        {
            fprintf(fp, " [");
            *is_first_arg = 0;
        }
        else
        {
            fprintf(fp, ", ");
        }

        fprintf(fp, "%s = \"%s\"", arg_name, arg_value);
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeHeadTailFree(List_t* list, FILE* fp)
{
    assert(list != NULL);
    assert(fp   != NULL);

    fprintf(fp,
    R"(    node [
        shape = "box",
        color = "#70421A",
        fontcolor = "#70421A",
        fillcolor = "#DEB887"
    ];
    edge [
        color = "#70421A",
        arrowhead=none,
        constraint=true
    ];
    tail; head; free;)""\n");

    /* Make edges to the elements */
    if (list->data[0].prev >= 0)
    {
        char name[MAX_NODE_NAME_LEN] = "";
        sprintf (name, "node%d", list->data[0].prev);
        MakeEdge("tail", name, NULL, NULL, NULL, NULL, NULL, NULL, fp);
    }
    if (list->data[0].next >= 0)
    {
        char name[MAX_NODE_NAME_LEN] = "";
        sprintf (name, "node%d", list->data[0].next);
        MakeEdge("head", name, NULL, NULL, NULL, NULL, NULL, NULL, fp);
    }
    if (list->free >= 0)
    {
        char name[MAX_NODE_NAME_LEN] = "";
        sprintf (name, "node%d", list->free);
        MakeEdge("free", name, NULL, NULL, NULL, NULL, NULL, NULL, fp);
    }

    /* Place on one rank */
    fprintf(fp, "\t{ rank=same; tail; head; free; }\n");

    return 0;
}

//==========================================================================================

#endif /* LIST_DEBUG */

//------------------------------------------------------------------------------------------
