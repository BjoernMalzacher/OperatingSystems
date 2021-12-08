#ifndef MEMSET_H
#define MEMSET_H
#include <stdint.h>
#include <stddef.h>

uint64_t rotr(uint64_t v);
void pattern_memset_byte(void *s, uint64_t *pat, size_t n);
void pattern_memset(void *s, uint64_t pat, size_t n);
#endif
