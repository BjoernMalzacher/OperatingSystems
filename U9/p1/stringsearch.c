#include "stringsearch.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Attempt to find needle within the first `len` bytes of haystack, returning
 * the first offset if successful, or -1 otherwise.
 */
ssize_t findFirstSubstring(const char *needle, const char *haystack, size_t len)
{
<<<<<<< HEAD
    int lastDiff = 0;
    size_t eq =0;
    for (size_t i = 0; i <= len; i++) {
        if(*(needle+eq) == '\0' ||eq >= strlen(needle) ){
            return lastDiff;
        }
        if(*(haystack+i) == *(needle+eq)){
            eq +=1;

        }else{
            lastDiff = i+1;
            eq = 0;
        }
        
    }
    
=======
    size_t count =0;
    if(len < strlen(needle))
        return-1;
    for (size_t i = 0; i < len; i++)
    {
        count =0;
        while (*(needle+count) == *(haystack+count+i)) {
            count +=1;
            if( strlen(needle) <= count)
                return i;
            if(count+i >= len)
                return -1;
        }
        
        
    }
>>>>>>> 5d97185cefc01a1d79aa27450ffc5e025b729f2e
    return -1;
}

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
 * Walk through `haystack` and find all lines (delimited by '\n').
 */
Lines *findLines(const char *haystack, size_t len)
{
    Lines *lines = newLines();
<<<<<<< HEAD


    size_t startOfLine = 0;
    int endOfLine = findFirstSubstring("\n",haystack+startOfLine,len);
      

    while (*(haystack+startOfLine) !='\0') { 
        Line l;
        Lines** linespointer = &lines; 
        endOfLine = findFirstSubstring("\n",haystack+startOfLine,len);
        l.start = haystack+startOfLine;
        l.len = endOfLine-startOfLine;
        appendLine(linespointer,l);
        startOfLine += endOfLine+1;
        
    }    
=======
    Line newLine;
    int last_Start = 0;
    for (size_t i = 0; i < len; i++) {
        if(*(haystack+i) == '\n') {
            newLine.start = haystack+last_Start;
            newLine.len = i;
            last_Start +=i+1;
            appendLine(&lines, newLine); 
        }
        
    }
    
>>>>>>> 5d97185cefc01a1d79aa27450ffc5e025b729f2e

    return lines;
}

/*
 * Use binary search to find the line corresponding to the given match.
 */
Line *findLineContaining(Lines *l, const char *match)
{
<<<<<<< HEAD

    for (size_t i = 0; i < l->len; i++)
    {
            printf("%s:::%li\n", l->lines[i].start, i);

    }
    
    size_t index =(l->len)/2;
    size_t eq =0;
    printf("%li\n",l->len);
    while(0){
          printf("%c:::%li \n",*(l->lines[index].start+eq), eq);
        if(*(l->lines[index].start+eq) == *(match+eq)) {
        printf("%c:::%c \n",*(l->lines[index].start+eq), *(match+eq));
            
            eq +=1;

        }else if(*(l->lines[index].start+eq) < *(match+eq)) {
            printf("<%li\n", index);

            index = index/2;
            eq = 0;
                       
        }else if (*(l->lines[index].start+eq) > *(match+eq)) {
            printf(">%li\n", index);
            index = (index/2)+index; 
            eq = 0;   
        }
        if(eq >=l->lines[index].len){
            return &(l->lines[index]);
        }
        
        
    }
   
    return &(l->lines[index]);
=======
   size_t lower = 0, upper = l->len;
    size_t middle;
    while (lower <= upper) {
        middle = (upper + lower) / 2;
        if (match >= l->lines[middle].start && match < l->lines[middle].start + l->lines[middle].len) {
            return &l->lines[middle];
        } else if (match < l->lines[middle].start) {
            upper = middle - 1;
        } else {
            lower = middle + 1;
        }
    }
    return NULL;
>>>>>>> 5d97185cefc01a1d79aa27450ffc5e025b729f2e
}

typedef struct _LineSearcherState {
    Lines **lines;
    const char *haystack;
    size_t len;
} LineSearcherState;

/*
 * Entry point for the line searching thread.
 */
void *lineSearcher(void *arg)
{
    LineSearcherState *state = arg;
    *state->lines = findLines(state->haystack, state->len);
    return NULL;
}

/*
 * Entry point for the needle searching threads.
 */
void *needleSearcher(void *arg)
{
    
    NeedleSearcherState *state = arg;
    NeedleSearcherResult *results = malloc(sizeof(NeedleSearcherResult));
    NeedleSearcherResult *start_Res = results; 
    NeedleSearcherResult *last_res = NULL;
    int needle_len = strlen(state->needle);
    int offset =0;
    int start = 0;
    
  while(offset >=0) {

        offset = findFirstSubstring(state->needle,state->haystack+start,state->len-start);
        
        if(offset != -1){
            results->match = state->haystack + start+offset;
            
            results->next = malloc(sizeof(NeedleSearcherResult));
            
            last_res = results;
            
            results = results->next; 
            
             
        }else{
            
            last_res->next = NULL;
        
            break;
        }
        
        start +=(needle_len+offset);
        
    }
    
    results = start_Res;
    
    
         
     
     



    /* TODO: Find all substrings by calling findFirstSubstring() and collect
     * them in a linked list with NeedleSearcherResult. */

    // Wait for line information to be ready.
    pthread_mutex_lock(state->mutex);
    while (!*state->lines_valid) {
        pthread_cond_wait(state->cond, state->mutex);
    }
    // Once the lines are valid, there are no more writes to them, so no
    // locking is necessary.
    pthread_mutex_unlock(state->mutex);

    /* TODO: Add line information to the results by calling findLineContaining(). */
        while (results != NULL)
    {
        results->line = findLineContaining(*(state->lines),results->match);
        results = results->next;
    }
    results = start_Res;

    return results;
}


/*
 * Perform a parallel search for `needle` on `haystack`, printing all matching lines.
 */
void parallelStringSearch(const char *needle, const char *haystack, size_t hay_len, int threads)
{
    size_t needle_len = strlen(needle);

    Lines *lines;
    // Start the line searcher thread.
    LineSearcherState line_state = {.lines = &lines, .haystack = haystack, .len = hay_len};
    pthread_t line_thread;
    pthread_create(&line_thread, NULL, lineSearcher, &line_state);

    // Start the threads searching for the keyword.
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    bool lines_valid = false;
    NeedleSearcherState *state = malloc(sizeof(NeedleSearcherState) * threads);
    pthread_t *searcher_threads = malloc(sizeof(pthread_t) * threads);
    for (int i = 0; i < threads; i++) {
        state[i] = (NeedleSearcherState) {
            .needle = needle,
            // overlap by needle_len - 1
            .haystack = haystack + i * hay_len / threads - (i > 0 ? needle_len - 1 : 0),
            .len = hay_len / threads + (i > 0 ? needle_len - 1 : 0),

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

    for (int i = 0; i < threads; i++) {
        NeedleSearcherResult *result;
        pthread_join(searcher_threads[i], (void **) &result);
        for (NeedleSearcherResult *r = result; r; r = result) {
            size_t lineno = r->line - lines->lines;
            printf("%zu:%zu\t%.*s\n", lineno, r->match - r->line->start, (int) r->line->len - 1, r->line->start);
            result = r->next;
            free(r);
        }
    }

    free(lines);
    free(state);
    free(searcher_threads);
}

