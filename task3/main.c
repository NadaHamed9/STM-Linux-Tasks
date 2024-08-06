#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"

#define STDIN  0
#define STDOUT 1
#define STDERR 2

char *args[10];

int main(int argc , char** argv)
{
	char command[100];
        char *token;
	ssize_t readSize=0;
	const char * shellMsg="Marhaba what do you want:) >";

         while(1) {

        printf("\n");
	write(STDOUT, shellMsg , strlen(shellMsg));
	readSize=read(STDIN,command,100);

        command[readSize-1]='\0';

	token=strtok(command," ");

       /*when enter is pressed*/
        while(token==NULL)
      {
        printf("\n");
        write(STDOUT, shellMsg , strlen(shellMsg));
        readSize=read(STDIN,command,100);

        command[readSize-1]='\0';

        token=strtok(command," ");

      }
        int argIndex=0;

        while(token !=NULL && argIndex<10)
	{     args[argIndex++]=token;
	      token = strtok(NULL," ");
	}
        args[argIndex]=NULL;
/********** echo command *************/
          if(strcmp(args[0],"echo")==0)
        {
            echo_command(args);
        }
/********** copy command *************/
        else if(strcmp(args[0],"cp")==0)
       {/*to check if source and target file is passed or not*/
         if (argIndex < 3) {
           fprintf(stderr, "Usage: %s <str1> <str2> [<len>]\n", args[0]);
           break;
       }
      copy_move_command(args);
     }
/********** move command *************/
        else if(strcmp(args[0],"mv")==0)
       {/*to check if source and target file is passed  or not*/
         if (argIndex < 3) {
           fprintf(stderr, "Usage: %s <str1> <str2> [<len>]\n", args[0]);
           break;
       }

      copy_move_command(args);

     }

/********* exit command *********/
        else if(strcmp(args[0],"exit")==0)
	{
         exit_command();
	 return 0;
	}

/*********** pwd command *********/
         else if(strcmp(args[0],"pwd")==0)
        {
          pwd_command();
	}

/************ help command ********/
         else if(strcmp(args[0],"help")==0)
        {

          help_command();

        }
/*********** unsupported command ********/
       else
       {
        printf("this command is unsupported:(\n");
        help_command();
       }

/* to empty buffer from enterred command*/
	for(int i=0;i<readSize;i++)
	{
		command[i]='\0';
	}
}

}

