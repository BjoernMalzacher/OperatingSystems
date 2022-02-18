#include "testlib.h"
#include "stringsearch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#define THREADS 5

int main() {
    test_start("stringsearch.c");

    const char *text = "hello\nworld\nworld\nhello\n";

    test_equals_int64(findFirstSubstring("hello", text, strlen(text)), 0, "offset of hello");
    test_equals_int64(findFirstSubstring("world", text, strlen(text)), 6, "offset of world");
    test_equals_int64(findFirstSubstring("a", "a", 1), 0, "find equal string");

    Lines *l = findLines(text, strlen(text));
    test_equals_int64(l->len, 4, "findLines finds the correct number of lines");
    test_equals_ptr(l->lines[0].start, text, "pointer to first line is correct");
    test_equals_ptr(l->lines[1].start, text + 6, "pointer to second line is correct");

    test_equals_ptr(findLineContaining(l, &text[2]), &l->lines[0], "findLineContaining finds first line");
    test_equals_ptr(findLineContaining(l, &text[8]), &l->lines[1], "findLineContaining finds second line");

    

    free(l);

    printf("searching for 'hello':\n");
    parallelStringSearch("hello", text, strlen(text), 1);

    int fd = open("/usr/share/dict/words", O_RDONLY);
    
    if (fd != -1) {
        struct stat s;
        fstat(fd, &s);
        size_t len = s.st_size;
        char *words = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);

        printf("searching for 'system' in /usr/share/dict/words:\n");
        parallelStringSearch("system", words, len, 4);
    
        munmap(words, len);
        close(fd);
    }

    return test_end();
}
