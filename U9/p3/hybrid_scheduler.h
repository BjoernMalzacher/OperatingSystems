#ifndef HYBRID_SCHEDULER_H
#define HYBRID_SCHEDULER_H

#include <inttypes.h>

/*
 * The number of kernel-level threads.
 */
#define KLT_COUNT 4

/*
 * The maximum priority.
 */
#define HIGHEST_PRIORITY 5

/*
 * The stack size of a single user-level thread (4 KiB = 1 page)
 */
#define STACK_SIZE (10*4096)

/*
 * Queue API
 */

typedef struct _Thread Thread;

typedef struct _QueueItem QueueItem;

typedef struct _QueueItem {
    /*
     * The data of this item.
     */
    Thread *thread;

    /*
     * The next item in the queue.
     * NULL if there is no next item.
     */
    struct _QueueItem *next;
} QueueItem;

typedef struct _Queue {
    /*
     * The queue head is represented by a dummy item.
     * The queue is empty if tail == head.
     */
    QueueItem *head;
    /*
     * The last item of the queue.
     */
    QueueItem *tail;
} Queue;

void initQueue(Queue *q);
void freeQueue(Queue *q);
void enqueue(Queue *q, Thread *t);
Thread *dequeue(Queue *q);

/*
 * Scheduler API
 */

void initScheduler();
void startScheduler();
void joinScheduler();
int startThread(void (*func)(void), int priority);

/*
 * API for threads running within the scheduler
 */

int getKLT();
int getThreadId();
void yield();

typedef struct _mutex mutex;
mutex *mutexNew();
void mutexLock(mutex *m);
int mutexTryLock(mutex *m);
void mutexUnlock(mutex *m);
void mutexFree(mutex *m);

#endif
