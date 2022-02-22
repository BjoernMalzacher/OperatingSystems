#include "testlib.h"
#include "fssize.h"
#include <string.h>

#define DIR_PATH "testdir"

int64_t numberOfFilesProcessed = 0;
DIR *openedDir = NULL;

DIR *__real_opendir(const char *name);
DIR *__wrap_opendir(const char *name) {
    test_equals_string(name, DIR_PATH, "opendir called with 'testdir'");
    openedDir = __real_opendir(name);
    return openedDir;
}

int __real_closedir(DIR *dirp);
int __wrap_closedir(DIR *dirp) {
    test_equals_ptr(dirp, openedDir, "closedir called with the directory that was previously openend");
    return __real_closedir(dirp);
}

void processEntry(const char *dirpath, struct dirent *entry) {
    test_equals_string(dirpath, DIR_PATH, "processEntry called with correct directory");
    if (strcmp(entry->d_name, "a.txt") == 0) {
        ++numberOfFilesProcessed;
    } else if (strcmp(entry->d_name, "b.txt") == 0) {
        ++numberOfFilesProcessed;
    } else if (strcmp(entry->d_name, "hardlink") == 0)  {
        ++numberOfFilesProcessed;
    } else if (strcmp(entry->d_name, ".") == 0) {
        ++numberOfFilesProcessed;
    } else if (strcmp(entry->d_name, "..") == 0)  {
        ++numberOfFilesProcessed;
    } else {
        test_failed_message("expected processEntry to be called with a.txt, b.txt and hardlink");
    }
}

int main()
{
    test_start("Testing listDir for example directory.");
    test_plan(8);

    listDir(DIR_PATH);
    test_equals_int64(numberOfFilesProcessed, 5, "Your listDir calls processEntry for the two files in the test directory, the hardlink, '.' and '..'.");
    return test_end();
}
