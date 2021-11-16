#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>

/*
 * Entry in a doubly linked list.
 */
typedef struct _entry {
    /*
     * Pointer to previous entry in list.
     */
    struct _entry *prev;
    /*
     * Pointer to next entry in list.
     */
    struct _entry *next;
    /*
     * Some data stored in this entry.
     */
    int value;
} entry;

/*
 * A linked list is defined by a pointer to its first entry.
 */
typedef struct _linkedlist {
    /*
     * First entry in this list.
     */
    entry *head;
} linkedlist;

entry* allocateEntry(int value);
void freeEntry(entry *entry);
void insertValue(linkedlist *list, int value);
entry* findFirstEntryWithValue(linkedlist *list, int value);
void removeFirstEntryWithValue(linkedlist *list, int value);

#endif
