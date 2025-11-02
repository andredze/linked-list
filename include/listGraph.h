#ifndef LIST_GRAPH_H
#define LIST_GRAPH_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "listDebug.h"
#include "listTypes.h"

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

//——————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListCreateDumpGraph(
    List_t* list,
    const char* image_name,
    const char* dot_dir);

int MakeListNodes(
    List_t* list,
    FILE*   fp);

int MakeListEdge(
    int     pos,
    List_t* list,
    FILE*   fp);

int MakeHeadTailFree(
    List_t*     list,
    FILE*       fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int ProcessFreeEdgeCase(
    int   pos,
    int   prev,
    int   next,
    int   next_limits_cross,
    FILE* fp);

int ProcessUncrossedLimitsEdge(
    int     pos,
    int     next,
    int     prev_limits_cross,
    int     next_limits_cross,
    List_t* list,
    FILE*   fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int MakeNode(
    const char* name,
    const char* label,
    const char* color,
    const char* fillcolor,
    const char* fontcolor,
    const char* shape,
    FILE*       fp);

int MakeDefaultNode(
    int         index,
    const char* color,
    const char* fillcolor,
    const char* fontcolor,
    const char* shape,
    List_t*     list,
    FILE*       fp);

int MakeWrongNode(
    int         pos,
    int         value,
    const char* connection,
    FILE*       fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int MakeDefaultEdge(
    int         index1,
    int         index2,
    const char* color,
    const char* constraint,
    const char* dir,
    const char* style,
    const char* arrowhead,
    const char* arrowtail,
    FILE*       fp);

int MakeWrongEdge(
    int         pos,
    const char* connection,
    FILE*       fp);

int MakeEdge(
    const char* node1,
    const char* node2,
    const char* color,
    const char* constraint,
    const char* dir,
    const char* style,
    const char* arrowhead,
    const char* arrowtail,
    FILE*       fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int PrintArg(
    const char* arg_name,
    const char* arg_value,
    int*        is_first_arg,
    FILE*       fp);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_GRAPH_H */
