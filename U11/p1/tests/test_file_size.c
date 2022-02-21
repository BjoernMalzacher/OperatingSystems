#include "testlib.h"
#include "filesearch.h"

int main()
{
    test_start("You return the file size of a given file.");
    test_plan(2);

    test_equals_int64(getFileSize(NULL), -1, "getFileSize returns -1 for non-existant files");
    test_equals_int64(getFileSize("testfile.txt"), 24, "getFileSize returns the correct size of testfile.txt");

    return test_end();
}
