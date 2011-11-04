//
//  demosh.c
//  demosh
//
//  Created by Jeremy Chalmer on 9/20/11.
//  Copyright 2011 Jeremy Chalmer. All rights reserved.
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

/*
 * Definition of the global builtins_t struct. Follows the #defines
 * in the header file. 
 */

Builtins_t cmdList[NUM_CMDS] = {
    {"exit", CMD_EXIT},
    {"sleep", CMD_SLEEP},
    {"list", CMD_LIST},
    {"wait", CMD_WAIT},
    {"wait-list", CMD_WAITLIST},
    {"time", CMD_TIME},
    {"kill", CMD_KILL},
    {"help", CMD_HELP}
};

/* 
 * This parser takes an input string and fills a command_t struct.
 * It uses strsep instead of strtok, as strtok_r is missing on Minix.
 * 
*/

void parseInput(char *input, Command_t *cmds)
{
    char *buffer;
    char *token;
    int count = 0;
    
    if (NULL != input) buffer = strdup(input);
    
    while ((token = strsep(&buffer, DELIMS)))
    {
        cmds->argv[count] = token;
        count++;
    }
    
    cmds->argc = count;
    cmds->argv[count] = NULL;
    cmds->command = cmds->argv[0];
    cmds->cmdCode = getCommandCode(cmds->command);
    
}

/*
 * This gets the command code associated with the command name
 * based on the Builtins struct it's passed. Returns 0 if 
 * the input commandName isn't found.
 *
*/

int getCommandCode(char *commandName)
{
    int i = 0;
    for (i=0; i <= NUM_CMDS; i++)
    {         
        if  (!strcmp(cmdList[i].cmdName, commandName))
        {
            return cmdList[i].cmdCode;
        }
    }
    return 0;
}

/*
 * createProcess creates a new process list node, 
 * returning a pointer to that node.
 *
 */

Process_t *createProcess(Process_t *root, char *command, int argc, 
                         char *argv[], pid_t pid, time_t start, int status)
{
    Process_t  *temp;
    
    if (NULL == root)
    {
        root = (Process_t *) malloc(sizeof(Process_t));
        if (NULL == root) exit(0);
        
        root->next = NULL;
        root->command = command;
        memset(root->argv, 0, sizeof(root->argv));
        root->pid = pid;
        root->start = start;
        root->completed = 0;
        root->status = 0;
        
    }
    else
    {
        temp = root;
        while (temp->next) temp = temp->next;
        
        temp->next = (Process_t *) malloc(sizeof(Process_t));
        if (NULL == temp->next) exit(0);
        
        temp = temp->next;
        temp->command = command;
        memcpy(temp->argv, argv, ((size_t)argc*(sizeof(char*))));
        temp->pid = pid;
        temp->start = start;
        temp->completed = 0;
        temp->status = 0;
        
    }
    return root;
}

/*
 * Prints a list of all processes.
 *
 */

void printList(Process_t *root)
{
    Process_t *current;
    current = (NULL != root)?root:NULL;
    
    printf("Command\t\tArgument\tPID\t\tStart\t\tStatus\n");
    
    while(current)
    {
        printf("%s\t\t",current->command);
        
        if (NULL != current->argv[1])
            printf("%-6s\t\t",current->argv[1]);
        else
            printf("None\t\t");
        
        printf("%i\t",current->pid);
        printf("%ld\t",current->start);
        printf("%i\n",current->status);
        
        current = current->next;
        
    }
    
}


/*
 * cmdSleep (named such to avoid overloading system sleep()) 
 * checks for valid input, forks a thread, sends it to sleep,
 * and the Parent thread creates a new process in the process list.
 *
 * It returns 0 if successful, 1 if something went wrong.
 */




int cmdSleep(Process_t *psList, Command_t *cmd)
{
    int pid;
    time_t startTime;
    
    if ((cmd->argc <= 1) || (!isdigit(*cmd->argv[1])))
    {
        printf("Incorrect Args for Sleep Command - 'sleep int'\n");
        return 1;
    } 
    
    else if ((cmd->argc > 1) && (isdigit(*cmd->argv[1])))
    {
        // Fork the process
        if ((pid = fork()) < 0) 
        {
            printf("Failed to fork process\n");
            return 1;
        }
        //We're the parent process
        if (pid > 0)
        {
            int status;
            waitpid(pid, &status, WNOHANG);
            time(&startTime);
            //printf("Sleep Arg %s\n",cmd->argv[1]);            
            createProcess(psList, cmd->command,cmd->argc, cmd->argv, pid,startTime,status );
            
            return 0;
        }
        
        // We're the child process
        
        else if (0 == pid)
        {
            int childPID = getpid();
            long sleepTime = strtol(cmd->argv[1], NULL, 10);
            printf("\nChild %i sleeping for %lds.\n",childPID,sleepTime);
            sleep(sleepTime);
            return 0;
        }

        
        else
        {
            printf("Error!");
            return 1;
        }
    }
    
    return 0;
}

/*
 * The cmdTime process (named to avoid overloading system time())
 * takes in the root of the process list and the given command,
 * parses the argument for a PID and searches the process list.
 * If the PID is found, it compares the start time of the process
 * with current time, and returns that value. It returns 0 if the
 * PID is not found, -1 if error is reached.
 *
 */


int cmdTime(Process_t *psList, Command_t *cmd)
{
    time_t now;
    Process_t *current;
    int pid;
        
    if ((cmd->argc <= 1) || (!isdigit(*cmd->argv[1])))
    {
        printf("Incorrect Args for Time Command - 'time PID'\n");
        return -1;
    } 
    else if ((cmd->argc > 1) && (isdigit(*cmd->argv[1])))
    {
        pid = atol(cmd->argv[1]);
        
        if ((pid > 0) && (NULL != psList))
        {
            for (current = psList; current; current=current->next)
            {
                if (pid == current->pid)
                {
                    time(&now);
                    return (int)(now-current->start);
                }

            }
            printf("PID %d not found.\n",pid);
            return 0;
        }
    }
    return -1;
}




