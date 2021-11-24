#include "mmu.h"

static SegmentTable *_table;

// ----------------
// Add your global data structures here.
// ----------------

void setSegmentTable(SegmentTable *newTable)
{
    _table = newTable;
    flushTLB();
}

int translateSegmentTable(uint32_t *address)
{
    (void) address;
    return -1;
}

void flushTLB(void)
{

}

void addToTLB(uint32_t virtual, uint32_t physical)
{
    (void) virtual;
    (void) physical;
}

int translateTLB(uint32_t *address)
{
    (void) address;
    return -1;
}