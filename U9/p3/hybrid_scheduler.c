#include "hybrid_scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

/* Set to 1 to get more verbose output */
#define SCHED_DEBUG 0

/*
 * Currently-active KLT
 */
__thread int _current_klt;
int getKLT() { return _current_klt; }

/* Epoch-based reclamation implementation */
/* ====================================== */
// Implementation based on Figure 2 in H. Wen, J. Izraelevitz, W. Cai, H. A.
// Beadle, and M. L. Scott, “Interval-based Memory Reclamation,” in Proceedings
// of the 23rd ACM SIGPLAN Symposium on Principles and Practice of Parallel
// Programming, New York, NY, USA, 2018, pp. 1–13.

// The current epoch, incremented periodically.
volatile size_t ebr_epoch;
// Reservations of each thread - the current epoch.
// Initialized in initScheduler().
volatile size_t ebr_reservations[KLT_COUNT];
// Per-thread counter for triggering periodic operations.
__thread size_t ebr_counter;

// TODO: Define a per-thread linked-list for retired allocations.

// maximum epoch
#define EBR_EPOCH_MAX SIZE_MAX
// how often to increase epoch
#define EBR_EPOCH_FREQ 150
// how often to empty the retired list
#define EBR_EMPTY_FREQ 30

// Private function: empty the retired list
void ebr_empty()
{
    // TODO: Implement after Figure 2 in the paper.
}

// Public function: retire a block so that it is freed later.
void ebr_retire(void *ptr)
{
    (void) ptr;
    // TODO: Implement after Figure 2 in the paper.

    ebr_counter++;
    if (ebr_counter % EBR_EPOCH_FREQ == 0) {
        __sync_fetch_and_add(&ebr_epoch, 1);
    }
    if (ebr_counter % EBR_EMPTY_FREQ == 0) {
        ebr_empty();
    }
}

void ebr_start_op()
{
    ebr_reservations[_current_klt] = ebr_epoch;
}

void ebr_end_op()
{
    ebr_reservations[_current_klt] = EBR_EPOCH_MAX;
}

/* Lock-free queue implementation */
/* ============================== */

void initQueue(Queue *queue) {
    QueueItem *head = (QueueItem *) malloc(sizeof(QueueItem));
    head->next = NULL;
    queue->tail = queue->head = head;
}

void freeQueue(Queue *queue) {
    // Remove all regular items.
    Thread *t;
    do {
        t = dequeue(queue);
    } while (t);
    // Free the head.
    free(queue->head);
}

/*
 * Append to the tail of the queue.
 * Does nothing on error.
 */
void enqueue(Queue *queue, Thread *thread)
{
    assert(queue != NULL);

    QueueItem *item = (QueueItem *) malloc(sizeof(QueueItem));
    if (item == NULL) {
        return;
    }

    item->thread = thread;
    item->next = NULL;

    ebr_start_op();

    // TODO: Implement as lock-free algorithm.

    queue->tail->next = item;
    queue->tail = item;

    ebr_end_op();
}

/*
 * Remove and get the head of the queue.
 * Return NULL if the queue is empty.
 */
Thread *dequeue(Queue *queue)
{
    assert(queue != NULL);

    ebr_start_op();

    // TODO: Implement as lock-free algorithm.

    QueueItem *head = queue->head;
    QueueItem *next = head->next;
    if (next == NULL) {
        return NULL;
    }
    Thread *thread = next->thread;
    queue->head = next;

    ebr_end_op();

    // TODO: Use ebr_retire() here.
    free(head);

    return thread;
}

/* Hybrid Scheduler implementation */
/* =============================== */

typedef enum _ThreadState {
    STATE_INVALID = 0,

    STATE_READY,      // The thread is ready and should be on a ready queue for
                      //   selection by the scheduler
    STATE_RUNNING,    // The thread is running and should not be on a ready queue
    STATE_WAITING,    // The thread is blocked and should not be on a ready queue
    STATE_FINISHED,   // The thread has finished execution and needs to be cleaned up
} ThreadState;

struct _Thread {
    int threadId;
    ThreadState state;
    /*
     * Range: 0 ... HIGHEST_PRIORITY (including)
     * HIGHEST_PRIORITY is highest priority
     */
    int priority;
    /*
     * The stack pointer of this thread while it is not running.
     */
    void *currentSP;
    /*
     * The thread's stack.
     */
    char stack[STACK_SIZE];
};

/*
 * Kernel-level thread handles.
 */
pthread_t _klt_handles[KLT_COUNT];

/*
 * The ready queues for each priority.
 */
Queue _queues[HIGHEST_PRIORITY + 1] = {0};

/*
 * Finished threads waiting to be cleaned up.
 */
Queue _finished_threads;

/*
 * Next thread id.
 * For simplicity, we assume that the thread id never wraps around.
 */
int _next_thread_id = 0;

/*
 * Number of active threads, excluding KLT.
 */
volatile int _active_threads = 0;

/*
 * Currently-running thread.
 */
__thread Thread *_current_thread;
int getThreadId() { return _current_thread->threadId; }

/*
 * Adds a new, waiting thread.
 */
static Thread *allocThread(int priority)
{
    Thread *thread = malloc(sizeof(Thread));
    if (thread == NULL) return NULL;

    thread->threadId = __sync_fetch_and_add(&_next_thread_id, 1);
    thread->state    = STATE_WAITING;
    thread->priority = priority;

    return thread;
}

/*
 * Removes a thread that has finished execution.
 */
static void freeThread(Thread *thread)
{
    // Only finished threads are safe to free - others are generally in some queue.
    assert(thread->state == STATE_FINISHED);
    free(thread);
}

static void *_initKLT(void *param)
{
    _current_klt = (intptr_t) param;

    _current_thread = allocThread(0);
    _current_thread->state = STATE_RUNNING;
#if SCHED_DEBUG
    fprintf(stderr, "KLT %d: _initKLT -> thread %d\n", getKLT(),  _current_thread->threadId);
#endif

    // Spin and reclaim finished threads until all threads are finished.
    while (_active_threads > 0) {
        Thread *finished = dequeue(&_finished_threads);
        if (finished) {
            __sync_fetch_and_sub(&_active_threads, 1);
            freeThread(finished);
            continue; // check for more threads to free
        }
        yield();
    }

    _current_thread->state = STATE_FINISHED;
    freeThread(_current_thread);
    return NULL;
}

/*
 * Initialize the scheduler.
 */
void initScheduler()
{
    // Initialize EBR reservations.
    for (int i = 0; i < KLT_COUNT; i++) {
        ebr_reservations[i] = EBR_EPOCH_MAX;
    }
    // Initialize scheduling queues.
    for (int i = 0; i <= HIGHEST_PRIORITY; i++) {
        initQueue(&_queues[i]);
    }
    initQueue(&_finished_threads);
}

/*
 * Start the scheduler KLTs.
 */
void startScheduler()
{
    // Start kernel-level pthreads.
    for (int i = 0; i < KLT_COUNT; i++) {
        pthread_create(&_klt_handles[i], NULL, _initKLT, (void *)(intptr_t) i);
    }
}

/*
 * Wait until all threads have exited.
 */
void joinScheduler()
{
    for (int i = 0; i < KLT_COUNT; i++) {
        pthread_join(_klt_handles[i], NULL);
    }
}

static void _enqueueThread(Thread *thread)
{
    assert(thread->state == STATE_READY);

    // Append a thread to the right ready queue.
    Queue *queue = &_queues[thread->priority];
    enqueue(queue, thread);
}

/*
 * Called whenever a waiting thread gets ready to run.
 */
void onThreadReady(Thread *thread)
{
    assert(thread->state == STATE_WAITING);

    thread->state = STATE_READY;
    _enqueueThread(thread);
}

/*
 * Called whenever a running thread needs to wait.
 */
void onThreadWaiting(Thread *thread)
{
    assert(thread->state == STATE_RUNNING);

    thread->state = STATE_WAITING;
    // We do not need to enqueue waiting threads.
}

static Thread *_dequeueThread(int priority)
{
    assert(priority <= HIGHEST_PRIORITY);
    if (priority < 0) {
        // Idle thread if priority is less than 0. This will abort recursion.
        return NULL;
    }

    Queue *queue = &_queues[priority];
    Thread *dequeued = dequeue(queue);

    if (dequeued == NULL) {
        return _dequeueThread(priority - 1);
    }
    // Schedule a thread with the current priority.
    return dequeued;
}

/*
 * Selects and returns the next thread to run.
 */
Thread *scheduleNextThread()
{
    // Try to schedule a thread with the highest priority.
    // This will return a thread with a lower priority if required to prevent
    // starvation.
    return _dequeueThread(HIGHEST_PRIORITY);
}

/*
 * Select and switch to another thread, and then enqueue the current thread to the given queue.
 */
void contextSwitch(Queue *queue)
{
    // Apply a scheduling policy to decide which user-level thread to run next.
    // This will update _currentThread to the next thread!
    Thread *prevThread = _current_thread;
    Thread *nextThread = scheduleNextThread();
    if (nextThread == NULL) {
        if (prevThread->state == STATE_RUNNING) {
            // There is no other thread to run, ignore yield().
            // Note that we will always fall back to the _initKLT() threads this way.
            return;
        }
        // We cannot continue running the current thread, try again until one becomes available.
        do { nextThread = scheduleNextThread(); } while (nextThread == NULL);
    }
    _current_thread = nextThread;

#if SCHED_DEBUG
    int klt = getKLT();
    fprintf(stderr, "KLT %d: Switch from thread %d with sp near %p\n",
        klt, prevThread->threadId, &prevThread->stack);
    fprintf(stderr, "KLT %d: Switch to thread %d with sp=%p\n",
        klt, _current_thread->threadId, _current_thread->currentSP);
#endif

    _current_thread->state = STATE_RUNNING;
    assert(_current_thread->currentSP != NULL);

    // Do the context switch
    __asm__ __volatile__ (
        "movq  %[prevTh], %%rax\n\t" // save prevThread from old stack
        "movq  %[queue], %%rcx\n\t"  // save queue from old stack
        "pushq %%rbp\n\t"            // Push registers to old stack
        "pushq %%rbx\n\t"
        "pushq %%r12\n\t"
        "pushq %%r13\n\t"
        "pushq %%r14\n\t"
        "pushq %%r15\n\t"

        "movq  %%rsp, %[prevSp]\n\t" // Store current stack pointer
        "movq  %[newSp], %%rsp\n\t"  // Switch to the new stack

        "popq  %%r15\n\t"            // Pop registers from new stack
        "popq  %%r14\n\t"
        "popq  %%r13\n\t"
        "popq  %%r12\n\t"
        "popq  %%rbx\n\t"
        "popq  %%rbp\n\t"
        "movq  %%rax, %[prevTh]\n\t" // restore prevThread to new stack
        "movq  %%rcx, %[queue]\n\t"  // restore queue to new stack

        : /* output */  [prevSp]"=m"(prevThread->currentSP), [prevTh]"+m"(prevThread), [queue]"+m"(queue)
        : /* input */   [newSp]"m"(_current_thread->currentSP)
        : /* clobber */ "rax", "rcx", "cc", "memory");

    if (prevThread->state == STATE_RUNNING) {
        prevThread->state = STATE_READY;
    }
    enqueue(queue, prevThread);
}

/*
 * Switches to the next thread.
 */
void yield()
{
    Queue *queue = &_queues[_current_thread->priority];
    contextSwitch(queue);
}

static void _threadFinished()
{
    _current_thread->state = STATE_FINISHED;
    contextSwitch(&_finished_threads);
    assert(!"contextSwitch should never return here");
}

/*
 * Prepares a new thread.
 * Returns the thread id or -1 on error.
 * This does not switch to the new thread, yet.
 */
int startThread(void (*func)(void), int priority)
{
    if (func == NULL) {
        return -1;
    }

    // Find a thread structure in our pre-allocated thread array, that we did
    // not use, yet.
    Thread *thread = allocThread(priority);
    if (thread == NULL) return -1;

    // Stacks grow from high addresses to low addresses. The stack thus
    // effectively starts at the end of the allocated memory area and
    // grows to the area's beginning.
    // We here assume the stack to be an array of void*'s, that is
    // pointers. A pointer has the right size (e.g., 32 or 64 bits) to
    // hold a full CPU register and can receive function pointers
    // without casting. stackTop is thus a pointer to pointers.
    void **stackTop = (void**)(thread->stack + STACK_SIZE);

    // We need to write the values to the stack in the reverse order
    // they will be needed when yield() switches to this stack.

    // 'Push' the address that func() will pop from the stack and jump
    // to when it returns. We will use parkThread here.
    stackTop--;
    *stackTop = _threadFinished;

    // The stack that is switched to on a yield().
    // The return of yield() will pop this address off the stack
    // and then jump to it. This way, the user function gets invoked.
    stackTop--;
    *stackTop = func;

    // Leave will pop this base pointer from the stack, when returning
    // from yield().
    stackTop--;
    *stackTop = stackTop;

    // Although we pop 6 registers after yield switches to this
    // stack, we only need to reserve space for 5 more, because we use
    // the position for the stack base pointer (ebp) twice (see
    // Understanding Solution PDF). We do not need to care about the
    // values because yield() switches to the thread for the first time
    // and there are thus no special values to restore.
    stackTop -= 5;

    // We initialized the stack and the thread is ready to run.
    // After setting STATE_READY, the thread is eligible to dispatching
    thread->currentSP = stackTop;
    thread->state     = STATE_READY;

    __sync_fetch_and_add(&_active_threads, 1);
    _enqueueThread(thread);

    return thread->threadId;
}

/* Mutex implementation */
/* ==================== */

typedef struct _mutex {
    // Threads waiting on the mutex.
    Queue waiting;
    // TODO: Declare lock variable.
} mutex;

/*
 * Allocate and initialize a new mutex.
 */
mutex *mutexNew()
{
    mutex *m = malloc(sizeof(mutex));
    initQueue(&m->waiting);
    // TODO: Initialize lock variable.
    return m;
}

/*
 * Lock the mutex. If the mutex is already locked, block the current thread.
 */
void mutexLock(mutex *m)
{
    (void) m;
    // TODO: Implement mutexLock()
    // - Try to acquire the lock by atomically changing the lock variable.
    // - If the mutex was already locked, suspend the current thread by running
    //     onThreadWaiting(_current_thread);
    //     contextSwitch(&m->waiting);
    // - Make sure that there is no race condition between checking the lock,
    //   adding the thread to the queue, and the wakup functionality in
    //   mutexUnlock().
}

/*
 * Try to lock the mutex. Return 0 on success, and a non-zero value otherwise.
 */
int mutexTryLock(mutex *m)
{
    (void) m;
    // TODO: Implement mutexTryLock()
    // - Try to acquire the lock by atomically changing the lock variable.
    // - Return immediately, even if the mutex could not be locked.
    return 1;
}

void mutexUnlock(mutex *m)
{
    (void) m;
    // TODO: Implement mutexUnlock()
    // - Free the lock by atomically changing the lock variable.
    // - Check if there are any other threads waiting on the mutex. Wake them
    //   up by running
    //     onThreadReady(thread);
    // - Make sure that there is no race condition as described in mutexLock().
}

void mutexFree(mutex *m) {
    freeQueue(&m->waiting);
    // TODO: Free additional fields, if necessary
    free(m);
}
