#include "barrier.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
struct _ThreadBarrier {

    int current_waiting_elements;
    int max_waiting_elements;
    //if (current_waiting_element == max_waiting_elements) is_barrier_active =0 else 1;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

};

ThreadBarrier *createBarrier(int threads) {
    ThreadBarrier *b = malloc(sizeof(ThreadBarrier));
    b->current_waiting_elements =0;
    b->max_waiting_elements = threads;
    if(pthread_mutex_init(&(b->mutex),NULL)){
        free(b);
        return NULL;
    }
    if(pthread_cond_init(&(b->cond), NULL)){
        pthread_mutex_destroy(&b->mutex);
        free(b);
        return 0;
    }
    return b;
    
}

void enterBarrier(ThreadBarrier *barrier)
{
    pthread_mutex_lock(&barrier->mutex);
    if(barrier->current_waiting_elements < barrier->max_waiting_elements-1){
        barrier->current_waiting_elements++;
        pthread_cond_wait(&barrier->cond,&barrier->mutex);
    }else{
        pthread_cond_broadcast(&barrier->cond);
        barrier->current_waiting_elements = 0;
    }    
    pthread_mutex_unlock(&barrier->mutex);
    
}

void deleteBarrier(ThreadBarrier *barrier)
{
    if(barrier == NULL)
        return;
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
    free(barrier);   
}