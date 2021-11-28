#include "mmu.h"
#include <stdio.h>
static SegmentTable *_table;

// ----------------
// Add your global data structures here.
// ----------------

void setSegmentTable(SegmentTable *newTable)
{
    _table = newTable;
    flushTLB();
}

int translateSegmentTable(uint32_t *address) {
    if(_table == NULL){
        return -1;
    }

    int i = (*address>>29);
    uint32_t ad  = *address&0x1FFF;
    
    if((_table->segments[i].base<= ad) && ((_table->segments[i].length+_table->segments[i].base)> ad)){
        //printf("%i:::%i\n", _table->segments[i].base,_table->segments[i].length);
        *address = ad;
        return 0;
    }

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