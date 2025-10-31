#include "common.h"
#include "listCommands.h"
#include "listDebug.h"

//------------------------------------------------------------------------------------------

int main()
{
    DPRINTF(BLUE "\nRunning tests\n\n" RESET_CLR);

    List_t list = {};

    BEGIN

    int insert_pos = 0;

    if (ListCtor        (&list, 4))
        break;

    // if (ListCtor        (&list, 0))
    //     break;

    if (ListInsertAfter (&list, 0, 10, &insert_pos))
        break;

    if (ListInsertAfter (&list, 1, 20,  &insert_pos))
        break;

    // if (ListInsertBefore (&list, 2, 30, &insert_pos))
    //     break;

    if (ListInsertAfter (&list, 2, 30, &insert_pos))
        break;

    if (ListInsertBefore(&list, 3, 40, &insert_pos))
        break;

    if (ListInsertAfter (&list, 4, 50, &insert_pos))
        break;

    if (ListInsertBefore(&list, 5, 45, &insert_pos))
        break;

    // list.data[7].next = 4;

    if (ListInsertAfter (&list, 4, 50, &insert_pos))
        break;

    if (ListInsertAfter (&list, 1, 50, &insert_pos))
        break;

    // list.data[3].prev = 84;
    // list.size = 2;
    list.data[3].prev = 4;

    if (ListInsertAfter (&list, 8, 35, &insert_pos))
        break;

    if (ListInsertBefore(&list, 3, 45, &insert_pos))
        break;

    if (ListInsertBefore(&list, 4, 45, &insert_pos))
        break;

    if (ListInsertBefore(&list, 4, 45, &insert_pos))
        break;

    if (ListErase       (&list, 2))
        break;

    if (ListErase       (&list, 1))
        break;

    if (ListErase       (&list, 3))
        break;

    if (ListErase       (&list, 4))
        break;

    if (ListInsertAfter (&list, 0, 666, &insert_pos))
        break;

    if (ListInsertBefore(&list, 0, 6471, &insert_pos))
        break;

    if (ListInsertBefore(&list, 0, 22, &insert_pos))
        break;

    END;

    if (ListDtor(&list))
    {
        return EXIT_FAILURE;
    }

    DPRINTF(GREEN "Tests ran successfully\n\n" RESET_CLR);

    return EXIT_SUCCESS;
}
