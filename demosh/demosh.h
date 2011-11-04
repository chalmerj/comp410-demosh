//
//  demosh.h
//  demosh
//
//  Created by Jeremy Chalmer on 9/20/11.
//  Copyright Jeremy Chalmer. All rights reserved.
//
//  Help found from these sources:
//  http://stackoverflow.com/questions/7150755/help-implementing-gnu-readline-in-c (user: rahmu)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_BUFFER 256
#define MAX_ARGS 32
#define DELIMS " \t\r\n"

/* 
 * The demo shell has a fixed number of commands and no environment variables.
 * I pre-define the 'Command Code' of each command, to enable use of a 'switch'
 * statement in the main loop. The built-in commands are assembled into a 
 * struct containing their name and code.
 */

#define CMD_EXIT 100
#define CMD_SLEEP 1
#define CMD_LIST 2
#define CMD_WAIT 3
#define CMD_WAITLIST 4
#define CMD_TIME 5
#define CMD_KILL 6
#define CMD_HELP 7

// How many built-in commands are there?
#define NUM_CMDS 8


/* The global struct containing the built-in commands and codes for demosh. */

typedef struct builtins_s {
    char cmdName[MAX_BUFFER];
    int cmdCode;
}Builtins_t;

/*
 * The command_t stuct: 
 * Each user entry is stored in this struct. The struct holds three items:
 * -- The string name of the command (also in argv[0]);
 * -- An int count of each word of command input (size of argv);
 * -- An array of strings, containing each word of input.
 */

typedef struct command_s {
    char *command;
    int argc;
    char *argv[MAX_ARGS];
    int cmdCode;
}Command_t;

/*
 * Data about each process forked from the parent is stored
 * in a process struct. 
 */

typedef struct process {
    struct process *next;   // Pointer to next process in the list.
    char *command;          // Command string used to start the process
    int argc;
    char *argv[MAX_ARGS];   // Argument Vector used for exec-ing the process
    pid_t pid;              // Process ID
    time_t start;           // Timestamp (in seconds) of process start
    char completed;         // True if process has completed
    int status;             // Reported status value
}Process_t;




//Prototypes

/* 
 * parseInput uses strsep() to parse the input line into an argument vector.
 * The first item (argv[0]) is the actual name of the command. 
 * The name of the command is passed to getCommandCode which returns 
 * the int value of the command, or 0 if the command is not found.
 */

void parseInput(char *input, Command_t *cmds);

int getCommandCode(char *commandName);

int cmdSleep(Process_t *psList, Command_t *cmd);

Process_t *createProcess(Process_t *root, char *command,int argc,
                         char *argv[], pid_t pid, time_t start, int status);

void printList(Process_t *root);

int cmdTime(Process_t * psList, Command_t *cmd);
