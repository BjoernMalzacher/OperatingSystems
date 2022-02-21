#include "testlib.h"
#include "filesearch.h"

#define PATH "testfile.txt"

FILE  *__real_fopen(const char *path, const char *mode);
FILE  *__wrap_fopen(const char *path, const char *mode) {
	test_equals_string(path, PATH, "fopen called with correct path");
	test_equals_string(mode, "r", "fopen mode is read");
	return NULL;
}

int main() {
	test_start("You handle fopen errors in openAtOffset.");
	test_plan(3);

	test_equals_ptr(openAtOffset(PATH, 0), NULL, "returned pointer is NULL when fopen fails");

	return test_end();
}

