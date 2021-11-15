#include <stdint.h>

#include "bits.h"

/*
 * Returns the n'th bit of the array A.
 */
int getN(uint64_t *A, size_t n)
{
    uint64_t p = 1;
 
    return  (*A >> n) & p;
}

/*
 * Sets the n'th bit of the array A.
 */
void setN(uint64_t *A, size_t n)
{
    uint64_t p = 1;
    *A |= p << n;
}

/*
 * Clears the n'th bit of the array A.
 */
void clrN(uint64_t *A, size_t n)
{
   uint64_t p = 1;
   *A &= ~(p << n);
    
}


/*
 * Rotates the integer i n bits to the right.
 */
uint64_t rot(uint64_t i, int n)
{
         if(n >=0){
             
            return (i << 64-n) |(i >> n);
         }else {
             n *=-1;
            return (i >> 64-n) |(i << n);
         }

    return 0;
}
