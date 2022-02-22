#include "testlib.h"
#include "fssize.h"
#include <pthread.h>
#include <sys/types.h>

#define PATH_A "testdir/a.txt"

extern pthread_mutex_t lock;
int locked = 0;

int tableContains(ino_t inode) {
    (void) inode;
    test_equals_int(locked, 1, "before calling tableContains the mutex was locked");
    return 1;
}

void tableInsert(ino_t inode) {
    (void) inode;
}

int __real_pthread_mutex_lock(pthread_mutex_t *mutex);
int __wrap_pthread_mutex_lock(pthread_mutex_t *mutex) {
    test_equals_ptr(mutex, &lock, "pthread_mutex_lock called with correct lock");
    locked = 1;
    return __real_pthread_mutex_lock(mutex);
}

int __real_pthread_mutex_unlock(pthread_mutex_t *mutex);
int __wrap_pthread_mutex_unlock(pthread_mutex_t *mutex) {
    test_equals_ptr(mutex, &lock, "pthread_mutex_unlock called with correct lock");
    locked = 0;
    return __real_pthread_mutex_unlock(mutex);
}
int main()
{
    test_start("Testing if you sync before calling hash table functions.");
    test_plan(4);
     /* initialize the mutex */
    pthread_mutex_init(&lock, NULL);

    processFile(PATH_A);
    test_equals_int(locked, 0, "after processFile is complete the mutex was unlocked");

    pthread_mutex_destroy(&lock);

    return test_end();
}
