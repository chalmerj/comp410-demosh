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

Process_t *createProcess(Process_t *root, char *command, char **argv, pid_t pid)
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
        root->start = time(NULL);
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
        temp->start = time(NULL);
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
    
    while(current)
    {
        printf("Command: %s\t",current->command);
        //printf("Argument: %s\t",current->argv[1]);
        printf("PID: %d\t",current->pid);
        printf("Start: %ld\n",current->start);
        printf("------------\n");
        
        current = current->next;
        
    }
    
}





