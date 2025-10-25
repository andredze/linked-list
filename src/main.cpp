#include "list.h"

//------------------------------------------------------------------------------------------

int main()
{
    ListCtx_t list_ctx = {};

    if (ListCtor(&list_ctx, -64))
    {
        return EXIT_FAILURE;
    }

    if (ListDtor(&list_ctx))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
