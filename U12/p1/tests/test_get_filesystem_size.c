#include "testlib.h"
#include "fssize.h"

int main()
{
    test_start("Your getFilesystemSize works for testdir.");
    test_plan(1);

    test_equals_int64(getFilesystemSize("testdir"), 11, "fs size of testdir");

    return test_end();
}
