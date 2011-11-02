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
#include <time.h>
#include <sys/wait.h>

#include "demosh.h"


int main (int argc, const char * argv[])
{
    char buffer[MAX_BUFFER];
    Command_t cmd = {NULL, 0, {NULL}};
    Process_t *psList;
    int runningTime = 0;
    
    memset((void *)&psList, 0, sizeof(psList));
    
    psList = createProcess(psList, "Parent", NULL, getpid(), time(NULL));
        
       
    //pid_t   pid = 0;
    
    while (1){
        memset((void *) buffer, 0, sizeof(buffer));
        
        printf("$ ");
        if(!fgets(buffer, MAX_BUFFER, stdin)) break;
        
        // Trim the trailing \n from the buffer
        char *ptr;
        if((ptr = strchr(buffer, '\n')) != NULL) *ptr = '\0';
        
        //Parse the command input
        parseInput(buffer, &cmd);
        
        printf("Command: %s\nNum. Args: %d\n",cmd.command, cmd.argc);
//        printf("Command Code:%d\n",cmd.cmdCode);
        
        switch (cmd.cmdCode)
        {
            case 0:
                break;
                
            case CMD_EXIT:
                return 0;
            
            case CMD_LIST:
                list(psList);
                break;
            
            case CMD_SLEEP:
                if (!cmdSleep(psList, &cmd))
                {
                    printf("Sleep thread created\n");
                    break;
                }
                else
                {
                    printf("Sleep thread failed\n");
                    break;
                }
                
            case CMD_TIME:
                runningTime = cmdTime(psList,&cmd);
                if (0 < runningTime)
                {
                    printf("PID %s running for %ds.\n",
                           cmd.argv[1],runningTime); 
                }
                else
                {
                    printf("Time Error\n");
                }
                break;
            
            case CMD_HELP:
                printf("List of Commands:\n");
                printf("list - list the running threads.\n");
                printf("sleep n - create a new sleeping thread for 'n' seconds.\n");
                printf("time PID - give the current running time for thread PID.\n");
                printf("wait PID - block execution until thread PID terminates.\n");
                printf("wait-list PID PID ... - block execution until each PID given terminates.\n");
                printf("kill PID - terminates thread PID.\n");
                printf("exit - exit the program.\n");
                printf("help  - print this help.\n");
                break;
                
                
            default:
                break;
                    
        }

    }
}








