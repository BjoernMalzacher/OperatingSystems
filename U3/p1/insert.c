#include "insert.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/*
 * Reminder: insert() function from the tutorial that does not manage capacity.
 */
void insert_tut(int *a, size_t *length, int z)
{
	int *p = a;
	int *end = a + *length;
	while ((p < end) && (*p < z)) { ++p; }
	memmove(p + 1, p, (end - p) * sizeof(int));
	*p = z;
	(*length)++;
}

/*
 * New insert() function that reallocates if the array is full.
 */
void insert(int **array, size_t *length, size_t *capacity, int z)
{
	int *p = *array;
	
	
	
	int extracap = 0;
	if(length+1 >=capacity){ extracap +=10;}
	
	int *newArray = (int *)malloc(sizeof(*p)+extracap);
	memmove(p,array,sizeof(*p));
	int num = 0;
	while ((array)[num++] != NULL);
	while ((p< num) || (*p<z))
	{
		memmove(p+1, p, ((p+num) - p) * sizeof(int));
	}
	
	
	

	printf("%li %li\n ", sizeof(*newArray), sizeof(*p));
	
	

	(void) array; (void) length; (void) capacity; (void) z;
}
