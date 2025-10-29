#include "common.h"
#include "listCommands.h"
#include "listDebug.h"

//------------------------------------------------------------------------------------------

int main()
{
    DPRINTF(BLUE "\nRunning tests\n\n" RESET_CLR);

    List_t list = {};

    BEGIN

    if (ListCtor        (&list, 4))
        break;
    LIST_CALL_DUMP(&list, "ctor",    "TEST_AFTER_CTOR");

    if (ListPushFront   (&list, 100))
        break;
    LIST_CALL_DUMP(&list, "front1",  "TEST_PUSH_FRONT_1");

    if (ListInsertAfter (&list, 1, 20))
        break;
    LIST_CALL_DUMP(&list, "insert1", "TEST_INSERT_AFTER_1");

    if (ListInsertAfter (&list, 2, 30))
        break;
    LIST_CALL_DUMP(&list, "insert2", "TEST_INSERT_AFTER_2");

    if (ListInsertAfter (&list, 1, 25))
        break;
    LIST_CALL_DUMP(&list, "insert3", "TEST_INSERT_AFTER_3");

    if (ListErase       (&list, 2))
        break;
    LIST_CALL_DUMP(&list, "erase1", "TEST_ERASE_1");

    if (ListErase       (&list, 1))
        break;
    LIST_CALL_DUMP(&list, "erase2", "TEST_ERASE_2");

    if (ListErase       (&list, 3))
        break;
    LIST_CALL_DUMP(&list, "erase3", "TEST_ERASE_3");

    if (ListErase       (&list, 4))
        break;
    LIST_CALL_DUMP(&list, "erase4", "TEST_ERASE_4");

    if (ListPushFront   (&list, 666))
        break;
    LIST_CALL_DUMP(&list, "front2",  "TEST_PUSH_FRONT_2");

    if (ListPushBack    (&list, 6471))
        break;
    LIST_CALL_DUMP(&list, "back1",   "TEST_PUSH_BACK_1");

    if (ListInsertBefore(&list, 4, 22))
        break;
    LIST_CALL_DUMP(&list, "insert4", "TEST_INSERT_BEFORE_4");

    END;

    if (ListDtor(&list))
    {
        return EXIT_FAILURE;
    }

    DPRINTF(GREEN "Tests ran successfully\n\n" RESET_CLR);

    return EXIT_SUCCESS;
}
