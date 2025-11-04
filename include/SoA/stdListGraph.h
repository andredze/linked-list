#ifndef STD_LIST_GRAPH_H
#define STD_LIST_GRAPH_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "stdListDebug.h"
#include "stdListTypes.h"
#include "graphCommon.h"

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef STD_LIST_DEBUG

//——————————————————————————————————————————————————————————————————————————————————————————

StdListErr_t StdListCreateDumpGraph(
    StdList_t* list,
    const char* image_name,
    const char* dot_dir);

int MakeStdListNodes(
    StdList_t* list,
    FILE*   fp);

int MakeStdListEdge(
    int     pos,
    StdList_t* list,
    FILE*   fp);

int MakeStdListHeadTailFree(
    StdList_t* list,
    FILE* fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int StdListProcessUncrossedLimitsEdge(int     pos,
                                      int     next,
                                      int     prev_limits_cross,
                                      int     next_limits_cross,
                                      StdList_t* list,
                                      FILE*   fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int MakeStdListDefaultNode(int index,
                           const char* color,
                           const char* fillcolor,
                           const char* fontcolor,
                           const char* shape,
                           StdList_t* list,
                           FILE* fp);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* STD_LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* STD_LIST_GRAPH_H */
