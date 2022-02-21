#include "testlib.h"
#include "filesearch.h"

#define OFFSET 42

FILE *file;

int __real_fseek(FILE *stream, long offset, int whence);
int __wrap_fseek(FILE *stream, long offset, int whence) {
	test_equals_ptr(stream, file, "fseek is called with the correct FILE pointer");
	test_assert(offset == OFFSET, "fseek is called with correct offset");
	test_assert(whence == 0 || whence == SEEK_SET, "fseek is called correctly to seek from beginning of file");
	return -1;
}

int __real_fclose(FILE *stream);
int __wrap_fclose(FILE *stream) {
	test_equals_ptr(stream, file, "fclose is called with the correct FILE pointer when fseek fails");
	return __real_fclose(stream);
}

FILE *__real_fopen(const char *path, const char *mode);
FILE *__wrap_fopen(const char *path, const char *mode) {
	file = __real_fopen(path, mode);
	return file;
}

int main() {
	test_start("You handle fseek errors in openAtOffset.");
	test_plan(5);

	test_equals_ptr(openAtOffset("testfile.txt", OFFSET), NULL, "returned pointer is NULL when fseek fails");

	return test_end();
}
