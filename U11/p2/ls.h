#ifndef LS_H
#define LS_H

#ifndef ALLOW_OVERRIDE
#define ALLOW_OVERRIDE __attribute__((weak))
#endif

int list(const char* path, const char *filterByExtension);


/* Necessary for test_printLine */
ALLOW_OVERRIDE
void _printLine(unsigned int size, unsigned int sizeOnDisk, const char* name);

#endif

