#include "testlib.h"
#include "fssize.h"
#include "string.h"
#include <stdlib.h>

int main()
{
    test_start("Testing makePath.");
    test_plan(6);

    char *result = NULL;

    char *shouldFirst = "/home/student/exam.txt";
    result = makePath("/home/student", "exam.txt");
    test_equals_int64(result[strlen(shouldFirst)], 0, "makePath with '/home/student', 'exam.txt' is correctly NULL byte terminated");
    test_equals_string(result, shouldFirst, "makePath with '/home/student', 'exam.txt' returns '/home/student/exam.txt");
    free(result);

    char *shouldSecond = "/on/";
    result = makePath("/on", "");
    test_equals_int64(result[strlen(shouldSecond)], 0, "makePath with '/on', '' is correctly NULL byte terminated");
    test_equals_string(result, shouldSecond, "makePath with '/on', '' returns '/on/");
    free(result);

    char *shouldThird = "./os.test";
    result = makePath(".", "os.test");
    test_equals_int64(result[strlen(shouldThird)], 0, "makePath with '.', 'os.test' is correctly NULL byte terminated");
    test_equals_string(result, shouldThird, "makePath with '.', 'os.test' returns './os.test");
    free(result);

    return test_end();
}
