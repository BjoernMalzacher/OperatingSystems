#ifndef FILESEARCH_H
#define FILESEARCH_H

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>

FILE *openAtOffset(const char *filename, long offset);
char *allocateAndRead(FILE *file, size_t len);
int64_t getFileSize(const char *filename);

int parallelFileSearch(const char *needle, const char *filename, int threads);

#endif
