#include "linkedlist.h"
#include <stdlib.h>
/*
 * Creates a new entry with the given value.
 */
entry* allocateEntry(int value)
{
    entry* p = (entry *)malloc(sizeof(entry));
    p->value =value;
    return p;
}

/*
 * Release memory for an entry that is not used anymore.
 */
void freeEntry(entry *entry)
{
    free(entry);
}

/*
 * Insert value at the front of the list.
 */
void insertValue(linkedlist *list, int value)
{
    entry *current_value = list->head;
    entry *newValue = allocateEntry(value);
    newValue->prev =current_value->prev;
    list->head = newValue;
    newValue->next = current_value;
    
    
    
    
    (void) list;
    (void) value;

    // TODO: Add code here.
}

/*
 * Find the first entry containing the value in the list.
 */
entry* findFirstEntryWithValue(linkedlist *list, int value)
{
    (void) list;
    (void) value;

    // TODO: Add code here.
    return NULL;
}

/*
 * Remove the first entry containing the value from the list.
 */
void removeFirstEntryWithValue(linkedlist *list, int value)
{
    
    entry *currentValue = list->head;
    while (currentValue->next != NULL) {
        if (currentValue->value == value) {        
            break;
        }
        currentValue = currentValue->next;
        
    }
    
    currentValue->prev->next = currentValue->next;
    currentValue->next->prev = currentValue->prev;
    free(currentValue);    
}
