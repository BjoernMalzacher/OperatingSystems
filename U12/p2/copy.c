#define _POSIX_C_SOURCE 2
#include "copy.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <regex.h>
#include <string.h>

// Intermediate copy buffer. Use this buffer for your read() and write() calls.
char _buffer[BUFFER_SIZE];

int parseCopyArgs(int argc, char * const argv[], CopyArgs* args)
{
    if (args == NULL) {
        return -1;
    }

    // Initialize with default values
    args->skip  = 0;
    args->count = -1;
    regex_t comp;
    if(regcomp(&comp, "-s ([0-9]+) -c ([0-9]+) ([/][a-zA-Z]+)+ ([/][a-zA-Z0-9., _-]+)+", REG_EXTENDED) == 0){
        printf("false reg:%s\n",*argv);
        return -1;
    }
    regmatch_t* pmatch = malloc(sizeof(regmatch_t)); 
    size_t nmatch = 0;
   if(regexec(&comp,*argv,nmatch,pmatch, REG_NOTBOL) == REG_NOMATCH){
        printf("false::::\n");
        return -1;
    }else{
        printf("hallo::::\n");
    }
    (void) argc;
    return -1;
}

int doCopy(CopyArgs* args)
{
    if (args == NULL) {
        return -1;
    }

    // ----------------
    // Copy the file.
    // ----------------

    return -1;
}
