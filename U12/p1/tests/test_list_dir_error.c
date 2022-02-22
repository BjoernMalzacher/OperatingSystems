#include "testlib.h"
#include "fssize.h"
#include <stddef.h>

#define DIR_PATH "testdir"

int64_t numberOfFilesProcessed = 0;

void __real_exit(int status);
void __wrap_exit(int status) {
    test_equals_int64(status, -1, "Your listDir calls exit correctly with status -1 when opendir fails.");
    if (status == -1)
    {
        __real_exit(test_end());
    }
    else
    {
        __real_exit(status);
    }
}

DIR *__wrap_opendir(const char *name) {
    test_equals_string(name, DIR_PATH, "opendir called with 'testdir'");

    return NULL;
}

void processEntry(const char *dirpath, struct dirent *entry) {
    (void)dirpath;
    (void)entry;
    // no-op should not be called but for safety
}

int main()
{
    test_start("You listDir handles NULL pointer from opendir.");
    test_plan(2);
    listDir(DIR_PATH);
    return test_end();
}
