#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <inttypes.h>

typedef struct {
	int valid;
	char *key;
	int value;
} element;

typedef struct {
	element *elements;
	size_t capacity;
} hashtable;

uint64_t hashString(char *c);
int insert(hashtable *table, char *key, int value);
int find(hashtable *table, char *key, int *value);

#endif
