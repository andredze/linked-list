#include "stdListDebug.h"

#ifdef STD_LIST_DEBUG

//==========================================================================================

StdListErr_t StdListCheck(StdList_t*  list,
                          const char* message,
                          const char* func,
                          const char* file,
                          int         line,
                          size_t      arg)
{
    StdListErr_t verify_status = STD_LIST_SUCCESS;
    if ((verify_status = StdListVerify(list)))
    {
        PRINTERR("%s (StdListVerify not passed! Check \"list_log.htm\")",
                 STD_LIST_STR_ERRORS[verify_status]);
        StdListDumpInfo_t dump_info = {verify_status, "error_dump", message,
                                       func, file, line, arg};

        if (StdListDump(list, &dump_info))
        {
            return STD_LIST_DUMP_ERROR;
        }
    }

    return verify_status;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListVerify(StdList_t* list)
{
    if (list == NULL)
    {
        return STD_LIST_NULL;
    }
    if (list->root == NULL)
    {
        return STD_LIST_HEAD_NULL;
    }
    if (list->size > STD_LIST_MAX_SIZE)
    {
        return STD_LIST_SIZE_EXCEEDS_MAX;
    }

    size_t next_count = 0;

    StdListErr_t error = STD_LIST_SUCCESS;

    if ((error = StdListVerifyNext(list, &next_count)) != STD_LIST_SUCCESS)
    {
        return error;
    }

    if (next_count != list->size)
    {
        DPRINTF("next_count = %zu;\nsize = %zu;\n", next_count, list->size);
        return STD_LIST_SIZE_IS_WRONG;
    }

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListVerifyNext(StdList_t* list, size_t* next_count_ptr)
{
    assert(list       != NULL);
    assert(list->root != NULL);

    if (list->root->next == NULL)
    {
        return STD_LIST_SUCCESS;
    }

    StdNode_t** next_nodes = (StdNode_t**) calloc(list->size, sizeof(StdNode_t*));
    size_t      next_count = 0;

    for (StdNode_t* node = list->root->next; node != list->root; node = node->next)
    {
        for (size_t i = 0; i < next_count; i++)
        {
            if (next_nodes[i] == node)
            {
                return STD_LIST_LOOP;
            }
        }
        if (node->prev == NULL)
        {
            return STD_NODE_PREV_NULL;
        }
        if (node->next == NULL)
        {
            return STD_NODE_NEXT_NULL;
        }
        if ((*node->next).prev != node)
        {
            return STD_NODE_WRONG_PREV;
        }

        next_nodes[next_count++] = node;
    }

    free(next_nodes);

    *next_count_ptr = next_count;

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

StdListErr_t StdListDump(StdList_t* list, StdListDumpInfo_t* dump_info)
{
    static int calls_count = 1;

    static char log_filename [MAX_FILENAME_LEN] = "";
    static char image_dir    [MAX_FILENAME_LEN] = "";
    static char dot_dir      [MAX_FILENAME_LEN] = "";

    if (calls_count == 1)
    {
        StdListSetDirectories(log_filename, image_dir, dot_dir);
    }

    char image_name[MAX_FILENAME_LEN] = {};
    sprintf(image_name, "%04d_%s", calls_count, dump_info->image_name);

    FILE* fp = fopen(log_filename, calls_count == 1 ? "w" : "a");

    if (fp == NULL)
    {
        PRINTERR("Opening logfile %s failed", log_filename);
        return STD_LIST_LOGFILE_OPEN_ERROR;
    }

    calls_count++;

    fprintf(fp, "<pre>\n<h3><font color=blue>%s%zu</font></h3>",
                dump_info->reason,
                dump_info->command_arg);

    fprintf(fp, dump_info->error == STD_LIST_SUCCESS ?
                "<font color=green><b>" :
                "<font color=red><b>ERROR: ");

    fprintf(fp, "%s (code %d)</b></font>\n"
                "STD_LIST DUMP called from %s at %s:%d\n\n",
                STD_LIST_STR_ERRORS[dump_info->error],
                dump_info->error,
                dump_info->func,
                dump_info->file,
                dump_info->line);

    if (StdListDumpStruct(list, dump_info, fp))
    {
        fclose(fp);
        return STD_LIST_SUCCESS;
    }

    StdListErr_t graph_error = STD_LIST_SUCCESS;
    if ((graph_error = StdListCreateDumpGraph(list, image_name, dot_dir)))
    {
        fclose(fp);
        return graph_error;
    }

    fprintf(fp, "\n<img src = svg/%s.svg width = 100%%>\n\n"
                "============================================================="
                "=============================================================\n\n",
                image_name);

    fclose(fp);

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

int StdListSetDirectories(char* log_filename, char* image_dir, char* dot_dir)
{
    assert(log_filename != NULL);
    assert(image_dir    != NULL);
    assert(dot_dir      != NULL);

    time_t rawtime = time(NULL);

    struct tm* info = localtime(&rawtime);

    char time_dir[MAX_FILENAME_LEN / 2] = "";
    char dir[MAX_FILENAME_LEN] = "";

    strftime(time_dir, sizeof(time_dir), "%d%m%Y_%H%M%S", info);

    snprintf(dir, 100, "std_log/%s", time_dir);
    mkdir(dir, 0777);

    sprintf(image_dir, "std_log/%s/svg", time_dir);

    DPRINTF("image_dir = %s;\n", image_dir);
    mkdir(image_dir, 0777);

    sprintf(dot_dir, "std_log/%s/dot", time_dir);

    DPRINTF("dot_dir   = %s;\n", dot_dir);
    mkdir(dot_dir, 0777);

    sprintf(log_filename, "std_log/%s/list_log.html", time_dir);

    return 0;
}

//------------------------------------------------------------------------------------------

int StdListDumpStruct(StdList_t* list, StdListDumpInfo_t* dump_info, FILE* fp)
{
    assert(dump_info != NULL);
    assert(fp        != NULL);

    fprintf(fp, "list [%p]:\n\n"
                "—————————————————————————————————————————————————————————————"
                "—————————————————————————————————————————————————————————————\n\n",
                list);

    if (dump_info->error == STD_LIST_NULL)
    {
        return 0;
    }

    fprintf(fp, "size = %zu;\n", list->size);

    if (dump_info->error == STD_LIST_HEAD_NULL)
    {
        return 0;
    }

    fprintf(fp, "head = %p;\n", list->root);

    StdListDumpData(list, dump_info, fp);

    fprintf(fp, "—————————————————————————————————————————————————————————————"
                "—————————————————————————————————————————————————————————————\n\n");

    return 0;
}

//------------------------------------------------------------------------------------------

int StdListDumpData(StdList_t* list, StdListDumpInfo_t* dump_info, FILE* fp)
{
    assert(list      != NULL);
    assert(dump_info != NULL);
    assert(fp        != NULL);

    fprintf(fp, "\tptr    ");

    StdNode_t* next = NULL;

    for (StdNode_t* node = list->root; next != list->root; node = next)
    {
        fprintf(fp, "%14p ", node);
        next = node->next;
    }

    fprintf(fp, "\n\tvalue [");

    next = NULL;
    for (StdNode_t* node = list->root; next != list->root; node = next)
    {
        fprintf(fp, "%14d ", node->value);
        next = node->next;
    }

    fprintf(fp, "];\n\tnext  [");

    next = NULL;
    for (StdNode_t* node = list->root; next != list->root; node = next)
    {
        fprintf(fp, "%14p ", node->next);
        next = node->next;
    }

    fprintf(fp, "];\n\tprev  [");

    next = NULL;
    for (StdNode_t* node = list->root; next != list->root; node = next)
    {
        fprintf(fp, "%14p ", node->prev);
        next = node->next;
    }

    fprintf(fp, "];\n];\n");

    return 0;
}

//==========================================================================================

#endif /* STD_LIST_DEBUG */

//------------------------------------------------------------------------------------------
