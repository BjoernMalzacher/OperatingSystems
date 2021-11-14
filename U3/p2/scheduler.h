#ifndef SCHEDULER_H
#define SCHEDULER_H

/*
 * The maximum priority.
 */
#define HIGHEST_PRIORITY 5
/*
 * Maximum number of threads
 */
#define MAX_PROCESSES 20

void initScheduler();

void onProcessReady(int threadId);

void onProcessPreempted(int threadId);

void onProcessBlocked(int threadId);

int scheduleNextProcess();

int startProcess(int threadId, int priority);
#endif
