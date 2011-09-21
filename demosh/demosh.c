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


/* A global struct containing the built-in commands and codes for demosh. */

Builtins_t cmdList[NUM_CMDS] = {
    {"exit", CMD_EXIT},
    {"sleep", CMD_SLEEP},
    {"list", CMD_LIST},
    {"wait", CMD_WAIT},
    {"wait-list", CMD_WAITLIST},
    {"time", CMD_TIME},
    {"kill", CMD_KILL}
};

/* 
 * This parser takes an input string and fills a command_t struct.
 * It uses strlen instead of strtok, as strtok_r is missing on Minix.
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

Process_t *createProcess(Process_t *root, char *command, char **argv, pid_t pid, time_t start)
{
    Process_t  *temp;
    
    if (NULL == root)
    {
        root = (Process_t *) malloc(sizeof(Process_t));
        if (NULL == root) exit(0);
        
        root->next = NULL;
        root->command = command;
        root->argv = argv;
        root->pid = pid;
        root->start = start;
        root->completed = 0;
        root->status = 0;
        
    }
    else
    {
        temp = root;
        while (temp->next)temp = temp->next;
        
        temp->next = (Process_t *) malloc(sizeof(Process_t));
        if (NULL == temp->next) exit(0);
        
        temp = temp->next;
        temp->command = command;
        temp->argv = argv;
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

void list(Process_t *root)
{
    Process_t *current;
    current = (NULL != root)?root:NULL;
    
    printf("Command\t\tArgument\t\tPID\t\t\tStart\n");
    
    while(current)
    {
        printf("%s\t\t",current->command);
        
        if (NULL != current->argv)
            printf("%-8s\t\t",current->argv[1]);
        else
            printf("(null)\t\t");
        
        printf("%-6i\t\t",current->pid);
        printf("%ld\n",current->start);
        
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
        // We're the child process
        if (0 == pid)
        {
            sleep(*cmd->argv[1]);
            exit(0);
        }
        //We're the parent process
        else if (pid > 0)
        {
            time(&startTime);
            
            createProcess(psList, cmd->command, cmd->argv, pid,startTime );
        }
        
        else
        {
            printf("Error!");
            return 1;
        }
    }
    
    return 0;
}



void cmdTime(Process_t *psList, Command_t *cmd)
{
    time_t now;
    Process_t *current;
    int pid;
        
    if ((cmd->argc <= 1) || (!isdigit(*cmd->argv[1])))
    {
        printf("Incorrect Args for Time Command - 'time PID'\n");
    } 
    else if ((cmd->argc > 1) && (isdigit(*cmd->argv[1])))
    {
        pid = *cmd->argv[1];
        
        if ((pid > 0) && (NULL != psList))
        {
            for (psList = current; current; current=current->next)
            {
                if (pid == current->pid)
                {
                    time(&now);
                    printf("%d running for %lus.\n",(int)current->pid,(long)(now-current->start));
                    break;
                }
            }
            printf("PID %d not found.\n",pid);
        }
    }
}




