#include "memset.h"
#include "testlib.h"
#include <stdio.h>

int main()
{
    test_start("memset.c");

    uint64_t pat = 0x123456789ABCDEF0ull;
    uint64_t rotatedPat = rotr(pat);
    test_equals_int64(rotatedPat, 0xF0123456789ABCDEull, "rotr works for rotating 0x123456789ABCDEF0ull by one byte to 0xF0123456789ABCDEull");

    char buf[5];
    pattern_memset_byte(buf, &pat, 2);
    test_equals_int64(pat,  0xDEF0123456789ABCull, "pattern_memset_byte with n = 2 results in the pattern being rotated correctly by 2 bytes");

    pattern_memset_byte(buf, &pat, 3);
    test_equals_int64(pat,  0x789ABCDEF0123456ull, "calling pattern_memset_byte again with n = 3 results in the pattern being rotated correctly by 3 more bytes");


    pat  = 0x123456789ABCDEF0ull;
    unsigned char buffer[14];
    pattern_memset(buffer, pat, sizeof(buffer));

    char str[2*sizeof(buffer) + 1];
    for (size_t i = 0; i < sizeof(buffer); i++)
        sprintf(str + 2*i, "%02x", buffer[i]);

    test_equals_string(str, "f0debc9a78563412f0debc9a7856", "calling pattern_memset fills the buffer correctly");
    
    return test_end();
}
