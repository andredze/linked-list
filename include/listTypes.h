#ifndef LIST_TYPES_H
#define LIST_TYPES_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "common.h"
#include <stdint.h>

//——————————————————————————————————————————————————————————————————————————————————————————

typedef int elem_t;

#define SPEC "%d"

const elem_t LIST_POISON = 0xAB0BA;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct Node
{
    elem_t value;
    int    next;
    int    prev;
} Node_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct List
{
    Node_t*  data;
    size_t   capacity;
    size_t   size;
    int      free;
    int      do_linear_realloc;
    int      is_sorted;
} List_t;

//——————————————————————————————————————————————————————————————————————————————————————————

const size_t LIST_MIN_CAPACITY = 4;
const size_t LIST_MAX_CAPACITY = 1024 * 1024 * 1024;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_TYPES_H */
