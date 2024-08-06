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
#include <pwd.h>
#include <sys/wait.h>

#define STDIN  0
#define STDOUT 1
#define STDERR 2

/*************** global var ********************/
extern char *args[10];
extern char **environ;
extern int  argIndex;
extern char **argList;
extern char **argList1;
extern char **argList2;
extern char pwd[1024];

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

/*
*paramters (in):const char*
*paramters (out):void
*description:takes path to change directory to it 
*/
void cd_command(const char* path);

/*
*paramters (in):char*
*paramters (out):void
*description:returns type of the command if its internal or external
*or unsupported.
*/
void type_command(char*args[]);

/*
*paramters (in):void
*paramters (out):void
*description:prints enviorment variables by looping in environ var
*/
void envir_command(void);

/*
*paramters (in):void
*paramters (out):void
*description:function called in child process to execvpe the given non internal
*commands and do it if its in the path enviroment var if not it exit with status
*not equal zero 
*/
void child(void);

/*
*paramters (in):void
*paramters (out):void
*description:if enterred command is not internal this function do fork where
*parent waits for exit status of child and child do execvpe to preform this 
*task through calling child function
*/
void  external_command(void);

/*
*paramters (in):const char*
*paramters (out):int
*description: takes the command to check if its external by searching in
*enviroment paths and variables and return 1 if its external and 0 if its not
*which will means that is unsupported command.
*/
int is_external(const char* cmd);

/*
*paramters (in):char ** ,char**
*paramters (out):void
*description:if enterred command contains pipe it handels the pipe function
*as it takes argList for two commands that i want to pipe.
*/

void pipe_command(char **argList1,char **argList2);


#endif
