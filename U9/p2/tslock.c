#include "tslock.h"
#include <sched.h>

void tslock_init(TicketSpinlock *tslock)
{
    tslock->currentTicket = 0;
    tslock->ticketCounter = 0;
}

void tslock_lock(TicketSpinlock *tslock)
{
    __asm__ volatile 
    (void) tslock;
}

void tslock_unlock(TicketSpinlock *tslock)
{
    (void) tslock;
}