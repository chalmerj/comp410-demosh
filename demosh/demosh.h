//
//  demosh.h
//  demosh
//
//  Created by Jeremy Chalmer on 9/20/11.
//  Copyright Jeremy Chalmer. All rights reserved.
//
//  Help found from these sources:
//  http://stackoverflow.com/questions/7150755/help-implementing-gnu-readline-in-c (user: rahmu)

#define MAX_BUFFER 256
#define ARR_SIZE 256
#define DELIMS " \t\r\n"

/* 
 * The demo shell has a fixed number of commands and no environment.
 * I pre-define the 'Command Code' of each command, to enable use of a 'switch'
 * statement in the main loop. The built-in commands are assembled into a 
 * struct containing their name and code.
 */

#define CMD_EXIT 100
#define CMD_SLEEP 1
#define CMD_WAIT 2
#define CMD_WAITLIST 3
#define CMD_TIME 4
#define CMD_KILL 5

// How many built-in commands are there?
#define NUM_CMDS 6


typedef struct builtins_s {
    char cmdName[MAX_BUFFER];
    int cmdCode;
}Builtins_t;

/*
 * Each user entry is stored in this struct. The struct holds three items:
 * -- The string name of the command (also in argv[0]);
 * -- An int count of each word of command input (size of argv);
 * -- An array of strings, containing each word of input.
 */

typedef struct command_s {
    char *command;
    int argc;
    char *argv[MAX_BUFFER];
    int cmdCode;
}Command_t;


//Prototypes

/* 
 * parseInput uses strsep() to parse the input line into an argument vector.
 * The first item (argv[0]) is the actual name of the command. 
 * The name of the command is passed to getCommandCode which returns the int value
 * of the command, or 0 if the command is not found.
 */

void parseInput(char *input, Command_t *cmds);
int getCommandCode(char *commandName);