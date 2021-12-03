#include "sorting.h"

/**
 * Swap both Integers the pointers are pointing to with each other.
 */
void swap(int *pointer1, int *pointer2)
{
    (void) pointer1;
    (void) pointer2;

    // TODO
}

/**
 * Sorts the array toSort with given length ascending using insertion sort.
 */
void insertionSort(int *toSort, size_t length)
{
    (void) toSort;
    (void) length;

    // TODO
}

/**
 * Creates a new array and copies length elements from src array into the new array.
 * The caller needs to free the new returned array.
 */
int *copy(int *src, size_t length)
{
    (void) src;
    (void) length;

    // TODO
    return NULL;
}

/**
  * Merge src array which contains two sorted sub array src[start...middle - 1] and src[middle...end-1].
  * The sub arrays are merged into destToMergeInto with the result sorted as well.
  */
void merge(int *src, size_t start, size_t middle, size_t end, int *destToMergeInto)
{
    (void) src;
    (void) start;
    (void) middle;
    (void) end;
    (void) destToMergeInto;

    // TODO
}

/**
 * Sorts the array srcToSort recursively with merge sort. 
 * Start and end indicate the current window of srcToSort array this call sorts.
 * The result will be in sortedDest.
 */
void mergeSortRec(int *srcToSort, size_t start, size_t end, int *sortedDest)
{
    if (end - start > 1) {
        size_t middle = (end + start) / 2;
        mergeSortRec(sortedDest, start, middle, srcToSort);
        mergeSortRec(sortedDest, middle, end, srcToSort);
        merge(srcToSort, start, middle, end, sortedDest);
    }
}

/**
 * Sorts the array toSort with given length ascending using merge sort.
 */
void mergeSort(int *toSort, size_t length)
{
    int *copyToSort = copy(toSort, length);
    mergeSortRec(copyToSort, 0, length, toSort);
    free(copyToSort);
}

/**
 * Rotates the array stackOfPancakes[0...numberOfPancakesToFlip-1].
 */
void flip(int *stackOfPancakes, size_t numberOfPancakesToFlip)
{
    (void) stackOfPancakes;
    (void) numberOfPancakesToFlip;
}

/**
 * Finds the index of the largest number in array with the given length.
 */
size_t findIndexOfMax(int *array, size_t length)
{
    (void) array;
    (void) length;

    // TODO

    return 42;
}

/**
 * Sorts the array toSort with given length ascending using pancake sort.
 */
void pancakeSort(int *toSort, size_t length)
{
    size_t maxIndex = 0;
    for (size_t i = length; i > 1; --i) {
        maxIndex = findIndexOfMax(toSort, i);
        if (maxIndex != i) {
            flip(toSort, maxIndex + 1);
            flip(toSort, i);
        }
    }
}
