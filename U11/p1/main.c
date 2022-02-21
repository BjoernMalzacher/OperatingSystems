#include "testlib.h"
#include "filesearch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    test_start("filesearch.c");

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

    printf("searching for 'hello':\n");
    test_equals_int(parallelFileSearch("hello", "testfile.txt", 1), 0, "search for hello works");

    const char *words_path = "/usr/share/dict/words";
    if (getFileSize(words_path) != -1) {
        printf("searching for 'system' in %s\n", words_path);
        test_equals_int(parallelFileSearch("system", words_path, 4), 0, "search for system works");
    }

    return test_end();
}
