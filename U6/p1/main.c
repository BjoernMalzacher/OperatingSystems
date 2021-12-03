#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sorting.h"
#include "testlib.h"

#define LENGTH 6

/* Helper for testing: prints an array to a newly-allocated string. */
static char *asprint_array(int *a, size_t len)
{
    char *out = NULL;
    size_t size = 1, printed = 0;

    /* We need to format the whole array twice, first to determine the length
     * of the output string, then to actually print the data. We can avoid
     * copying code by using macros. */
#define COUNTING_PRINT(...) \
    do { \
        int n = snprintf(OUT, SIZE, __VA_ARGS__); \
        if (n < 0) { free(out); return NULL; } \
        COUNT += (size_t) n; \
    } while (0)

#define SPRINT_ARRAY \
    COUNTING_PRINT("{"); \
    for (size_t i = 0; i < len; i++) \
        COUNTING_PRINT(i == len-1 ? "%d}" : "%d, ", a[i]);

    /* These are substituted in COUNTING_PRINT above. */
#define SIZE 0
#define OUT NULL
#define COUNT size
    SPRINT_ARRAY;
#undef SIZE
#undef OUT
#undef COUNT

    out = malloc(size);
    if (out == NULL) return NULL;

#define SIZE (size - printed)
#define OUT (out + printed)
#define COUNT printed
    SPRINT_ARRAY;
#undef SIZE
#undef OUT
#undef COUNT

    return out;
#undef SPRINT_ARRAY
#undef COUNTING_PRINT
}

static void test_array_sorted(int *supposedlySorted, size_t length, const char *algo)
{
    bool sorted = true;
    for (size_t i = 0; i < length - 1; ++i) {
        if (supposedlySorted[i] > supposedlySorted[i + 1]) {
            sorted = false;
            break;
        }
    }
    char *after = asprint_array(supposedlySorted, length);
    size_t size = strlen(after) + strlen(algo) + 24;
    char *msg = malloc(size);
    snprintf(msg, size, "%s(array) = %s is sorted", algo, after);
    test_assert(sorted, msg);
    free(msg); free(after);
}

int main()
{
    test_start("sorting.c");

    int insertionSortArray[LENGTH] = {3, 5, 2, 1, 7, 4};
    insertionSort(insertionSortArray, LENGTH);
    test_array_sorted(insertionSortArray, LENGTH, "insertionSort");

    int mergeSortArray[LENGTH] = {3, 5, 2, 1, 7, 4};
    mergeSort(mergeSortArray, LENGTH);
    test_array_sorted(mergeSortArray, LENGTH, "mergeSort");

    int pancakeSortArray[LENGTH] = {3, 5, 2, 1, 7, 4};
    pancakeSort(pancakeSortArray, LENGTH);
    test_array_sorted(pancakeSortArray, LENGTH, "pancakeSort");

    return test_end();
}
