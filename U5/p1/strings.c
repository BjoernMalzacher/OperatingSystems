#include "strings.h"

/**
 * Returns the length of string s without counting the terminating null byte.
 */
size_t stringlength(const char *s)
{
    size_t count = 0 ;
    while (*s != '\0') {
        count +=1;
        s +=1;
    }   
    return count;   
}

/**
 * Returns a new null byte terminated string that is the result of appending string s2 to s1.
 * The caller is responsible to free the returned string.
 * Returns NULL on any error.
 */
char *stringconcat(const char *s1, const char *s2) {
    char* concat = malloc(stringlength(s1)+stringlength(s2));
    char* startpoint= concat;
    while (*s1 != '\0') {
        *concat = *s1;
        concat+=1; 
        s1+=1;
    }
    while (*s2 !='\0') {
        *concat = *s2;
        concat +=1;
        s2+=1;
    }
    return startpoint;   
}

/**
 * Splits string toSplit at every occurrence of the delimiter.
 * Returns a null-terminated array with pointers to the null-terminated
 * substrings. The caller should free the result by calling stringsplit_free().
 * Returns NULL on any error.
 */
char **stringsplit(const char *toSplit, char delimiter)
{
    char** splittedArrays = malloc(stringlength(toSplit));
    char *array;

    int count = 0;
    while (*toSplit !='\0')
    {
        if(*toSplit == delimiter){
            *splittedArrays = prefix;
            *splittedArrays +=1;
            free(array); 
            char *newArray;
            array = newArray;
        }
        *array = *toSplit;
        array +=1; 
        toSplit+=1;
    }// doesnt work;
     
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
