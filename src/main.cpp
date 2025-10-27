#include "list.h"

//------------------------------------------------------------------------------------------

int main()
{
    ListCtx_t list_ctx = {};

    BEGIN

    if (ListCtor(&list_ctx, 64))
    {
        break;
    }

    END;

    DEBUG_LIST_CHECK(&list_ctx, "DEBUG DUMP FROM MAIN");

#ifdef LIST_DEBUG
    ListDumpInfo_t dump_info = {LIST_SUCCESS, "graph.png", "TEST_CALL1",
                                __PRETTY_FUNCTION__, __FILE__, __LINE__};
    SAFE_CALL(ListDump(&list_ctx, &dump_info));

    ListDumpInfo_t dump_info2 = {LIST_SUCCESS, "graph.png", "TEST_CALL2",
                                 __PRETTY_FUNCTION__, __FILE__, __LINE__};
    SAFE_CALL(ListDump(&list_ctx, &dump_info2));
#endif

    if (ListDtor(&list_ctx))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
