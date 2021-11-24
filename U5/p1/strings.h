#ifndef STRINGS_H
#define STRINGS_H

#include <stdio.h>
#include <stdlib.h>



/**
 * Returns the length of string s without counting the terminating null byte.
 */
size_t stringlength(const char *s);

/**
 * Returns a new null byte terminated string that is the result of appending string s2 to s1.
 * The caller is responsible to free the returned string.
 * Returns NULL on any error.
 */
char *stringconcat(const char *s1, const char *s2);

/**
 * Splits string toSplit at every occurrence of the delimiter.
 * Returns an array with pointers to the null byte terminated substrings.
 * The substrings exclude the delimiter.
 * The original string toSplit is not modified.
 * Iterating over the array of substrings and putting the delimiter in between two substrings results in the original string toSplit.
 * The caller is responsible to free the returned two dimensional array.
 * Returns NULL on any error.
 */
char **stringsplit(const char *toSplit, char delimiter);

/**
 * Frees the result from a stringsplit() call.
 */
void stringsplit_free(char **parts);



#endif
