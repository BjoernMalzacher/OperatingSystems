// This changes the way some includes behave.
// This should stay before any include.
#define _GNU_SOURCE

#include "pipe.h"
#include <sys/wait.h> /* For waitpid */
#include <unistd.h> /* For fork, pipe */
#include <stdlib.h> /* For exit */
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
int run_program(char *file_path, char *argv[])
{

    if ((file_path == NULL) || (argv == NULL)) {
        return -1;
    }
    int* mailbox = malloc(sizeof(int)*2); 
    pipe2(mailbox, O_CLOEXEC);
    // -------------------------
    // TODO: Open a pipe
    // -------------------------

    int child_pid = fork();
    if (child_pid == -1) {
        return -1;
    } else if (child_pid == 0) {

        // Replace program
        execvp(file_path, argv);
        write(*(mailbox+1),&child_pid,sizeof(int));
        // -------------------------
        // TODO: Write the error on the pipe
        // -------------------------

        exit(0);
    } else {
        int status, hadError = 0;

        int waitError = waitpid(child_pid, &status, 0);
        if (waitError == -1) {
            // Error while waiting for child.
            hadError = 1;
        } else if (!WIFEXITED(status)) {
            // Our child exited with another problem (e.g., a segmentation fault)
            // We use the error code ECANCELED to signal this.
            hadError = 1;
            errno = ECANCELED;
        } else {
            close(*(mailbox+1));
            int buffer;
            read(*mailbox,&buffer, sizeof(int));
            printf("%i:::\n",buffer);
          
        /*    if(buffer == -1){
                return -1;
            }*/
            // -------------------------
            // TODO: If there was an execvp error in the child, set errno
            //       to the value execvp set it to.
            // -------------------------
        }

        return hadError ? -1 : WEXITSTATUS(status);
    }
}
