#include "list.h"
#include <stdio.h>

#include "debug_lib.h"

int main()
{
    List_t list = {};

    StatusCode error = LIST_IS_OK;
    size_t     index = 0;
    Val_t      value = DEAD_VALUE;

    if (error = ListCtor(&list, 4))
        printf("error = %lu\n", error);
    // ListDump(&list);

    error = ListInsertAfter(&list, 1, 15);
    printf("error = %lu\n", error);
    // ListDump(&list);

    for (int i = 1; i < 12; ++i)
        ListInsertAfter(&list, i, 1000 - 7 * i);

    // ListDump(&list);

    for (int i = 0; i < 4; ++i)
    {
        ListPopFront(&list);
    }

    // ListLinearize(&list);
    // ListDump(&list);

    for (int i = 0; i < 5; ++i)
    {
        value = ListPopFront(&list);
        // ListDump(&list);
        // PRINT_D(value);
    }

    // ListDump(&list);

    for (int i = 0; i < 3; ++i)
    {
        value = ListPopFront(&list);
        // ListDump(&list);
        PRINT_D(value);
    }

    ListDump(&list);

    for (int i = 1; i < 5; i++)
    {
        ListPushBack(&list, 1000 + 7 * i);
        ListDump(&list);
    }

    ListLinearize(&list);
    ListDump(&list);

    for (int i = 4; i < 7; i++)
    {
        ListPushBack(&list, 1000 + 7 * i);
        ListDump(&list);
    }

    ListLinearize(&list);
    ListDump(&list);

    // Val_t x = ListRemove(&list, 1);
    // printf("x = %d\n", x);
    // ListDump(&list);

    // ListPopFront(&list);
    // ListPopFront(&list);

    // x = ListRemove(&list, 1);
    // printf("x = %d\n", x);
    // ListDump(&list);

    if (error = ListVerify(&list))
        printf("error = %lu\n", error);

    ListDtor(&list);

    return 0;
}