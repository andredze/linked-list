#include "list.h"

//------------------------------------------------------------------------------------------

int main()
{
    ListCtx_t list_ctx = {};

    BEGIN

    MAIN_CALL(ListCtor        (&list_ctx, 2));
    LIST_CALL_DUMP(&list_ctx, "ctor",    "TEST_AFTER_CTOR");

    MAIN_CALL(ListPushFront   (&list_ctx, 100));
    LIST_CALL_DUMP(&list_ctx, "front1",  "TEST_PUSH_FRONT_1");

    MAIN_CALL(ListInsertAfter (&list_ctx, 1, 20));
    LIST_CALL_DUMP(&list_ctx, "insert1", "TEST_INSERT_AFTER_1");

    MAIN_CALL(ListInsertAfter (&list_ctx, 2, 30));
    LIST_CALL_DUMP(&list_ctx, "insert2", "TEST_INSERT_AFTER_2");

    MAIN_CALL(ListInsertAfter (&list_ctx, 1, 25));
    LIST_CALL_DUMP(&list_ctx, "insert3", "TEST_INSERT_AFTER_3");

    MAIN_CALL(ListPushFront   (&list_ctx, 666));
    LIST_CALL_DUMP(&list_ctx, "front2",  "TEST_PUSH_FRONT_2");

    MAIN_CALL(ListPushBack    (&list_ctx, 6471));
    LIST_CALL_DUMP(&list_ctx, "back1",   "TEST_PUSH_BACK_1");

    MAIN_CALL(ListInsertBefore(&list_ctx, 4, 22));
    LIST_CALL_DUMP(&list_ctx, "insert4", "TEST_INSERT_BEFORE_4");

// TODO: dump for free list

    END;

    if (ListDtor(&list_ctx))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
