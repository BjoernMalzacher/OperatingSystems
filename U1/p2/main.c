#include "testlib.h"
#include "parseint.h"

int main()
{
    test_start("parseint.c");

    test_equals_int(parseDecimalChar('0'), 0, "parseDecimalChar parses '0'");
    test_equals_int(parseInt("010"), 8, "parseInt parses octal number");
    test_equals_int(parseInt("10"), 10, "parseInt parses decimal number");
    test_equals_int(parseInt("?"), -1, "parseInt handles invalid input");

    return test_end();
}
