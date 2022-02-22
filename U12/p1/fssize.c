#include "fssize.h"

#include <assert.h>
#include <dirent.h>
#include <pthread.h>
#include <search.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t lock;
unsigned long long total; /* sum of file sizes */

long stack_working; /* number of directories that are not (completely) processed yet */
sem_t stack_fill;   /* fill level of the stack */

struct dirstack {
    char *path;
    struct dirstack *next;
} *dirhead;

/* Enqueue a directory for reading its contents. */
void addDirectory(const char *dirpath) {
    struct dirstack *entry = malloc(sizeof(struct dirstack));
    entry->path = strdup(dirpath);
    pthread_mutex_lock(&lock);
    entry->next = dirhead;
    dirhead = entry;
    pthread_mutex_unlock(&lock);
    __sync_fetch_and_add(&stack_working, 1);
    sem_post(&stack_fill);
}

/* Remove a directory to work on, waiting if there are none. */
char *popDirectory() {
    char *ret = NULL;
    sem_wait(&stack_fill);
    pthread_mutex_lock(&lock);
    struct dirstack *head = dirhead;
    if (head != NULL) {
        ret = dirhead->path;
        dirhead = head->next;
        free(head);
    }
    pthread_mutex_unlock(&lock);
    return ret;
}

/* Thread entry function. */
void *doWork(void *param) {
    (void) param;
    char *path;

    while (stack_working > 0) {
        path = popDirectory();
        if (path == NULL) {
            break;
        }

        listDir(path);

        if (__sync_fetch_and_sub(&stack_working, 1) == 1) {
            // everything done, signal other threads
            assert(dirhead == NULL);
            for (int i = 0; i < NUM_THREADS; i++)
                sem_post(&stack_fill);
        }

        free(path);
    }

    return NULL;
}

#define INO_STRSIZE (sizeof(ino_t)*2 + 1)
char hstrings[(MAX_FILES+1) * INO_STRSIZE];
size_t hstrings_count;

/* Checks whether the hash table contains the given inode. */
int tableContains(ino_t inode) {
    // Note: avoid using POSIX hash tables in real code.
    ENTRY e;
    char key[INO_STRSIZE];
    snprintf(key, INO_STRSIZE, "%lx", inode);
    e.key = key;
    return hsearch(e, FIND) != NULL;
}

/* Inserts an inode into the hash table. */
void tableInsert(ino_t inode) {
    ENTRY e;
    char *key = &hstrings[hstrings_count++ * INO_STRSIZE];
    snprintf(key, INO_STRSIZE, "%lx", inode);
    e.key = key;
    e.data = NULL;
    if (hsearch(e, ENTER) == NULL) {
        fprintf(stderr, "tableInsert failed - too many files\n");
        exit(1);
    }
}

/**
 * Main entry function: return the sum of the size of all files in `path`.
 * Note: Not reentrant or thread-safe, as all data is in global variables!
 */
unsigned long long getFilesystemSize(char *path) {
    (void) path;

    total = 0;
    stack_working = 0;

    /* Initialize hash table */
    hstrings_count = 0;
    hcreate(MAX_FILES);
    sem_init(&stack_fill, 0, 0);

    /* TODO: initialize remaining global variables */

    /* TODO: start scanning the directory */

    /* TODO: spawn the threads, then wait for completion */

    /* TODO: free remaining resources */

    sem_destroy(&stack_fill);
    hdestroy();

    return total;
}

/* Read the given directory and process its files and directories. */
void listDir(const char *dirpath) {
    /* TODO */
    (void) dirpath;
}

/* Calls the appropriate processing function for files and directories. */
void processEntry(const char *dirpath, struct dirent *entry) {
    /* TODO: check entry->d_type */
    (void) dirpath;
    (void) entry;
}

/* Process a file, reading its size. */
void processFile(const char *path) {
    /* TODO */
    (void) path;
}

/* Join a directory path and a filename to a newly-allocated string. */
char *makePath(const char *dirpath, const char *name) {
    /* TODO */
    (void) dirpath;
    (void) name;
    return NULL;
}


