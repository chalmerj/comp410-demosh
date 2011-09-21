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


int main (int argc, const char * argv[])
{
    char buffer[MAX_BUFFER];
    Command_t cmd = {NULL, 0, {NULL}};
       
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
        printf("Command Code:%d\n",cmd.cmdCode);

    }
}








