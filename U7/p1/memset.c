#include "memset.h"
/**
 * Rotates a 64 bit integer by one byte (8 bit) to the right.
 */
uint64_t rotr(uint64_t v) {
    (void) v;

    // TODO
    return 0;
}

/**
 * Writes the pattern in *pat byte-by-byte to the memory area pointed to with s and of length n.
 * After the call the pattern in *pat should be rotated so that the next byte to write is at the front of the pattern.
 * As a result calling pattern_memset_byte again with the same pattern will continue the pattern.
 */
void pattern_memset_byte(void *s, uint64_t *pat, size_t n) {
    (void) s;
    (void) pat;
    (void) n;

    // TODO    
}

/**
 * Writes the pattern pat to the memory area starting at s of length n.
 * The function should minimize the number of memory accesses.
 */
void pattern_memset(void *s, uint64_t pat, size_t n) {
    (void) s;
    (void) pat;
    (void) n;

    // TODO
}
