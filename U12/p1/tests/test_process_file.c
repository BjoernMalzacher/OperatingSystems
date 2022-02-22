#include "testlib.h"
#include "fssize.h"
#include <pthread.h>
#include <search.h>

#define PATH_A "testdir/a.txt"
#define PATH_B "testdir/b.txt"
#define FILE_SIZE_A 7
#define FILE_SIZE_B 4

extern unsigned long long total; /* sum of file sizes */
extern pthread_mutex_t lock;
extern size_t hstrings_count;

int main()
{
    test_start("Your processPath correctly sums file sizes.");
    test_plan(3);
    /* initialize the mutex */
    pthread_mutex_init(&lock, NULL);
    hstrings_count = 0;
    hcreate(MAX_FILES);

    total = 0;
    processFile(PATH_A);

    test_equals_int64(total, FILE_SIZE_A, "processFile correctly increases total by number of bytes of a.txt");

    processFile(PATH_B);
    test_equals_int64(total, FILE_SIZE_A + FILE_SIZE_B, "processFile correctly increases total by number of bytes of a.txt and b.txt together");

    processFile(PATH_A);
    test_equals_int64(total, FILE_SIZE_A + FILE_SIZE_B, "processFile correctly does not recount a file twice");

    /* free remaining resources */
    pthread_mutex_destroy(&lock);
    hdestroy();

    return test_end();
}
