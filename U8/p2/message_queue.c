#include "message_queue.h"
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/*
 * The commands supported by the server
 */
typedef enum _Command {
    CmdExit      = 0x00,     // Stops the server

    CmdAdd,                  // Adds the two message parameters
    CmdSubtract              // Subtracts the two message parameters
} Command;

/*
 * The message format to be sent to the server.
 */
typedef struct _Message {
    /*
     * One of the command constants.
     */
    Command command;
    /*
     * Used as operand 1 (if required)
     */
    int parameter1;
    /*
     * Used as operand 2 (if required)
     */
    int parameter2;
} Message;

#define QUEUE_NAME "/simple_calculator"

#define FORMAT_STRING_ADD      "%d + %d = %d\n"
#define FORMAT_STRING_SUBTRACT "%d - %d = %d\n"

mqd_t startClient(void)
{
    
    return mq_open(QUEUE_NAME, O_WRONLY);
}

int sendExitTask(mqd_t client)
{
    Message m; 
    m.command = CmdExit;
    m.parameter1 = 0; 
    m.parameter2 = 0; 
    if(mq_send(client, (char*)&m,sizeof(m),0) == -1){return -1;}
    return 0;

   
}

int sendAddTask(mqd_t client, int operand1, int operand2)
{
    Message m; 
    m.command = CmdAdd;
    m.parameter1 =operand1;
    m.parameter2 = operand2;
    if(mq_send(client, (char*) &m,sizeof(m),0) == -1){return -1;}
    return 0;

}

int sendSubtractTask(mqd_t client, int operand1, int operand2)
{
    Message m; 
    m.command = CmdSubtract;
    m.parameter1 =operand1;
    m.parameter2 = operand2;
    if(mq_send(client, (char*) &m,sizeof(m),0) == -1){return -1;}
    return 0;
}

int stopClient(mqd_t client)
{
    (void)client;

    // TODO: Clean up anything on the client-side
    return -1;
}

int runServer(void)
{
    int error = 0; 
    struct mq_attr attr;
    attr.mq_flags =0;
    attr.mq_curmsgs =0;
    attr.mq_maxmsg = 10;    
    attr.mq_msgsize = sizeof(Message);
    mqd_t queue = mq_open(QUEUE_NAME,O_RDONLY | O_CREAT | O_EXCL,  S_IWUSR | S_IWGRP | S_IWOTH,&attr);
    if(((int)queue) == -1) {
        printf("%i::%i", errno,EEXIST);   
        return -1;   
    }   
    int run = 1;
    Message* val = malloc(100);
    do{
        
        if(mq_receive(queue,( char*) val,sizeof(val),NULL) != sizeof(val)){error =-1;}
        switch (val->command)
        {
        case CmdExit:
            run =0; 
            break;
        
        case CmdAdd:
            printf(FORMAT_STRING_ADD, val->parameter1,val->parameter2, val->parameter1+val->parameter2);
            break;
        
        case CmdSubtract:
            printf(FORMAT_STRING_SUBTRACT, val->parameter1,val->parameter2, val->parameter1-val->parameter2);
            break;
        
        default:
            run =0;
            error = 1;
            break;
        }

    }while (run);
       if (mq_close(queue) == -1)  {
        error = 1;
    }
    if (mq_unlink((char*)&queue) == -1) {
        error = 1;
    }
    return error? -1 : 0;
}