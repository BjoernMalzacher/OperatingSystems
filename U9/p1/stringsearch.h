#ifndef STRINGSEARCH_H
#define STRINGSEARCH_H

#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

ssize_t findFirstSubstring(const char *needle, const char *haystack, size_t len);

typedef struct _Line {
    const char *start;
    size_t len;
} Line;

typedef struct _Lines {
    size_t len, cap;
    Line lines[];
} Lines;

#define LINES_DEFAULT_CAP 16

Lines *findLines(const char *haystack, size_t len);
Line *findLineContaining(Lines *l, const char *match);

typedef struct _NeedleSearcherState {
    const char *needle;
    const char *haystack;
    size_t len;

    pthread_cond_t *cond;
    pthread_mutex_t *mutex;
    bool *lines_valid;
    Lines **lines;
} NeedleSearcherState;

typedef struct _NeedleSearcherResult {
    const char *match;
    const Line *line;
    struct _NeedleSearcherResult *next;
} NeedleSearcherResult;

void *needleSearcher(void *arg);

void parallelStringSearch(const char *needle, const char *haystack, size_t hay_len, int threads);

#endif
