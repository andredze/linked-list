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
    LIST_CALL_DUMP(&list, "ctor", "TEST_AFTER_CTOR");

    if (ListPushFront   (&list, 100, &insert_pos))
        break;
    LIST_CALL_DUMP(&list, "front",  "TEST_PUSH_FRONT_1");

    if (ListInsertAfter (&list, 0, 20, &insert_pos))
        break;
    LIST_CALL_DUMP(&list, "insert", "TEST_INSERT_AFTER_1");

    if (ListInsertAfter (&list, 2, 30, &insert_pos))
        break;
    LIST_CALL_DUMP(&list, "insert", "TEST_INSERT_AFTER_2");

    if (ListInsertAfter (&list, 1, 25, &insert_pos))
        break;
    LIST_CALL_DUMP(&list, "insert", "TEST_INSERT_AFTER_3");

    if (ListErase       (&list, 2))
        break;
    LIST_CALL_DUMP(&list, "erase", "TEST_ERASE_1");

    if (ListErase       (&list, 1))
        break;
    LIST_CALL_DUMP(&list, "erase", "TEST_ERASE_2");

    if (ListErase       (&list, 3))
        break;
    LIST_CALL_DUMP(&list, "erase", "TEST_ERASE_3");

    if (ListErase       (&list, 4))
        break;
    LIST_CALL_DUMP(&list, "erase", "TEST_ERASE_4");

    if (ListPushFront   (&list, 666, &insert_pos))
        break;
    LIST_CALL_DUMP(&list, "front",  "TEST_PUSH_FRONT_2");

    if (ListPushBack    (&list, 6471, &insert_pos))
        break;
    LIST_CALL_DUMP(&list, "back",   "TEST_PUSH_BACK_1");

    if (ListInsertBefore(&list, 0, 22, &insert_pos))
        break;
    LIST_CALL_DUMP(&list, "insert", "TEST_INSERT_BEFORE_4");

    END;

    if (ListDtor(&list))
    {
        return EXIT_FAILURE;
    }

    DPRINTF(GREEN "Tests ran successfully\n\n" RESET_CLR);

    return EXIT_SUCCESS;
}
