#include "filesearch.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * Open the file for reading and seek to the given offset. Return NULL if any
 * error occurs.
 */
FILE *openAtOffset(const char *filename, long offset)
{
    // TODO
    (void) filename; (void) offset;
    return NULL;
}

/*
 * Read `len` bytes from `file` into a freshly-allocated buffer. Return NULL if
 * any I/O error occurs or if not `len` bytes could be read.
 */
char *allocateAndRead(FILE *file, size_t len)
{
    // TODO
    (void) file; (void) len;
    return NULL;
}

/*
 * Return the file size of the given file, or -1 if an error occurs.
 */
int64_t getFileSize(const char *filename)
{
    // TODO
    (void) filename;
    return -1;
}

/*
 * Attempt to find needle within the first `len` bytes of haystack, returning
 * the first offset if successful, or -1 otherwise.
 */
ssize_t findFirstSubstring(const char *needle, const char *haystack, size_t len)
{
    size_t i, j;
    size_t needle_len = strlen(needle);
    if (len < needle_len)
        return -1;
    for (i = 0; i <= len - needle_len; i++) {
        for (j = 0; needle[j]; j++) {
            if (needle[j] != haystack[i + j])
                break;
        }
        // We found a match if we walked to the end of needle.
        if (!needle[j])
            return i;
    }
    // no match
    return -1;
}

typedef struct _Line {
    char *start;
    size_t len;
} Line;

typedef struct _Lines {
    size_t len, cap;
    Line lines[];
} Lines;

#define LINES_DEFAULT_CAP 16
/*
 * Allocate a new Lines structure.
 */
Lines *newLines()
{
    Lines *lines = malloc(sizeof(Lines) + LINES_DEFAULT_CAP * sizeof(Line));
    lines->len = 0;
    lines->cap = LINES_DEFAULT_CAP;
    return lines;
}

/*
 * Append a line to the Lines structure, growing it as necessary.
 */
void appendLine(Lines **lines, Line l)
{
    if ((*lines)->len == (*lines)->cap) {
        size_t new_cap = (*lines)->cap * 2;
        *lines = realloc(*lines, sizeof(Lines) + new_cap * sizeof(Line));
        (*lines)->cap = new_cap;
    }
    (*lines)->lines[(*lines)->len++] = l;
}

/*
 * Read `len` bytes of `file` and find all lines (delimited by '\n').
 * Return NULL if any I/O error occurs.
 */
Lines *findLines(FILE *file, size_t len)
{
    char *haystack = allocateAndRead(file, len);
    if (haystack == NULL)
        return NULL;
    Lines *lines = newLines();
    size_t i, start = 0;
    for (i = 0; i < len; i++) {
        if (haystack[i] == '\n') {
            appendLine(&lines, (Line) {.start = haystack + start, .len = i - start + 1});
            start = i + 1;
        }
    }
    if (start < len)
        appendLine(&lines, (Line) {.start = haystack + start, .len = len - start});
    return lines;
}

/*
 * Use binary search to find the line corresponding to the given match.
 */
Line *findLineContaining(Lines *l, size_t offset)
{
    size_t lower = 0, upper = l->len;
    size_t middle, loff;
    char *first = l->lines[0].start;
    while (lower <= upper) {
        middle = (upper + lower) / 2;
        loff = l->lines[middle].start - first;
        if (offset >= loff && offset < loff + l->lines[middle].len) {
            return &l->lines[middle];
        } else if (offset < loff) {
            upper = middle - 1;
        } else {
            lower = middle + 1;
        }
    }
    return NULL;
}

typedef struct _LineSearcherState {
    Lines **lines;
    FILE *file;
    size_t len;
} LineSearcherState;

/*
 * Entry point for the line searching thread.
 */
void *lineSearcher(void *arg)
{
    LineSearcherState *state = arg;
    *state->lines = findLines(state->file, state->len);
    fclose(state->file);
    return NULL;
}

typedef struct _NeedleSearcherState {
    const char *needle;
    FILE *file;
    size_t len, offset;

    bool have_errors;

    pthread_cond_t *cond;
    pthread_mutex_t *mutex;
    bool *lines_valid;
    Lines **lines;
} NeedleSearcherState;

typedef struct _NeedleSearcherResult {
    size_t offset;
    const Line *line;
    struct _NeedleSearcherResult *next;
} NeedleSearcherResult;

/*
 * Entry point for the needle searching threads.
 */
void *needleSearcher(void *arg)
{
    NeedleSearcherState *state = arg;
    NeedleSearcherResult *results = NULL;
    NeedleSearcherResult *results_tail = NULL;

    ssize_t offset;
    size_t len = state->len;
    char *haystack = allocateAndRead(state->file, len);
    if (haystack == NULL) {
        perror("needle searcher failed to read file");
        state->have_errors = true;
        return NULL;
    }
    char *hay_off = haystack;
    size_t needle_len = strlen(state->needle);

    // Find and collect all substrings.
    while ((offset = findFirstSubstring(state->needle, hay_off, len)) != -1) {
        NeedleSearcherResult *r = malloc(sizeof(NeedleSearcherResult));
        r->offset = offset + (hay_off - haystack);
        r->next = NULL;
        // no overlapping matches
        hay_off += offset + needle_len;
        len -= offset + needle_len;
        if (results_tail) {
            results_tail->next = r;
        } else {
            results = r;
        }
        results_tail = r;
    }

    free(haystack);

    // Wait for line information to be ready.
    pthread_mutex_lock(state->mutex);
    while (!*state->lines_valid) {
        pthread_cond_wait(state->cond, state->mutex);
    }
    // Once the lines are valid, there are no more writes to them, so no
    // locking is necessary.
    pthread_mutex_unlock(state->mutex);

    // Add line information to the results.
    for (NeedleSearcherResult *r = results; r; r = r->next) {
        r->line = findLineContaining(*state->lines, state->offset + r->offset);
    }

    return results;
}


/*
 * Perform a parallel search for `needle` in file `filename`, printing all matching lines.
 * Return 0 on success, -1 on any error.
 */
int parallelFileSearch(const char *needle, const char *filename, int threads)
{
    size_t needle_len = strlen(needle);
    int64_t file_size = getFileSize(filename);

    Lines *lines;
    FILE *line_file = openAtOffset(filename, 0);
    if (line_file == NULL)
        return -1;
    // Start the line searcher thread.
    LineSearcherState line_state = {.lines = &lines, .file = line_file, .len = file_size};
    pthread_t line_thread;
    pthread_create(&line_thread, NULL, lineSearcher, &line_state);

    // Start the threads searching for the keyword.
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    bool lines_valid = false;
    NeedleSearcherState *state = malloc(sizeof(NeedleSearcherState) * threads);
    pthread_t *searcher_threads = malloc(sizeof(pthread_t) * threads);
    for (int i = 0; i < threads; i++) {
        // overlap by needle_len - 1
        size_t offset = i * file_size / threads - (i > 0 ? needle_len - 1 : 0);
        state[i] = (NeedleSearcherState) {
            .needle = needle,
            .file = openAtOffset(filename, offset),
            .len = file_size / threads + (i > 0 ? needle_len - 1 : 0),
            .offset = offset,

            .cond = &cond,
            .mutex = &mutex,
            .lines_valid = &lines_valid,
            .lines = &lines,
        };
        pthread_create(&searcher_threads[i], NULL, needleSearcher, &state[i]);
    }

    // Wait for the lines to be ready, then signal the search threads.
    pthread_join(line_thread, NULL);

    pthread_mutex_lock(&mutex);
    lines_valid = true;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    int retval = 0;
    char *first_line = lines->len ? lines->lines[0].start : NULL;
    for (int i = 0; i < threads; i++) {
        NeedleSearcherResult *result;
        pthread_join(searcher_threads[i], (void **) &result);

        if (state[i].have_errors)
            retval = -1;

        for (NeedleSearcherResult *r = result; r; r = result) {
            size_t lineno = r->line - lines->lines;
            printf("%zu:%zu\t%.*s\n", lineno, state[i].offset + r->offset - (r->line->start - first_line), (int) r->line->len - 1, r->line->start);
            result = r->next;
            free(r);
        }
    }

    free(first_line);
    free(lines);
    free(state);
    free(searcher_threads);
    return retval;
}

