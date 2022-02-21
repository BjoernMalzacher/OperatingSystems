#include "ls.h"
// You may not need all these headers ...
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

// Use this function to display the files. DO NOT CHANGE IT.
void _printLine(unsigned int size, unsigned int sizeOnDisk, const char* name)
{
    printf("%010u   %010u   %s\n", size, sizeOnDisk, name);
}

// Assume this to be the maximum length of a file name returned by readdir
#define MAX_FILE_NAME_LENGTH 255

int list(const char* path, const char *filterByExtension) {
    if(path == NULL){
        return -1;
    }
    DIR *dir = opendir(path);
    struct dirent* t = malloc(sizeof(struct dirent));
    struct stat* buff = malloc(sizeof(struct stat));
do
{
    
    t = readdir(dir);
    if(t == NULL){
        continue;   
    }
    
    char* p = malloc(strlen(path)+ strlen(t->d_name)+1);
    if(t->d_name[0] == '.')
        continue;
    if(strrchr(t->d_name,'.') == NULL && filterByExtension != NULL)
        continue;
    
    
    if( filterByExtension != NULL &&strcmp(strrchr(t->d_name,'.')+1, filterByExtension) != 0)
        continue;
    strcpy(p,path);
    strcat(p,t->d_name);
    if(stat(p,buff) != 0)
        return -1;
    _printLine((unsigned int)buff->st_size,(buff->st_blocks*512),t->d_name);
    
} while (t != NULL);
    
    if(dir ==NULL){
        return -1;
    }
    free(buff);
    free(t);
    if(closedir(dir) != 0 ){
        return -1;
    }
    
    (void) filterByExtension;

    return 0;
}
