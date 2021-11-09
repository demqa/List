#include "list.h"
#include <stdio.h>

int main()
{
    List_t list = {};

    StatusCode error = LIST_IS_OK;

    if (error = ListCtor(&list, 7))
        printf("error = %lu\n", error);

    ListDump(&list);

    error = ListInsertAfter(&list, 1, 15);
    printf("error = %lu\n", error);

    error = ListInsertAfter(&list, 1, 30);
    printf("error = %lu\n", error);

    error = ListInsertAfter(&list, 2, 45);
    printf("error = %lu\n", error);

    // ListDump(&list);

    error = ListInsertAfter(&list, 2, 60);
    printf("error = %lu\n", error);

    ListDump(&list);

    if (error = ListVerify(&list))
        printf("error = %lu\n", error);

    ListDtor(&list);

    return 0;
}