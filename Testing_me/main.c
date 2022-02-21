#include <stdio.h>
#include"testing.h"
#include <pthread.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
    printf("hallo\n");
    test* t = malloc(sizeof(*t));
    t->i= 0;
    pthread_mutex_init(&t->l,NULL);
    
    multi_threading(t);
    printf("%i::\n", t->i);
    (void) argc; (void) argv;
    return 0;
}
