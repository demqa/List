#include "list.h"
#include <stdio.h>

int main()
{
    List_t list = {};

    StatusCode error = LIST_IS_OK;
    size_t     index = 0;

    if (error = ListCtor(&list, 7))
        printf("error = %lu\n", error);

    // ListDump(&list);

    error = ListInsertAfter(&list, 1, 15);
    printf("error = %lu\n", error);
    // ListDump(&list);

    // Val_t x = ListRemove(&list, 1);
    // printf("x = %d\n", x);
    // ListDump(&list);

    error = ListInsertAfter(&list, 1, 45);
    printf("error = %lu\n", error);
    // ListDump(&list);

    error = ListInsertAfter(&list, 2, 60);
    printf("error = %lu\n", error);
    // ListDump(&list);

    error = ListInsertAfter(&list, 3, 75);
    printf("error = %lu\n", error);
    ListDump(&list);

    ListPopFront(&list);
    ListPopFront(&list);

    ListDump(&list);

    ListPushFront(&list, 123);
    ListPushFront(&list, 987);

    ListDump(&list);

    index = ListReturnPhysIndex(&list, 3);
    printf("index = %lu\n", index);

    ListPushFront(&list, 1337);

    ListDump(&list);

    index = ListReturnPhysIndex(&list, 5);
    printf("index = %lu\n", index);

    // x = ListRemove(&list, 1);
    // printf("x = %d\n", x);
    // ListDump(&list);

    if (error = ListVerify(&list))
        printf("error = %lu\n", error);

    ListDtor(&list);

    return 0;
}