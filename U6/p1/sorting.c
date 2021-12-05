#include "sorting.h"
#include "string.h"

/**
 * Swap both Integers the pointers are pointing to with each other.
 */
void swap(int *pointer1, int *pointer2)
{
 int* save = pointer1;
 pointer1 = pointer2;
 pointer1 = save;

    // TODO
}

/**
 * Sorts the array toSort with given length ascending using insertion sort.
 */
void insertionSort(int *toSort, size_t length)
{
    int currentvalue;
    int j;
    for (size_t i = 0; i < length; i++) {
        currentvalue = *(toSort+i);
        j =i;
        while (j>0 && *(toSort+(j-1)) > currentvalue){
            *(toSort+j) = *(toSort+j-1);
            j=j-1;
        }
        *(toSort+j) = currentvalue;
    }
}

/**
 * Creates a new array and copies length elements from src array into the new array.
 * The caller needs to free the new returned array.
 */
int *copy(int *src, size_t length) {
    int* newArray = malloc(sizeof(int)*length);
    memmove(newArray, src,length*sizeof(int));  
    return newArray;
}

/**
  * Merge src array which contains two sorted sub array src[start...middle - 1] and src[middle...end-1].
  * The sub arrays are merged into destToMergeInto with the result sorted as well.
  */
void merge(int *src, size_t start, size_t middle, size_t end, int *destToMergeInto) {
    size_t i = 0;
    size_t j = middle;
    size_t n = 0;
    while (i<middle && j<end) {  
        if(*(src+i)<=*(src+j)) {
            *(destToMergeInto+n) = *(src+i);
            i++;
        }else {
            *(destToMergeInto+n) = *(src+j);
            j++;
        }
        n++;
    }
    while (i < middle ) {
        *(destToMergeInto+n)= *(src+i);
        i++;
        n++;
    }
    while (j < end) {
        *(destToMergeInto+n)= *(src+j);
        j++;
        n++;
    } 
 
    (void) start; //mit start lässt sich das programm nicht wirklich umsetzten. Es beginnt  der ersten Iteration bei 1 und der zweiten bei 0. 
                  //  Dadurch werden werte bei merge aus dem Array entfernt die nicht entfernt werden dürfen. 
                  
 
  
    
         
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
    size_t start = 0;
    int temp = 0;
    numberOfPancakesToFlip-=1;
    while (start < numberOfPancakesToFlip) {
        temp = *(stackOfPancakes+start);
        *(stackOfPancakes+start) = *(stackOfPancakes+numberOfPancakesToFlip);
        *(stackOfPancakes+numberOfPancakesToFlip)= temp;
        start+=1;
        numberOfPancakesToFlip-=1;
    }
}

/**
 * Finds the index of the largest number in array with the given length.
 */
size_t findIndexOfMax(int *array, size_t length)
{
    size_t index = 0;
    for (size_t i = 0; i < length; i++)
    {
        if(*(array+index)< *(array+i)){
            index = i;
        }
    }

    return index;
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
