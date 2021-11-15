#include "run_program.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "run_program.h"
#include <sys/wait.h> /* For waitpid */
#include <unistd.h> /* For fork */
#include <string.h> /* For memcpy */
#define ERROR_CODE 127

int run_program(char *file_path, char *arguments[])
{
    int status;
    int child_pid; 
    if((file_path == NULL   ) || (arguments == NULL)){
        return ERROR_CODE;
    }
    child_pid = fork();
    if(child_pid == -1){
        return ERROR_CODE;

    }else if(child_pid == 0 ){
         int num = 0; 
         char **nargv  ;
        while(arguments[num++] != NULL);

        nargv = (char**)malloc(sizeof(char*) * (num+1)   );
        if(nargv == NULL){
            exit(ERROR_CODE);
        }
        nargv[0] = file_path;
        memcpy(&nargv[1], nargv, sizeof(char*)*num);
        exit(ERROR_CODE);
   
   
    }else{

    if(waitpid(child_pid, &status, 0) == -1){
        return ERROR_CODE;
    }
    return (WIFEXITED(status)) ? WEXITSTATUS(status): ERROR_CODE;
    }
   
    return ERROR_CODE;
}
