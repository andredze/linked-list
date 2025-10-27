#ifndef COMMON_H
#define COMMON_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include "colors.h"

//——————————————————————————————————————————————————————————————————————————————————————————

#define BEGIN do {

#define END   } while (0)

//——————————————————————————————————————————————————————————————————————————————————————————

#define SAFE_CALL(call)                         \
        BEGIN                                   \
        ListErr_t error = LIST_SUCCESS;         \
        if ((error = call) != LIST_SUCCESS)     \
        {                                       \
            return error;                       \
        }                                       \
        END

//——————————————————————————————————————————————————————————————————————————————————————————

#define PRINTERR(error)                                                 \
        BEGIN                                                           \
        fprintf(stderr, RED "ERROR: %s in %s:%d from %s\n" RESET_CLR,   \
                error, __FILE__, __LINE__, __PRETTY_FUNCTION__);        \
        END

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG
    #define DPRINTF(text, ...)                                        \
            BEGIN                                                     \
            fprintf(stderr, YELLOW text RESET_CLR, __VA_ARGS__);      \
            END
#else
    #define DPRINTF(...) ;
#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* COMMON_H */
