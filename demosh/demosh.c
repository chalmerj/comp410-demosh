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
#include "demosh.h"


/* A global struct containing the built-in commands and codes for demosh. */

Builtins_t cmdList[NUM_CMDS] = {
    {"exit", CMD_EXIT},
    {"sleep", CMD_SLEEP},
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