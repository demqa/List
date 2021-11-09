#include "list.h"
#include "debug_lib.h"

StatusCode ListCtor(List_t *list, size_t capacity)
{
    if (list == nullptr)
        return LIST_PTR_IS_NULL;

    if (capacity == 0)
        return LIST_CTY_CANT_BE_ZERO;

    if (ListIsEmpty(list)      != LIST_IS_EMPTY && 
        ListIsDestructed(list) != LIST_IS_DESTRUCTED)
        return LIST_ISNT_EMPTY;

    list->data = (Elem_t__ *) calloc(capacity + 1, sizeof(Elem_t__));
    if (list->data == nullptr)
        return LIST_BAD_ALLOC;
    
    list->capacity = capacity;
    list->front = 1;
    list->back  = 1;
    list->free  = 1;

    for (size_t index = 1; index < capacity; ++index)
    {
        list->data[index].next  = index + 1;
        list->data[index].prev  = FREE_INDEX;
        list->data[index].value = DEAD_VALUE;
    }

    list->data[capacity].next  = 0;
    list->data[capacity].prev  = FREE_INDEX;
    list->data[capacity].value = DEAD_VALUE;

    return LIST_IS_OK;
}

StatusCode ListDtor(List_t *list)
{
    if (list == nullptr)
        return LIST_PTR_IS_NULL;

    if (ListIsDestructed(list) == LIST_IS_DESTRUCTED)
        return LIST_IS_DESTRUCTED;

    if (ListVerify(list) == LIST_IS_OK)
        for (size_t index = 0; index < list->capacity + 1; ++index)
        {
            list->data[index].next  = 0xDED32DED;
            list->data[index].prev  = 0xDED32DED;
            list->data[index].value = DEAD_VALUE;
        }

    free(list->data);

    list->data = (Elem_t__ *) (1000 - 7);

    list->back     = 0xEBA1;
    list->capacity = 0xDEAD;

    list->free     = 0xFAAC;
    list->front    = 0xF1FA;

    list->sorted   = 0x77;
    list->error    = (StatusCode) 0xD70D;

    return LIST_IS_DESTRUCTED;
}

StatusCode ListIsDestructed(List_t *list)
{
    if (list == nullptr)
        return LIST_PTR_IS_NULL;

    if (list->data   == (Elem_t__ *)(1000 - 7)             &&
        list->back   == 0xEBA1 && list->capacity == 0xDEAD &&
        list->free   == 0xFAAC && list->front    == 0xF1FA &&
        list->sorted == 0x77   && list->error    == (StatusCode) 0xD70D)
        return LIST_IS_DESTRUCTED;
    
    return LIST_STATUS_UNKNOWN;
}

StatusCode ListIsEmpty(List_t *list)
{
    if (list == nullptr)
        return LIST_PTR_IS_NULL;
    
    if (list->back == 0 && list->front == 0 && list->sorted   == 0 &&
        list->free == 0 && list->error == 0 && list->capacity == 0 &&
        list->data == nullptr)
        return LIST_IS_EMPTY;
    
    return LIST_STATUS_UNKNOWN;
}

// logical first elem has index 0
size_t     ListReturnPhysIndex(List_t *list, size_t logical_index)
{
    if (ListVerify(list) != LIST_IS_OK)
        return 0;
    
    if (list->sorted)
        return list->front + logical_index;

    size_t counter = 0;
    size_t index = list->front;

    while (counter < logical_index)
    {
        index = list->data[index].next;
        counter++;
    }

    return index;
}       

size_t     ListFront (List_t *list)
{
    if (ListVerify(list) != LIST_IS_OK)
        return 0;
    
    return list->front;
}

size_t     ListBack  (List_t *list)
{
    if (ListVerify(list) != LIST_IS_OK)
        return 0;
    
    return list->back;
}

size_t     ListNext  (List_t *list, size_t physical_index)
{
    if (ListVerify(list) != LIST_IS_OK)
        return 0;
    
    return list->data[physical_index].next;
}

size_t     ListPrev  (List_t *list, size_t physical_index)
{
    if (ListVerify(list) != LIST_IS_OK)
        return 0;
    
    return list->data[physical_index].prev;
}

Elem_t__  *ListResize(List_t *list)
{
    StatusCode status = ListVerify(list);
    if (status != LIST_IS_OK)
        return nullptr;

    return list->data;
}

StatusCode ListInsertAfter (List_t *list, size_t physical_index, Val_t value)
{
    StatusCode status = ListVerify(list);
    if (status != LIST_IS_OK)
        return status;

    if (physical_index != list->back)
        list->sorted = 0;

    if (physical_index < 1 || physical_index > list->capacity + 1 ||
        list->data[physical_index].prev == FREE_INDEX && (list->back != list->front || physical_index != list->back))
        return INVALID_INSERT_INDEX;

    if (list->free == 0)
    {
        Elem_t__ *ptr = ListResize(list);
        if (ptr == nullptr)
        {
            return LIST_BAD_ALLOC;
        }

        list->data = ptr;
    }

    if (list->front == physical_index && list->front == list->back &&
        list->data[list->front].prev == FREE_INDEX)
    {
        assert(list->free == list->back);

        list->free = list->data[list->free].next;

        list->data[list->back].value = value;
        list->data[list->back].next  = 0;
        list->data[list->back].prev  = 0;
    }
    else
    if (physical_index == list->back)
    {
        if (list->free != list->back + 1)
            list->sorted = 0;

        list->data[list->free].prev  = list->back;
        list->data[list->free].value = value;

        size_t index_new = list->free;

        list->data[list->back].next  = index_new;
        list->back = index_new;

        list->free = list->data[index_new].next;
        list->data[index_new].next = 0;
    }
    else
    {
        list->sorted = 0;

        size_t new_free = list->data[list->free].next;

        list->data[list->free].next  = list->data[physical_index].next;
        list->data[list->free].prev  = physical_index;
        list->data[list->free].value = value;

        list->data[list->data[list->free].prev].next = list->free;
        list->data[list->data[list->free].next].prev = list->free;

        list->free = new_free;
    }

    status = ListVerify(list);
    if (status != LIST_IS_OK)
        return status;

    return LIST_IS_OK;
}

StatusCode ListInsertBefore(List_t *list, size_t physical_index, Val_t value)
{
    StatusCode status = ListVerify(list);
    if (status != LIST_IS_OK)
        return status;

    if (physical_index > list->capacity + 1 ||
        physical_index == 0)
        return INVALID_INSERT_INDEX;

    // i can maximize linearization, but i will not
    if (physical_index != list->front ||
        list->front != list->free + 1 || list->free == 0)
        list->sorted = 0;
    
    if (list->front == list->back)
        return ListInsertAfter(list, physical_index, value);

    if (list->free == 0)
    {
        Elem_t__ *ptr = ListResize(list);
        if (ptr == nullptr)
        {
            return LIST_BAD_ALLOC;
        }

        list->data = ptr;
    }

    if (physical_index == list->front)
    {
        size_t new_free = list->data[list->free].next;

        list->data[list->front].prev = list->free;
        
        list->data[list->free].next  = list->front;
        list->data[list->free].prev  = 0;
        list->data[list->free].value = value;

        list->front = list->free;
        list->free  = new_free;
    }
    else
    {
        return ListInsertAfter(list, list->data[physical_index].prev, value);
    }

    status = ListVerify(list);
    if (status != LIST_IS_OK)
        return status;

    return LIST_IS_OK;
}

StatusCode ListPushBack(List_t *list, Val_t value)
{
    StatusCode status = ListVerify(list);
    if (status != LIST_IS_OK)
        return status;

    return ListInsertAfter(list, ListBack(list), value);
}

StatusCode ListPushFront(List_t *list, Val_t value)
{
    StatusCode status = ListVerify(list);
    if (status != LIST_IS_OK)
        return status;

    return ListInsertBefore(list, ListFront(list), value);
}

// Val_t      ListRemove(List_t *list, size_t physical_index)
// {
//     if (ListVerify(list) != LIST_IS_OK)
//         return DEAD_VALUE;
    
//     if (physical_index == 0 || physical_index > list->capacity + 1 ||
//         list->data[physical_index].prev == FREE_INDEX)
//         return DEAD_VALUE;
    
//     if (physical_index == list->front)
//     {
//         if (physical_index == list->back)
//             if (list->data[physical_index].prev == FREE_INDEX)
//                 return DEAD_VALUE;
//             else
//             {
//                 Val_t elem = list->data[physical_index].value;
                
//                 list->data[physical_index].prev  = FREE_INDEX;
//                 list->data[physical_index].next  = list->free;
//                 list->data[physical_index].value = DEAD_VALUE;

//                 list->free = physical_index;

//                 return elem;
//             }
//         else

//     }
//     else
//     if (physical_index == list->back)
//     {
        
//     }
//     else
//     {

//     }
    

// }

StatusCode ListVerify(List_t *list)
{
    if (list == nullptr)
        return LIST_PTR_IS_NULL;

    if (ListIsEmpty(list) == LIST_IS_EMPTY)
        return LIST_IS_EMPTY;
    
    if (ListIsDestructed(list) == LIST_IS_DESTRUCTED)
        return LIST_IS_DESTRUCTED;

    int error = LIST_IS_OK;

    if (list->data == nullptr)
        error |= DATA_PTR_IS_NULL;

    if (list->front > list->capacity)
        error |= (LIST_FRONT_BIGGER_THAN_CTY | LIST_INDEXES_RUINED);

    if (list->back  > list->capacity)
        error |= (LIST_BACK__BIGGER_THAN_CTY | LIST_INDEXES_RUINED);
    
    if (list->sorted > 1)
        error |= LIST_SORTED_RUINED;
    
    if (list->free   > list->capacity)
        error |= (LIST_FREE_RUINED | LIST_INDEXES_RUINED);

    if (list->data[0].next  != 0 ||
        list->data[0].prev  != 0 ||
        list->data[0].value != 0)
        error |= LIST_ZERO_INDEX_VAL_RUINED;

    if (error != LIST_IS_OK)
        return (StatusCode) error;

    if (list->front == list->back)
    {   
        if (!(list->data[list->front].next  == 0 &&
              list->data[list->front].prev  == 0 
              ||
              list->data[list->front].prev  == FREE_INDEX &&
              list->data[list->front].value == DEAD_VALUE))
            error |= LIST_DATA_RUINED;
        
        for (size_t index = 1; index <= list->capacity; ++index)
        {
            if (index != list->front)
            {
                if (list->data[index].value != DEAD_VALUE ||
                    list->data[index].prev  != FREE_INDEX)
                {
                    error |= LIST_DATA_RUINED;
                    break;
                }
            }
        }
    }

    if (error != LIST_IS_OK)
        return (StatusCode) error;

    if (list->front != list->back &&
        list->data[list->front].prev != 0)
        error |= LIST_FRONT_RUINED;
    
    if (list->front != list->back && 
        list->data[list->back].next != 0)
        error |= LIST_BACK_RUINED;

    if (error != LIST_IS_OK)
        return (StatusCode) error;

    for (size_t index = 1; index <= list->capacity; ++index)
    {
        if (list->data[index].next  >  list->capacity
            ||
           (list->data[index].prev  >  list->capacity &&
            list->data[index].prev  != FREE_INDEX)
            ||
           (list->data[index].prev  == FREE_INDEX     && 
            list->data[index].value != DEAD_VALUE))
        {
            error |= LIST_INDEXES_RUINED;
            break;
        }
    }

    if (error != LIST_IS_OK)
        return (StatusCode) error;

    size_t size  = 0;
    size_t place = list->front;
    
    while (list->front != list->back && size < list->capacity)
    {
        if (list->data[place].next == 0)
        {
            if (place != list->back)
                error |= LIST_DATA_RUINED;
            size++;
            break;
        }
        else
        if (list->data[list->data[place].next].prev != place
            ||
            list->data[place].prev != 0 && 
            list->data[list->data[place].prev].next != place)
        {
            error |= LIST_DATA_RUINED;
            break;
        }

        place = list->data[place].next;
        size++;
    }

    if (list->front == list->back && list->data[list->front].prev != FREE_INDEX)
        size++;
    
    place = list->free;
    
    while (size < list->capacity && place != 0)
    {
        if (list->data[place].next == 0)
        {
            if (place != list->capacity               ||
                list->data[place].prev  != FREE_INDEX ||
                list->data[place].value != DEAD_VALUE)
                error |= LIST_DATA_RUINED;
            size++;
            break;
        }
        else
        if (list->data[place].prev  != FREE_INDEX ||
            list->data[place].value != DEAD_VALUE)
        {
            error |= LIST_DATA_RUINED;
            break;
        }
        
        place = list->data[place].next;
        size++;
    }

    if (size != list->capacity)
        error |= LIST_DATA_RUINED;

    return (StatusCode) error;
}

static const char *ColorPicker(List_t *list, size_t index)
{
    const char *colors[] =
    {
        "#E6A8D7", // [0] pink
        "#42AAFF", // [1] cyan
        "#1CD3A2", // [2] green
        "#FF7538", // [3] orange
        "#E34234", // [4] red
        "#5035DE", // [5] (almost) royal blue
        "#EED202", // [6] YELLOW WARNING
    };

    if ((ListVerify(list) & LIST_INDEXES_RUINED) != 0)
        return colors[6];
    else
    if (index <= list->capacity + 1 && list->data[index].prev == FREE_INDEX)
        return colors[1];
    else
    if (index == 0)
        return colors[5];
    else
    if (index == list->back)
        return colors[3];
    else
    if (index == list->front)
        return colors[2];
    else
    if (index > list->capacity + 1)
        return colors[4];
    else
        return colors[0];
}

StatusCode ListDump(List_t *list)
{
    if ((ListVerify(list) & LIST_INDEXES_RUINED) != 0)
    {
        fprintf(stderr, "CANT DUMP. LIST INDEXES ARE RUINED\n");
        return LIST_INDEXES_RUINED;
    }

    FILE *dump_file = fopen("dump", "w");

    fputs("digraph structs {\n", dump_file);

    fputs("    node [color=black, shape=box, style=\"rounded, filled\"];\n", dump_file);
    
    fputs("    rankdir=LR;\n", dump_file);
  
    fprintf(dump_file, "    front [fillcolor=\"%s\", "
                       "    label=\"FRONT = %lu\"];\n",
                       ColorPicker(list, list->front), list->front);

    fprintf(dump_file, "    back  [fillcolor=\"%s\", "
                       "    label=\"BACK = %lu\"];\n",
                       ColorPicker(list, list->back), list->back);

    fprintf(dump_file, "    free  [fillcolor=\"%s\", "
                       "    label=\"FREE = %lu\"];\n",
                       ColorPicker(list, list->free), list->free);

    fputs("    node [color=black, shape=record, style=\"rounded, filled\"];\n", dump_file);
    fputs("\n", dump_file);
    fputs("    edge [style=invis, constraint=true];\n", dump_file);

    for (size_t index = 0; index <= list->capacity; ++index)
    {
        fprintf(dump_file, "    node%lu [fillcolor=\"%s\","
                           "label=\" %lu | { <p> %d | %d | <n> %lu}\"];\n",
                                index, ColorPicker(list, index), index,
                                (list->data[index].prev == FREE_INDEX) ? -1 : list->data[index].prev,
                                list->data[index].value, list->data[index].next);
        
        if (index > 0) fprintf(dump_file, "    node%lu -> node%lu;\n", index - 1, index);
    }

    fputs("\n    edge [style=solid, constraint=false];\n", dump_file);

    for (size_t index = 1; index <= list->capacity; ++index)
    {
        if (list->data[index].next != 0)
            fprintf(dump_file, "    node%lu: <n> -> node%lu;\n", index, list->data[index].next);
        if (list->data[index].prev != FREE_INDEX && list->data[index].prev != 0)
            fprintf(dump_file, "    node%lu: <p> -> node%lu;\n", index, list->data[index].prev);
        fputs("\n", dump_file);
    }

    fputs("\n    edge [style=bold, constraint=false];\n", dump_file);

    fprintf(dump_file, "    front -> node%lu; \n", list->front);
    fprintf(dump_file, "    back  -> node%lu; \n", list->back );
    fprintf(dump_file, "    free  -> node%lu; \n", list->free );

    fputs("}\n", dump_file);

    fclose(dump_file);

    system("dot dump -T png -o dump.png");
    system("eog dump.png");

    return DUMPED;
}
