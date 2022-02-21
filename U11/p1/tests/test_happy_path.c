#include "testlib.h"
#include "filesearch.h"
#include <stdlib.h>

int main() {
    test_start("filesearch.c works with example in main.c");
    test_plan(5);


    FILE *file = openAtOffset("testfile.txt", 0);
    test_assert(file != NULL, "open testfile.txt");

    int64_t size = getFileSize("testfile.txt");
    test_equals_int64(size, 24, "file size of testfile.txt");

    char *buf = allocateAndRead(file, size);
    if (buf) buf[size - 1] = 0;
    test_equals_string(buf, "hello\nworld\nworld\nhello", "contents of testfile.txt");

    free(buf);
    if (file) fclose(file);

    file = openAtOffset("testfile.txt", 6);
    test_assert(file != NULL, "open testfile.txt at offset 6");

    buf = allocateAndRead(file, size - 6);
    if (buf) buf[size - 7] = 0;
    test_equals_string(buf, "world\nworld\nhello", "contents of testfile.txt at offset 6");

    free(buf);
    if (file) fclose(file);


    return test_end();
}
