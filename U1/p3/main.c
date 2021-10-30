#include "testlib.h"
#include "bits.h"


int main()
{
    test_start("bits.c");

    uint64_t A[10] = {0};
    test_equals_int(getN(A, 100), 0, "bit 100 is initially 0");
    setN(A, 100);
    test_equals_int(getN(A, 100), 1, "bit 100 is then set to 1");
    clrN(A, 100);
    test_equals_int(getN(A, 100), 0, "bit 100 is finally cleared again");

    test_equals_int64(rot(0x1234, 8), 0x3400000000000012ULL, "rot(0x1234, 8)");

    return test_end();
}
