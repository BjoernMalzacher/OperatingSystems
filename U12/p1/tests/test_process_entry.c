#include "testlib.h"
#include "fssize.h"
#include "stdlib.h"

#define NAME "somefile.txt"
#define DIR_PATH_TEST "testdir"

void processFile(const char *path) {
    test_equals_string(path, "testdir/somefile.txt", "processEntry calls processFile with correct path");
}

void addDirectory(const char *dirpath) {
    test_equals_string(dirpath, "testdir/somefile.txt", "processEntry calls addDirectory with correct path");
}

int main()
{
    test_start("Your processEntry calls the correct processFile or addDirectory.");
    test_plan(2);
    struct dirent processDirEntry = {.d_name = NAME, .d_type = DT_DIR};

    processEntry(DIR_PATH_TEST, &processDirEntry);

    processDirEntry.d_type = DT_REG;
    processEntry(DIR_PATH_TEST, &processDirEntry);

    return test_end();
}
