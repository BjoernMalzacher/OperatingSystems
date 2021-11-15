#include "insert.h"
#include "testlib.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

char *debug_array_to_string(int *a, size_t len)
{
	static char buf[100];
	size_t written = 0;
	for (size_t i = 0; i < len && written < sizeof(buf) - 1; i++) {
		written += snprintf(buf + written, sizeof(buf) - written, "%d ", a[i]);
	}
	if (written > 0 && buf[written - 1] == ' ') {
		buf[written - 1] = 0;
	}
	return buf;
}

int main() {
	test_start("insert.c");

	size_t capacity = 10;
	int *a = malloc(sizeof(int) * capacity);
	size_t len = 9;
	for (size_t i = 0; i < len; i++) {
		a[i] = i < 4 ? i : i + 1;
	}
	insert(&a, &len, &capacity, 4);
	test_equals_int(a[4], 4, "insert() put 4 in the correct slot");
	test_equals_int(len, 10, "insert() updated length correctly");
	test_equals_int(capacity, 10, "insert() did not change the capacity");

	insert(&a, &len, &capacity, -1);
	test_equals_int(a[0], -1, "insert() put -1 in the correct slot");
	test_equals_int(len, 11, "insert() updated length correctly");
	test_equals_int(capacity, 20, "insert() updated the capacity correctly");

	test_equals_string(debug_array_to_string(a, len), "-1 0 1 2 3 4 5 6 7 8 9", "array contents are correct");

	free(a);

	return test_end();
}
