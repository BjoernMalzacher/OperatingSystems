#include "testlib.h"
#include "strings.h"



int main()
{
    test_start("strings.c");

    char *hello = "Hello";
    test_equals_int(stringlength(hello), 5, "stringlength works for 'Hello'");
    char *helloWorld = stringconcat(hello, " World!");
    test_equals_string(helloWorld, "Hello World!", "stringconcat works for 'Hello' and ' World!'");
    char **helloAndWorld = stringsplit(helloWorld, ' ');
    test_equals_string(helloAndWorld[0], hello, "stringsplit for 'Hello World!' with delimiter ' ' returns 'Hello' as the first substring");
    test_equals_string(helloAndWorld[1], "World!", "stringsplit for 'Hello World!' with delimiter ' ' returns 'World!' as the second substring");
    test_equals_ptr(helloAndWorld[2], NULL, "stringsplit for 'Hello World!' with delimiter ' ' returns result that is correctly terminated by NULL pointer");

    free(helloWorld);
    stringsplit_free(helloAndWorld);

    return test_end();
}
