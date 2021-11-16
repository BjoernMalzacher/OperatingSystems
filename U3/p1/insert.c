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
	if(*length>=*capacity) { 

		int *newArray = (int *)malloc((*capacity+10));
		memmove(newArray,p,sizeof(*p));
		

	*capacity+=10;
	p=newArray;
	
	
	}
	
	for (size_t i = 0; i <*length; i++)
	{
		if(*(p)> z){
			memmove(p+1,p,*length-i);
			*(p) = z;
			*length+=1;
			break;
		}
		p++;
	}
	**array = *p;
	

	
	
		
	

	(void) array; (void) length; (void) capacity; (void) z;
}
