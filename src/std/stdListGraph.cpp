#include "stdListGraph.h"

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef STD_LIST_DEBUG

//==========================================================================================

StdListErr_t StdListCreateDumpGraph(StdList_t* list,
                                    const char* image_name,
                                    const char* dot_dir)
{
    assert(list != NULL);

    if (image_name == NULL)
    {
        PRINTERR("Given image name is null\n");
        return STD_LIST_IMAGE_NAME_NULL;
    }
    if (strlen(image_name) > MAX_FILENAME_LEN / 2)
    {
        PRINTERR("Too big filename for graph image");
        return STD_LIST_FILENAME_TOOBIG;
    }

    char filename[MAX_FILENAME_LEN] = {};

    sprintf(filename, "%s/%s.dot", dot_dir, image_name);

    FILE* fp = fopen(filename, "w");

    if (fp == NULL)
    {
        PRINTERR("Opening graph logfile failed");
        // fix double free here
        return STD_LIST_LOGFILE_OPEN_ERROR;
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

    MakeStdListNodes(list, fp);

    /* make all edges */
    StdNode_t* next = NULL;

    for (StdNode_t* node = list->root; next != list->root; node = next)
    {
        MakeStdListEdge(node, list, fp);
        next = node->next;
    }

    MakeStdListHeadTail(list, fp);

    fprintf(fp, "}\n");

    fclose(fp);

    return STD_LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

int MakeStdListNodes(StdList_t* list,
                     FILE*      fp)
{
    assert(list != NULL);
    assert(fp   != NULL);

    /* place all nodes in one rank */
    fprintf(fp, "\t{ rank=same; ");

    StdNode_t* next = NULL;

    for (StdNode_t* node = list->root; next != list->root; node = next)
    {
        fprintf(fp, "node%p; ", node);
        next = node->next;
    }
    next = NULL;

    fprintf(fp, "}\n");

    /* Create invisible edges for all nodes */
    for (StdNode_t* node = list->root; next != list->root; node = next)
    {
        next = node->next;
        MakeStdListDefaultEdge(node, next, NULL, NULL, NULL, "invis", NULL, NULL, fp);
    }
    next = NULL;

    MakeStdListDefaultNode(list->root, "#3E3A22", "#ecede8", "#3E3A22", "record", list, fp);

    for (StdNode_t* node = list->root; next != list->root; node = next)
    {
        MakeStdListDefaultNode(node, NULL, NULL, NULL, NULL, list, fp);
        next = node->next;
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeStdListDefaultEdge (StdNode_t*  node1,
                            StdNode_t*  node2,
                            const char* color,
                            const char* constraint,
                            const char* dir,
                            const char* style,
                            const char* arrowhead,
                            const char* arrowtail,
                            FILE*       fp)
{
    assert(fp != NULL);

    char name_node1[MAX_NODE_NAME_LEN] = "";
    char name_node2[MAX_NODE_NAME_LEN] = "";

    sprintf (name_node1, "node%p", node1);
    sprintf (name_node2, "node%p", node2);

    MakeEdge(name_node1, name_node2, color, constraint, dir, style, arrowhead, arrowtail, fp);

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeStdListEdge(StdNode_t* node,
                    StdList_t* list,
                    FILE*      fp)
{
    assert(list != NULL);
    assert(fp   != NULL);

    StdNode_t* next = node->next;
    StdNode_t* prev = node->prev;

    if ((*node->prev).next != node)
    {
        if (((*node->prev).next)->prev != prev)
        {
            MakeStdListDefaultNode(prev, "#530000", "#FFC0C0", "#400000", NULL, list, fp);
        }
        MakeStdListDefaultEdge(node, prev, "#640000", NULL, NULL, "dashed", "dot", NULL, fp);
    }

    if (StdListProcessUncrossedLimitsEdge(node, next, list, fp) == 1)
    {
        return 0;
    }

    if (next != list->root)
    {
        MakeStdListDefaultEdge(node, next, "#000064", NULL, NULL, NULL, NULL, NULL, fp);
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int StdListProcessUncrossedLimitsEdge(StdNode_t* node,
                                      StdNode_t* next,
                                      StdList_t* list,
                                      FILE*   fp)
{
    assert(list != NULL);
    assert(fp   != NULL);

    if (next == list->root)
    {
        return 1;
    }

    if ((*node->next).prev == node)
    {
        // NOTE: next and prev with different arrow heads
        MakeStdListDefaultEdge(node, next, "#000064", NULL, "both", NULL, "normal", "normal", fp);

        return 1;
    }

    return 1;
}

//------------------------------------------------------------------------------------------

int MakeStdListDefaultNode(StdNode_t* node,
                           const char* color,
                           const char* fillcolor,
                           const char* fontcolor,
                           const char* shape,
                           StdList_t* list,
                           FILE* fp)
{
    assert(fp   != NULL);
    assert(list != NULL);

    char name[MAX_NODE_NAME_LEN] = "";

    sprintf(name, "node%p", node);

    char label[MAX_LABEL_LEN] = "";
    int  current_pos = 0;

    current_pos += sprintf(label, "{ ptr = %p | value = ", node);

    current_pos += sprintf(current_pos + label, SPEC, node->value);

    sprintf(current_pos + label, " | { prev = %p | next = %p }}",
                                    node->prev,
                                    node->next);

    MakeNode(name, label, color, fillcolor, fontcolor, shape, fp);

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeStdListHeadTail(StdList_t* list, FILE* fp)
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
    tail; head;)""\n");

    /* Make edges to the elements */
    if ((*list->root).prev != NULL)
    {
        char name[MAX_NODE_NAME_LEN] = "";
        sprintf (name, "node%p", (*list->root).prev);
        MakeEdge("tail", name, NULL, NULL, NULL, NULL, NULL, NULL, fp);
    }
    if ((*list->root).next != NULL)
    {
        char name[MAX_NODE_NAME_LEN] = "";
        sprintf (name, "node%p", (*list->root).next);
        MakeEdge("head", name, NULL, NULL, NULL, NULL, NULL, NULL, fp);
    }

    /* Place on one rank */
    fprintf(fp, "\t{ rank=same; tail; head; }\n");

    return 0;
}

//==========================================================================================

#endif /* STD_LIST_DEBUG */

//------------------------------------------------------------------------------------------
