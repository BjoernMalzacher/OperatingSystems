#include "testlib.h"
#include "fssize.h"
#include <pthread.h>
#include <semaphore.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>

extern pthread_mutex_t lock;
extern long stack_working;       /* number of directories that are not (completely) processed yet */
extern unsigned long long total; /* sum of file sizes */
extern size_t hstrings_count;
extern sem_t stack_fill; /* fill level of the stack */

int lock_initialized = 0;
int sem_initialized = 0;
int hash_table_initialized = 0;
int pthread_called = 0;
int num_threads_created = 0;

int __real_pthread_mutex_init(pthread_mutex_t *restrict mutex,
                              const pthread_mutexattr_t *restrict attr);

int __wrap_pthread_mutex_init(pthread_mutex_t *restrict mutex,
                              const pthread_mutexattr_t *restrict attr) {
    test_equals_ptr(mutex, &lock, "mutex init called");
    lock_initialized = 1;
    return __real_pthread_mutex_init(mutex, attr);
}


int __real_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine)(void *), void *arg);
int __wrap_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine)(void *), void *arg) {
    if (pthread_called == 0)
    {
        test_equals_int(hash_table_initialized, 1, "hash table is initialized before other threads start working on it");
        pthread_called = 1;
    }
    ++num_threads_created;
    return __real_pthread_create(thread, attr, start_routine, arg);
}

int __real_sem_init(sem_t *sem, int pshared, unsigned int value);
int __wrap_sem_init(sem_t *sem, int pshared, unsigned int value) {
    test_equals_ptr(sem, &stack_fill, "semaphore init called with correct stack_fill pointer");
    sem_initialized = 1;
    return __real_sem_init(sem, pshared, value);
}

int __real_hcreate(size_t nel);
int __wrap_hcreate(size_t nel) {
    test_equals_int(nel, MAX_FILES, "created hash table with correct size");
    hash_table_initialized = 1;
    return __real_hcreate(nel);
}

void addDirectory(const char *dirpath) {
    (void) dirpath;

    test_equals_int(stack_working, 0, "stack_working correctly initialized");
    test_equals_int(total, 0, "total correctly initialized");
    test_equals_int(hstrings_count, 0, "total correctly initialized");
    test_equals_int(lock_initialized, 1, "lock is initialized");
    test_equals_int(sem_initialized, 1, "semaphore is initialized");

    /* Don't do anything - we'll be done immediately. */
}

int main()
{
    test_start("Your getFilesystemSize seems to initialize resources.");
    test_plan(10);

    getFilesystemSize("testdir");
    test_equals_int(num_threads_created, NUM_THREADS, "getFileSystemSize uses correct number of threads");
    return test_end();
}
