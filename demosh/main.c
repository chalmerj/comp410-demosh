//
//  main.c
//  demosh
//
//  Created by Jeremy Chalmer on 9/19/11.
//  Copyright 2011 Jeremy Chalmer. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFFER_SIZE 1 << 8 //1 Bitwise shift left 8  = 256 

//Prototypes
int checkCmdList(char *cmdList[], char *input);

char *cmdList[] = {
    "sleep",
    "wait",
    "wait-list",
    "time",
    "kill"
};

int main (int argc, const char * argv[])
{
    char buffer[BUFFER_SIZE];
    
    
    while (1){
        memset((void *) buffer, 0, sizeof(buffer));
        printf("$ ");
        if (!fgets(buffer, BUFFER_SIZE, stdin)) break;
        
        // Trim the trailing \n from the buffer
        char *ptr;
        if((ptr = strchr(buffer, '\n')) != NULL) *ptr = '\0';
        
        
        printf("You entered: %s\n", buffer);
        
        if ((checkCmdList(cmdList, buffer) == 0)) printf("Command Found!\n");
        
    }
}

int checkCmdList(char *cmdList[], char *input) {
    int i;
    for (i=0; i <=sizeof(cmdList); i++) {
        if (!strncmp(cmdList[i], input, strlen(cmdList[i])) && (strlen(cmdList[i]) == strlen(input))) return 0;
    }
    return 1;
}
