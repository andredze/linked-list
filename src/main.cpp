#include "common.h"
#include "listCommands.h"
#include "listDebug.h"

//------------------------------------------------------------------------------------------

int main()
{
    DPRINTF(BLUE "\nRunning tests\n\n" RESET_CLR);

    ListCtx_t list_ctx = {};

    BEGIN

    if (ListCtor        (&list_ctx, 8))     break;
    LIST_CALL_DUMP(&list_ctx, "ctor",    "TEST_AFTER_CTOR");

    if (ListPushFront   (&list_ctx, 100))   break;
    LIST_CALL_DUMP(&list_ctx, "front1",  "TEST_PUSH_FRONT_1");

    // MAIN_CALL(ListPushFront   (&list_ctx, 100));
    // LIST_CALL_DUMP(&list_ctx, "front1",  "TEST_PUSH_FRONT_1");

    if (ListInsertAfter (&list_ctx, 1, 20)) break;
    LIST_CALL_DUMP(&list_ctx, "insert1", "TEST_INSERT_AFTER_1");

    if (ListInsertAfter (&list_ctx, 2, 30)) break;
    LIST_CALL_DUMP(&list_ctx, "insert2", "TEST_INSERT_AFTER_2");

    if (ListInsertAfter (&list_ctx, 1, 25)) break;
    LIST_CALL_DUMP(&list_ctx, "insert3", "TEST_INSERT_AFTER_3");

    if (ListErase       (&list_ctx, 2))     break;
    LIST_CALL_DUMP(&list_ctx, "erase1", "TEST_ERASE_1");

    if (ListErase       (&list_ctx, 1))     break;
    LIST_CALL_DUMP(&list_ctx, "erase2", "TEST_ERASE_2");

    if (ListErase       (&list_ctx, 3))     break;
    LIST_CALL_DUMP(&list_ctx, "erase3", "TEST_ERASE_3");

    if (ListErase       (&list_ctx, 4))     break;
    LIST_CALL_DUMP(&list_ctx, "erase4", "TEST_ERASE_4");

    if (ListPushFront   (&list_ctx, 666))   break;
    LIST_CALL_DUMP(&list_ctx, "front2",  "TEST_PUSH_FRONT_2");

    if (ListPushBack    (&list_ctx, 6471))  break;
    LIST_CALL_DUMP(&list_ctx, "back1",   "TEST_PUSH_BACK_1");

    if (ListInsertBefore(&list_ctx, 4, 22)) break;
    LIST_CALL_DUMP(&list_ctx, "insert4", "TEST_INSERT_BEFORE_4");

    END;

    if (ListDtor(&list_ctx))
    {
        return EXIT_FAILURE;
    }

    DPRINTF(GREEN "Tests ran successfully\n\n" RESET_CLR);

    return EXIT_SUCCESS;
}
