#include "testlib.h"
#include "fssize.h"

int main()
{
    test_start("fssize.c");

    test_equals_int64(getFilesystemSize("testdir"), 11, "fs size of testdir");

    return test_end();
}
