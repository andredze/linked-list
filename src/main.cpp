#ifdef AOS_LIST
    #include "listCommands.h"
    #include "listDebug.h"
#endif
#ifdef STD_LIST
    #include "stdListCommands.h"
    #include "stdListDebug.h"
#endif

#include "common.h"
#include <time.h>
#include <intrin.h>
#include <windows.h>

//------------------------------------------------------------------------------------------

int main()
{
    DPRINTF(BLUE "\nRunning tests\n\n" RESET_CLR);

#ifdef STD_LIST

    StdList_t std_list = {};

    unsigned __int64 std_start_ticks = 0;
    unsigned __int64 std_end_ticks   = 0;

    do
    {
        StdNode_t* insert_node = NULL;
        StdNode_t* root_node   = NULL;

        std_start_ticks = __rdtsc();

        if (StdListCtor (&std_list, &root_node))
            break;

        if (StdListInsertAfter (&std_list, root_node,   25, &insert_node))
            break;

        if (StdListInsertAfter (&std_list, insert_node, 30, &insert_node))
            break;

        if (StdListInsertAfter (&std_list, root_node,   20, &insert_node))
            break;

        if (StdListInsertBefore(&std_list, root_node,   35, &insert_node))
            break;

        if (StdListInsertBefore(&std_list, insert_node, 32, &insert_node))
            break;

        if (StdListInsertAfter (&std_list, root_node,   25, &insert_node))
            break;

        if (StdListInsertAfter (&std_list, insert_node, 30, &insert_node))
            break;

        if (StdListInsertAfter (&std_list, root_node,   20, &insert_node))
            break;

        if (StdListInsertBefore(&std_list, root_node,   35, &insert_node))
            break;

        if (StdListInsertBefore(&std_list, insert_node, 1000, &insert_node))
            break;

#ifndef STD_LIST_DEBUG
        StdNode_t* new_ins_node = 0;

        for (int j = 0; j < 3000; j++)
        {
            for (int i = 0; i < 100000; i++)
            {
                if (StdListInsertBefore(&std_list, insert_node, 100, &new_ins_node))
                {
                    StdListDtor(&std_list);
                    return EXIT_FAILURE;
                }
                if (StdListInsertAfter(&std_list, insert_node, 110, &new_ins_node))
                {
                    StdListDtor(&std_list);
                    return EXIT_FAILURE;
                }
                if (StdListEraseElem(&std_list, new_ins_node))
                {
                    StdListDtor(&std_list);
                    return EXIT_FAILURE;
                }
            }
        }
#endif /* STD_LIST_DEBUG */

        if (StdListEraseElem(&std_list, insert_node))
            break;
    }
    while (0);

    if (StdListDtor(&std_list))
    {
        return EXIT_FAILURE;
    }

    std_end_ticks = __rdtsc();

#endif
#ifdef AOS_LIST

    Sleep(50000);

    List_t list = {};

    unsigned __int64 aos_start_ticks = 0;
    unsigned __int64 aos_end_ticks   = 0;

    do
    {
        int insert_pos = 0;

        aos_start_ticks = __rdtsc();

        if (ListCtor(&list, 4))
            break;

        if (ListInsertAfter(&list, 0, 25, &insert_pos))
            break;

        if (ListInsertAfter(&list, insert_pos, 25, &insert_pos))
            break;

        if (ListInsertAfter(&list, 0, 20, &insert_pos))
            break;

        if (ListInsertBefore(&list, 0, 35, &insert_pos))
            break;

    #ifdef LIST_DEBUG
        LIST_CALL_DUMP(&list, "normal", "LIST IS NORMAL");
        // list.data[2].prev = 3;
        // ListCheck(&list, "RUINED LIST (list is looped): list.data[3].prev = ", __func__, __FILE__, __LINE__, 3);
        list.data[3].prev = 84;
        ListCheck(&list, __func__, __FILE__, __LINE__, "RUINED LIST: list.data[3].prev = %d", list.data[3].prev);
        // list.size = 84;
        // ListCheck(&list, "RUINED LIST: size = ", __func__, __FILE__, __LINE__, 84);
    #endif /* LIST_DEBUG */

        if (ListInsertBefore(&list, insert_pos, 32, &insert_pos))
            break;

        if (ListInsertAfter(&list, 0, 25, &insert_pos))
            break;

        if (ListInsertAfter(&list, insert_pos, 25, &insert_pos))
            break;

        if (ListInsertAfter(&list, 0, 20, &insert_pos))
            break;

        if (ListInsertBefore(&list, 0, 35, &insert_pos))
            break;

        if (ListInsertBefore(&list, insert_pos, 32, &insert_pos))
            break;

#ifndef LIST_DEBUG
        int new_ins_pos = 0;

        for (int j = 0; j < 3000; j++)
        {
            for (int i = 0; i < 100000; i++)
            {
                if (ListInsertBefore(&list, insert_pos, 100, &new_ins_pos))
                {
                    ListDtor(&list);
                    return EXIT_FAILURE;
                }
                if (ListInsertAfter(&list, insert_pos, 110, &new_ins_pos))
                {
                    ListDtor(&list);
                    return EXIT_FAILURE;
                }
                if (ListEraseElem(&list, new_ins_pos))
                {
                    ListDtor(&list);
                    return EXIT_FAILURE;
                }
            }
        }
#endif /* LIST_DEBUG */

        if (ListEraseElem(&list, insert_pos))
            break;

//         if (ListReallocDownLinear(&list))
//             break;
    }
    while (0);

    if (ListDtor(&list))
    {
        return EXIT_FAILURE;
    }

    aos_end_ticks = __rdtsc();

#endif

#ifdef STD_LIST
    signed __int64 std_run_ticks = std_end_ticks - std_start_ticks;
    printf("Standard list run ticks       = %lld;\n", std_run_ticks);
#endif
#ifdef AOS_LIST
    signed __int64 aos_run_ticks = aos_end_ticks - aos_start_ticks;
    printf("Array of structures run ticks = %lld;\n", aos_run_ticks);
#endif

#ifdef STD_LIST
#ifdef AOS_LIST
    printf("Absolute difference          = %lld;\n", std_run_ticks - aos_run_ticks);
    printf("How faster? (std/aos)        = %lg;\n", ((double) std_run_ticks / (double) aos_run_ticks));
    printf("Relative difference          = %lg%%;\n", 100 * ((double) (std_run_ticks - aos_run_ticks) / (double) aos_run_ticks));
#endif
#endif

    DPRINTF(GREEN "Tests ran successfully\n\n" RESET_CLR);

    return EXIT_SUCCESS;
}
