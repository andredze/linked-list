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

//------------------------------------------------------------------------------------------

int main()
{
    DPRINTF(BLUE "\nRunning tests\n\n" RESET_CLR);

#ifdef STD_LIST

    StdList_t std_list = {};

    clock_t std_start_time = 0;
    clock_t std_end_time   = 0;

    do
    {
        StdNode_t* insert_node = NULL;
        StdNode_t* root_node   = NULL;

        if (StdListCtor (&std_list, &root_node))
            break;

        std_start_time = clock();

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

        StdNode_t* new_ins_node = 0;

        for (int i = 0; i < 10000; i++)
        {
            if (StdListInsertBefore(&std_list, insert_node, 100, &new_ins_node))
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

        if (StdListEraseElem(&std_list, insert_node))
            break;
    }
    while (0);


    std_end_time = clock();

    if (StdListDtor(&std_list))
    {
        return EXIT_FAILURE;
    }

#endif
#ifdef AOS_LIST

    List_t list = {};

    clock_t aos_start_time = 0;
    clock_t aos_end_time   = 0;

    do
    {
        int insert_pos = 0;

        if (ListCtor(&list, 10, 1))
            break;

        aos_start_time = clock();

        if (ListInsertAfter(&list, 0, 25, &insert_pos))
            break;

        if (ListInsertAfter(&list, insert_pos, 25, &insert_pos))
            break;

        if (ListInsertAfter(&list, 0, 20, &insert_pos))
            break;

        if (ListInsertBefore(&list, 0, 35, &insert_pos))
            break;

        // list.data[2].prev = 3;
        // list.data[3].prev = 84;

    #ifdef LIST_DEBUG
        ListCheck(&list, "PREV = ", "main", "main.cpp", 57, 84);
    #endif /* LIST_DEBUG */

        if (ListInsertBefore(&list, insert_pos, 32, &insert_pos))
            break;

        // if (ListEraseElem   (&list, insert_pos))
        //     break;

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

        int new_ins_pos = 0;

        for (int i = 0; i < 10000; i++)
        {
            if (ListInsertBefore(&list, insert_pos, 100, &new_ins_pos))
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

        if (ListEraseElem(&list, insert_pos))
            break;
    }
    while (0);

    aos_end_time = clock();

    if (ListDtor(&list))
    {
        return EXIT_FAILURE;
    }

#endif

#ifdef STD_LIST
    double std_run_time = ((double) (std_end_time - std_start_time)) / CLOCKS_PER_SEC;
    printf("Standard list run time       = %lg;\n", (double) std_run_time);
#endif
#ifdef AOS_LIST
    double aos_run_time = ((double) (aos_end_time - aos_start_time)) / CLOCKS_PER_SEC;
    printf("Array of structures run time = %lg;\n", aos_run_time);
#endif

#ifdef STD_LIST
#ifdef AOS_LIST
    printf("Absolute difference          = %lg;\n", std_run_time - aos_run_time);
    printf("How faster? (std/aos)        = %lg;\n", std_run_time / aos_run_time);
    printf("Relative difference          = %lg%%;\n", 100 * (std_run_time - aos_run_time) / aos_run_time);
#endif
#endif

    DPRINTF(GREEN "Tests ran successfully\n\n" RESET_CLR);

    return EXIT_SUCCESS;
}
