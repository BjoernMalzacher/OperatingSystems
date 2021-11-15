#include "testlib.h"
#include "scheduler.h"
#include <stdio.h>

#define test_scheduleNextProcess(should) ({ \
    int scheduled = scheduleNextProcess(); \
    char msg[100]; \
    snprintf(msg, sizeof(msg), "%s line %d: Next Process: %d", __FILE__, __LINE__, should); \
    test_equals_int(scheduled, should, msg); \
})

int main()
{
    test_start("scheduler.c");
    initScheduler();

    startProcess(0, 5);
    startProcess(1, 4);
    startProcess(2, 2);
    startProcess(3, 4);
    startProcess(4, 3);
    startProcess(5, 4);
    startProcess(6, 2);
    startProcess(7, 0);

    test_scheduleNextProcess(-1);
    // Let's assume we idle some time.
    onProcessReady(1);
    test_scheduleNextProcess(1);
    // Let's assume Process 1 is running here.
    onProcessPreempted(1);
    test_scheduleNextProcess(1);
    // Let's assume Process 1 is still running here.
    // Process 1 decides to do some Disk IO.
    onProcessBlocked(1);
    test_scheduleNextProcess(-1);
    // Assume we idle some time.
    // Suddenly, thread 1 gets the data from the disk.
    onProcessReady(1);
    test_scheduleNextProcess(1);
    // Now thread 1 is running.
    // Process 0 and 7 get ready.
    onProcessReady(0);
    onProcessReady(7);
    // Process 1 is still running and gets preempted.
    onProcessPreempted(1);
    test_scheduleNextProcess(0);
    // Now thread 0 is running.
    for (int i = 0; i < 4; i++) {
        onProcessPreempted(0);
        test_scheduleNextProcess(0);
    }
    onProcessPreempted(0);
    // Now we have scheduled thread 0 five times.
    // This means that a lower priority thread should be scheduled next.
    test_scheduleNextProcess(1);

    // The whole thing should repeat until thread 1 was scheduled 5 times.
    // Then thread 7 should be scheduled.
    // You can write the test for this yourself.

    return test_end();
}
