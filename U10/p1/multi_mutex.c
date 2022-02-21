#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <alloca.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>
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
    if ((mutexv == NULL) || (mutexc < 0)) {
        return -1;
    }
    for (int i = 0; i < mutexc; i++){
        if(mutexv[i]){
            int r = pthread_mutex_trylock(mutexv[i]);
            if( r == 0){
                continue;
            }
        }

        
        multi_mutex_unlock(mutexv,i);
        return -1;
    }
    return 0;
}
static int compare_mutexv(const void *p1,const void *p2 ){
   assert(p1 != NULL);
    assert(p2 != NULL);

    // Qsort passes a pointer to the two array elements to compare.
    // Each array element is a pthread_mutex_t*.
    pthread_mutex_t **mutex_a = (pthread_mutex_t **) p1;
    pthread_mutex_t **mutex_b = (pthread_mutex_t **) p2;
    return (*mutex_a < *mutex_b) ? -1 : ((*mutex_a == *mutex_b) ? 0 : 1);
}

int multi_mutex_lock(pthread_mutex_t **mutexv, int mutexc)
{
    if(mutexv == NULL || mutexc < 0)
        return -1;
    if(mutexc == 0)
        return 0;
    pthread_mutex_t** cpy = malloc(sizeof(pthread_mutex_t)*mutexc);
  
    memcpy(*cpy,*mutexv, mutexc);   
    qsort(*cpy, mutexc,sizeof(pthread_mutex_t*), compare_mutexv);
    printf("%p:%p\n", cpy, mutexv);

    return -1;
}