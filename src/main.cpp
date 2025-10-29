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

    if (ListInsertAfter (&list, 0, 100, &insert_pos))
        break;

    if (ListInsertAfter (&list, 0, 20,  &insert_pos))
        break;

    if (ListInsertAfter (&list, insert_pos, 30, &insert_pos))
        break;

    if (ListInsertAfter (&list, 1, 25, &insert_pos))
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
