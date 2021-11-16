#include "testlib.h"
#include "linkedlist.h"


int main()
{
    test_start("linkedlist.c");

    entry *first = allocateEntry(1);
    test_equals_int(first->value, 1, "allocating entry for 1 works");
    freeEntry(first);

    linkedlist list = {NULL};

    insertValue(&list, 1);
    test_equals_int(findFirstEntryWithValue(&list, 1)->value, 1, "finding entry with 1 works");
    test_equals_ptr((void*)findFirstEntryWithValue(&list, 0), (void*)NULL, "list does not contain an entry with value 0");

    insertValue(&list, 2);
    removeFirstEntryWithValue(&list, 1);
    test_equals_ptr((void*)findFirstEntryWithValue(&list, 1), (void*)NULL, "list does not contain an entry with value 1 anymore");

    test_equals_int(findFirstEntryWithValue(&list, 2)->value, 2, "finding entry with 2 works");
    removeFirstEntryWithValue(&list, 2);

    return test_end();
}
