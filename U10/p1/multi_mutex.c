#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <alloca.h>
#include <pthread.h>

#include "multi_mutex.h"

int multi_mutex_unlock(pthread_mutex_t **mutexv, int mutexc)
{
    for (int i = 0; i < mutexc; i++) {
        if(pthread_mutex_unlock(*(mutexv+i))< 0)
            return -1;

    }
    return 0;
}

int multi_mutex_trylock(pthread_mutex_t **mutexv, int mutexc)
{
     
    for (int i = 0; i < mutexc; i++){
        if(pthread_mutex_trylock(*(mutexv)) < 0){
            for (int j = 0; j <= i; j++){
                pthread_mutex_unlock(*(mutexv+j));
        
            }
            return -1;       
        }

    }
    return 0;
}

int multi_mutex_lock(pthread_mutex_t **mutexv, int mutexc)
{
    (void) mutexv;
    (void) mutexc;
    return -1;
}