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

    clock_t std_start_time = clock();

    StdList_t std_list_data = {};

    BEGIN

    StdNode_t* insert_node = NULL;
    StdNode_t* head_node   = NULL;

    if (StdListCtor (&std_list_data, &head_node))
        break;

    if (StdListInsertAfter (&std_list_data, head_node,   25, &insert_node))
        break;

    if (StdListInsertAfter (&std_list_data, insert_node, 30, &insert_node))
        break;

    if (StdListInsertAfter (&std_list_data, head_node,   20, &insert_node))
        break;

    if (StdListInsertBefore(&std_list_data, head_node,   35, &insert_node))
        break;

    if (StdListInsertBefore(&std_list_data, insert_node, 32, &insert_node))
        break;

    if (StdListInsertAfter (&std_list_data, head_node,   25, &insert_node))
        break;

    if (StdListInsertAfter (&std_list_data, insert_node, 30, &insert_node))
        break;

    if (StdListInsertAfter (&std_list_data, head_node,   20, &insert_node))
        break;

    if (StdListInsertBefore(&std_list_data, head_node,   35, &insert_node))
        break;

    if (StdListInsertBefore(&std_list_data, insert_node, 1000, &insert_node))
        break;

    // for (int i = 0; i < 10000; i++)
    // {
    //     if (StdListInsertBefore(&std_list_data, insert_node, 100, &insert_node))
    //         return EXIT_FAILURE;
    // }

    END;

    if (StdListDtor(&std_list_data))
    {
        return EXIT_FAILURE;
    }

    clock_t std_end_time = clock();

#endif
#ifdef AOS_LIST

    clock_t aos_start_time = clock();

    List_t list = {};

    BEGIN

    int insert_pos = 0;

    if (ListCtor(&list, 16, 1))
        break;

    if (ListInsertAfter(&list, 0, 25, &insert_pos))
        break;

    if (ListInsertAfter(&list, insert_pos, 25, &insert_pos))
        break;

    if (ListInsertAfter(&list, 0, 20, &insert_pos))
        break;

    if (ListInsertBefore(&list, 0, 35, &insert_pos))
        break;


    // list.data[2].prev = 3;
    list.data[3].prev = 84;

#ifdef LIST_DEBUG
    ListCheck(&list, "PREV = ", "main", "main.cpp", 57, 84);
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

    // for (int i = 0; i < 10000; i++)
    // {
    //     if (ListInsertBefore(&list, insert_pos, 100, &insert_pos))
    //         return EXIT_FAILURE;
    // }

    END;

    if (ListDtor(&list))
    {
        return EXIT_FAILURE;
    }

    clock_t aos_end_time = clock();

#endif

#ifdef STD_LIST
    double std_run_time = (double) (std_end_time - std_start_time) / CLOCKS_PER_SEC;
    printf("Standard list run time       = %lg;\n", std_run_time);
#endif
#ifdef AOS_LIST
    double aos_run_time = (double) (aos_end_time - aos_start_time) / CLOCKS_PER_SEC;
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
