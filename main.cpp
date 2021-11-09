#include "list.h"
#include <stdio.h>

#include "debug_lib.h"

void test1();

int main()
{
    test1();

    return 0;
}

void test1()
{
    StatusCode error = LIST_IS_OK;
    size_t     index = 0;
    Val_t      value = DEAD_VALUE;

    List_t list = {};

    if (error = ListCtor(&list, 4))
        printf("error = %lu\n", error);

    if (error = ListVerify(&list))
        printf("error = %lu\n", error);

    ListInsertAfter(&list, 1, 15);

    for (int i = 1; i < 12; ++i)
        ListInsertAfter(&list, i, 1000 - 7 * i);

    for (int i = 0; i < 4; ++i)
    {
        ListPopFront(&list);
    }

    for (int i = 0; i < 5; ++i)
    {
        value = ListPopFront(&list);
    }

    for (int i = 0; i < 3; ++i)
    {
        value = ListPopFront(&list);
        PRINT_D(value);
    }

    ListDump(&list);

    ListLinearize(&list);

    ListDump(&list);

    for (int i = 1; i < 5; i++)
    {
        ListPushBack(&list, 1000 + 7 * i);
        // ListDump(&list);
    }

    ListLinearize(&list);
    // ListDump(&list);

    for (int i = 4; i < 7; i++)
    {
        ListPushBack(&list, 1000 + 7 * i);
        // ListDump(&list);
    }

    ListLinearize(&list);
    // ListDump(&list);

    ListDtor(&list);
}
