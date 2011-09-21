//
//  demosh.h
//  demosh
//
//  Created by Jeremy Chalmer on 9/20/11.
//  Copyright Jeremy Chalmer. All rights reserved.
//
#define BUFFER_SIZE 256
#define ARR_SIZE 256
#define DELIMS " \t\r\n"

//Prototypes
int checkCmdList(char *cmdList[], char *input);     // Iterates through the command list, returning 0 if 'input' is in the list
void parseInput(char *buffer, char **args, size_t args_size, size_t *numArgs);  //Uses strsep() to parse the buffer into an argument vector