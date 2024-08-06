#ifndef COMMANDS_H
#define COMMANDS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>

/*************** global var ********************/
extern char *args[10];

/*
*paramters (in):void
*paramters (out):void
*description: this function display GoodBye<3 as termination of shell 
*/
void exit_command(void);

/*
*paramters (in):void
*paramters (out):void
*description: this function prints the current working directory
*/
void pwd_command(void);

/*
*paramters (in):void
*paramters (out):void
*description: this function prints the supported commands that shell accept
*/
void help_command(void);

/*
*paramters (in):char*
*paramters (out):void
*description: this function takes that written after echo command 
*and dislpays it.
*/
void echo_command(char *args[]);

/*
*paramters (in):char*
*paramters (out):void
*description: this function performs copy and move commands as they take
*source and target paths ,and when take target path without filename it 
*uses filename in source path.
*/
void copy_move_command(char*args[]);


#endif
