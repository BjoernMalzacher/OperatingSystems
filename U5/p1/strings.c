#include "strings.h"

/**
 * Returns the length of string s without counting the terminating null byte.
 */
size_t stringlength(const char *s)
{
    (void) s;
    
    // TODO: Add code here.
    return 0;   
}

/**
 * Returns a new null byte terminated string that is the result of appending string s2 to s1.
 * The caller is responsible to free the returned string.
 * Returns NULL on any error.
 */
char *stringconcat(const char *s1, const char *s2)
{
    (void) s1;
    (void) s2;
   
    // TODO: Add code here.
    return NULL;   
}

/**
 * Splits string toSplit at every occurrence of the delimiter.
 * Returns a null-terminated array with pointers to the null-terminated
 * substrings. The caller should free the result by calling stringsplit_free().
 * Returns NULL on any error.
 */
char **stringsplit(const char *toSplit, char delimiter)
{
    (void) toSplit;
    (void) delimiter;

    // TODO: Add code here.
    return NULL;
}

/**
 * Frees the result from a stringsplit() call.
 */
void stringsplit_free(char **parts)
{
    (void) parts;
    // TODO: Free whatever memory you allocated in stringsplit().
}
