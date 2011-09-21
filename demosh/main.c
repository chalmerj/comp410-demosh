//
//  main.c
//  demosh
//
//  Created by Jeremy Chalmer on 9/19/11.
//  Copyright 2011 Jeremy Chalmer. All rights reserved.
// 
//  Help found from these sources:
//  http://stackoverflow.com/questions/4788374/writing-a-basic-shell-for-linux-in-c
//  http://rik0.altervista.org/snippets/csimpleshell.html
//  http://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html#Implementing-a-Shell
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include "demosh.h"







char *cmdList[] = {
    "sleep",
    "wait",
    "wait-list",
    "time",
    "kill"
};


/* **************************************************************** */
/*                                                                  */
/*                              Main                                */
/*                                                                  */
/* **************************************************************** */

int main (int argc, const char * argv[])
{
    char buffer[BUFFER_SIZE];
    char *args[ARR_SIZE];
    int return_status;
    
    pid_t   pid = 0;
    
    size_t  numArgs;
    
    while (1){
        memset((void *) buffer, 0, sizeof(buffer));
        
        printf("$ ");
        if(!fgets(buffer, BUFFER_SIZE, stdin)) break;
        
        // Trim the trailing \n from the buffer
        char *ptr;
        if((ptr = strchr(buffer, '\n')) != NULL) *ptr = '\0';
        
        //Parse the command input
        parseInput(buffer, args, ARR_SIZE, &numArgs);
        
        printf("Command: %s\nNum. Args: %lu\n", args[0],numArgs);
        
        if (numArgs == 0) continue;
        
        if (!strcmp(args[0], "exit")) exit(0);
        
        if (!strcmp(args[0], "sleep") && (numArgs > 1))
        {
            long sleepVal;
            if (!(sleepVal = strtol(args[1], NULL, 10)))
            {
                printf("Incorrect Args\n"); 
                continue;
            }
                
            if ((pid = fork()) < 0) 
            {
                printf("Failed to fork process\n");
                break;
            }
            
            if (!pid) // We're the child process
            {
                sleep(sleepVal);
                break;
            }
            else 
            {
                printf("Child going to sleep for %lds!\n", sleepVal);
                printf("Waiting for child (%d)\n", pid);
                pid = wait(&return_status);
                printf("Child (%d) finished with status %d.\n",pid, return_status);
            }
        }
        
        //if ((checkCmdList(cmdList, args[0]) == 0)) printf("Command Found!\n");
        
    }
}

/* **************************************************************** */
/*                                                                  */
/*                       Supporting Functions                       */
/*                                                                  */
/* **************************************************************** */


// Input parser function
// Copyright Enrico Franchi © 2005
// Used under BSD License
// http://rik0.altervista.org/snippets/parse_args.html


void parseInput(char *buffer, char **args, size_t args_size, size_t *numArgs)
{
    char *arg_buffer[args_size];
    char **charPointer;
    char *workingBuffer;
    size_t i, j;
    
    workingBuffer = buffer;
    arg_buffer[0] = buffer;
    args[0] = buffer;
    
    // Break the input buffer into an array, including NULLs
    for (charPointer = arg_buffer; (*charPointer = strsep(&workingBuffer, DELIMS)) != NULL;)
         {
             if ((*charPointer != '\0') && (++charPointer >= &arg_buffer[args_size])) break;
         }
     
    // Copy the arg buffer array into the actual args array without NULLs
    for (i = j = 0; arg_buffer[i] !=NULL; i++)
         {
            if (strlen(arg_buffer[i]) > 0)
            {
                args[j++] = arg_buffer[i];
            }
         }
         
    *numArgs = j;
    args[j] = NULL;     
}


int checkCmdList(char *cmdList[], char *input) 
{
    int i;
    int lenCmd;
    int lenInput = strlen(input);
    
    for (i=0; i <=sizeof(cmdList); i++) {
        lenCmd = strlen(cmdList[i]);
        if (!strncmp(cmdList[i], input, lenCmd) && (lenCmd == lenInput)) return 0;
    }
    return 1;
}





