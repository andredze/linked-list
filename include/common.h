#ifndef COMMON_H
#define COMMON_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "colors.h"

//——————————————————————————————————————————————————————————————————————————————————————————

#define BEGIN do {

#define END   } while (0)

//——————————————————————————————————————————————————————————————————————————————————————————

#define PRINTERR(text, ...)                                                     \
        BEGIN                                                                   \
        fprintf(stderr, RED "In %s:%d from %s:\nERROR: " text "\n" RESET_CLR,   \
                __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);        \
        END

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG
    #define DPRINTF(text, ...)                                          \
            BEGIN                                                       \
            fprintf(stderr, YELLOW text RESET_CLR, ##__VA_ARGS__);      \
            END
#else
    #define DPRINTF(...) ;
#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

const int MAX_FILENAME_LEN = 100;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* COMMON_H */
