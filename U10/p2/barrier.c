#include "barrier.h"
#include <unistd.h>

struct _ThreadBarrier {
};

ThreadBarrier *createBarrier(int threads)
{
    (void) threads;
    return NULL;
}

void enterBarrier(ThreadBarrier *barrier)
{
    (void) barrier;
}

void deleteBarrier(ThreadBarrier *barrier)
{
    (void) barrier;
}