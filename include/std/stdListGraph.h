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

int MakeStdListEdge(StdNode_t* node,
                    StdList_t* list,
                    FILE*      fp);

int MakeStdListHeadTail(StdList_t* list, FILE* fp);

int MakeStdListDefaultEdge (StdNode_t*  node1,
                            StdNode_t*  node2,
                            const char* color,
                            const char* constraint,
                            const char* dir,
                            const char* style,
                            const char* arrowhead,
                            const char* arrowtail,
                            FILE*       fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int StdListProcessUncrossedLimitsEdge(StdNode_t* node,
                                      StdNode_t* next,
                                      StdList_t* list,
                                      FILE*   fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int MakeStdListDefaultNode(StdNode_t* node,
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
