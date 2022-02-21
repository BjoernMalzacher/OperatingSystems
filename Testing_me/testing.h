#include <pthread.h>

typedef struct __test{
    int i; 
    pthread_mutex_t l; 
}test;

void* exe(void* tp); 
int multi_threading(test *t);