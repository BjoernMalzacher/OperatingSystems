#include "testlib.h"
#include "filesearch.h"

#define LENGTH 42

FILE *file;
void *bufferAllocated;

void __real_free(void *ptr);
void __wrap_free(void *ptr) {
    test_equals_ptr(ptr, bufferAllocated, "free called to return memory allocated to the buffer when fread does not read all bytes");
    __real_free(ptr);
}

void *__real_malloc(size_t size);
void *__wrap_malloc(size_t size) {
    test_equals_int(size, LENGTH, "malloc called with correct size for buffer");
    bufferAllocated = __real_malloc(size);
    return bufferAllocated;
}

size_t __real_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t __wrap_fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    test_equals_ptr(ptr, bufferAllocated, "fread called correctly with the previously allocated buffer");
    test_assert((size == 1 && nmemb == LENGTH) || (size == LENGTH && nmemb == 1), "fread called with correct size to read len bytes");
    test_equals_ptr(stream, file, "fread called with correct pointer to the file");
    return 0;
}


int main()
{
    test_start("You handle errors in allocateAndRead.");
    test_plan(6);
    file = fopen("testfile.txt", "r");
    if(file == NULL) {
        test_failed_message("something unexpected happen: test could not open file to use for testing");
        test_end();
    }
    test_equals_ptr(allocateAndRead(file, LENGTH), NULL, "allocateAndRead correctly returns NULL when fread does not read all bytes");

    return test_end();
}
