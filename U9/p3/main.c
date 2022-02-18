#include "testlib.h"
#include "hybrid_scheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

// This does return an address close to the top of the stack.
void *getSp()
{
    int i;
    uintptr_t iPos = (uintptr_t)&i;
    return (void*) iPos;
}

mutex *m1, *m2;
int counter = 0;

void t1()
{
    printf("Thread 1: Step 1, sp is %p\n", getSp());
    mutexLock(m1);
    printf("Thread 1: Locked m1\n");
    sleep(1);
    printf("Thread 1: Unlock m1\n");
    mutexUnlock(m1);
    printf("Thread 1: Step 2, sp is %p\n", getSp());
    sleep(1);
    yield();
    printf("Thread 1: Finished\n");
    // Thread should terminate.
}

void t2()
{
    for (int i = 1; i < 5; i++) {
        printf("Thread 2: Step %d, sp is %p\n", i, getSp());
        mutexLock(m1);
        printf("Thread 2: Locked m1\n");
        sleep(1);
        printf("Thread 2: Unlock m1\n");
        mutexUnlock(m1);
        yield();
    }
    printf("Thread 2: Finished\n");
}

void t3()
{
    printf("Thread %d: start counting\n", getThreadId());
    for (int i = 0; i < 10000; i++) {
        mutexLock(m2);
        counter++;
        mutexUnlock(m2);
        if (i % 100 == 99) yield();
    }
    printf("Thread %d: finish counting\n", getThreadId());
}

int main() {
    test_start("hybrid_scheduler.c");
    initScheduler();
    m1 = mutexNew();
    m2 = mutexNew();

    int tid1 = startThread(t1, 1);
    printf("Started thread: %d\n", tid1);
    test_equals_int(tid1, 0, "first thread gets tid 0");

    int tid2 = startThread(t2, 1);
    printf("Started thread: %d\n", tid2);
    test_equals_int(tid2, 1, "second thread gets tid 1");

    for (int i = 0; i < 4; i++) {
        startThread(t3, 2);
    }

    startScheduler();
    joinScheduler();

    test_equals_int(counter, 40000, "concurrent count is correct");

    mutexFree(m1);

    return test_end();
}
