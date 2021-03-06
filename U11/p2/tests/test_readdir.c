#include "testlib.h"
#include "ls.h"
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#include "tests/stat.h"

int dir;
struct dirent e;
int pass;

DIR *opendir(const char *name) {
	(void) name;
	return (DIR*) &dir;
}

struct dirent *readdir(DIR *dirp) {
	test_equals_ptr(dirp, (DIR*) &dir, "You call readdir with the correct pointer")
	e.d_name[0] = 'a' + pass;
	e.d_name[1] = 0;
	e.d_ino = 0;
	pass++;
	return pass > 3 ? NULL : &e;
}

int closedir(DIR *dirp) {
	(void) dirp;
	return 0;
}

int main() {
	test_start("Your list calls readdir to read entries from the directory.");
	test_plan(5);

	test_equals_int(list("dirname", NULL), 0, "list succeeds");

	return test_end();
}

