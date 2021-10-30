#ifndef BITS_H
#define BITS_H

#include <stddef.h>
#include <stdint.h>

int getN(uint64_t *A, size_t n);
void setN(uint64_t *A, size_t n);
void clrN(uint64_t *A, size_t n);

uint64_t rot(uint64_t i, int n);

#endif
