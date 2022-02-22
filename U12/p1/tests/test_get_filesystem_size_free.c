#include "testlib.h"
#include "fssize.h"
#include <pthread.h>
#include <semaphore.h>
#include <search.h>

extern pthread_mutex_t lock;
extern sem_t stack_fill; /* fill level of the stack */

int number_pthread_join_called = 0;
int hash_table_destroyed = 0;

int __real_pthread_join(pthread_t thread, void **retval);

int __wrap_pthread_join(pthread_t thread, void **retval)
{
    ++number_pthread_join_called;
    return __real_pthread_join(thread, retval);
}

int __real_pthread_mutex_destroy(pthread_mutex_t *mutex);
int __wrap_pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    test_equals_ptr(mutex, &lock, "lock destroyed");
    return __real_pthread_mutex_destroy(mutex);
}

int __real_sem_destroy(sem_t *sem);
int __wrap_sem_destroy(sem_t *sem)
{
    test_equals_ptr(sem, &stack_fill, "semaphore destroyed");
    return __real_sem_destroy(sem);
}

void __real_hdestroy(void);
void __wrap_hdestroy(void)
{
    hash_table_destroyed = 1;
    return __real_hdestroy();
}

int main()
{
    test_start("Your getFilesystemSize frees resources.");
    test_plan(4);

    getFilesystemSize("testdir");
    test_equals_int(number_pthread_join_called, NUM_THREADS, "getFileSystemSize joins correct number of threads");
    test_equals_int(hash_table_destroyed, 1, "hash table destroyed");

    return test_end();
}
