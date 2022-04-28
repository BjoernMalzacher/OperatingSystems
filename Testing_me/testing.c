#include <pthread.h>
#include "testing.h"
#include <stdlib.h>
void* exe(void* tp){
    test* t = (test*) tp;
    pthread_mutex_lock(&t->l);
    for (size_t i = 0; i < 500; i++){
        i++;
    }
    pthread_mutex_unlock(&t->l);
    return 0;
}

int mutlti_threading(test *t){
    
    
    
    
    for (size_t i = 0; i < 4; i++)
    {        
        pthread_t* n = malloc(sizeof(*n));
        
    pthread_create(n,NULL,exe,t);

    }

 return 0;   
}