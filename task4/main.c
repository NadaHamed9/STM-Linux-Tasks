#include "commands.h"

#define BLUE_MSG "\033[34m"
#define GREEN_PATH "\033[32m"
#define RESET_COLOR "\033[0m"

char *args[10];
int argIndex;
char **argList1;
char **argList2;
char pwd[1024];

int main(int argc , char** argv)
{
	char command[100];
        char *token;
	ssize_t readSize=0;
	const char * shellMsg="Marhaba what do you want:) >";

         while(1) {

        printf("\n");
	/*print path in green*/
	printf("%s%s%s",GREEN_PATH,getcwd(pwd,sizeof(pwd)),RESET_COLOR);
	printf("$:");
	/*print shell msg in blue*/
	printf("%s%s%s",BLUE_MSG,shellMsg,RESET_COLOR);
	fflush(stdout);

//	write(STDOUT, shellMsg , strlen(shellMsg));
	readSize=read(STDIN,command,100);

        command[readSize-1]='\0';

	token=strtok(command," ");

       /*when enter is pressed*/
        while(token==NULL)
      {
        printf("\n");
        /*print path in green*/
        printf("%s%s%s",GREEN_PATH,getcwd(pwd,sizeof(pwd)),RESET_COLOR);
        printf("$:");
        /*print shell msg in blue*/
        printf("%s%s%s",BLUE_MSG,shellMsg,RESET_COLOR);
        fflush(stdout);

        readSize=read(STDIN,command,100);

        command[readSize-1]='\0';

        token=strtok(command," ");

      }
        argIndex=0;
        while(token !=NULL && argIndex<10)
	{     args[argIndex++]=token;
	      token = strtok(NULL," ");
	}
        args[argIndex]=NULL;

/********** internal echo command *************/
          if(strcmp(args[0],"myecho")==0)
        {
            echo_command(args);
        }
/********** internal copy command *************/
        else if(strcmp(args[0],"mycp")==0)
       {/*to check if source and target file is passed or not*/
         if (argIndex < 3)
           fprintf(stderr, "Usage: %s <str1> <str2> [<len>]\n", args[0]);
         else
           copy_move_command(args);
       }
/********** internal move command *************/
        else if(strcmp(args[0],"mymv")==0)
       {/*to check if source and target file is passed  or not*/
         if (argIndex < 3)
           fprintf(stderr, "Usage: %s <str1> <str2> [<len>]\n", args[0]);
        else
          copy_move_command(args);

       }

/********* exit command *********/
        else if(strcmp(args[0],"exit")==0)
	{
         exit_command();
	 return 0;
	}

/*********** internal pwd command *********/
         else if(strcmp(args[0],"mypwd")==0)
        {
          pwd_command();
	}
/*********** cd command *********/
         else if(strcmp(args[0],"cd")==0)
        {
	  if(argIndex<2)
	  cd_command(NULL);
          else	  
          cd_command(args[1]);
        }

/*********** type command *********/
         else if(strcmp(args[0],"type")==0)
        {
           type_command(args);
        }

/*********** envir command *********/
         else if(strcmp(args[0],"envir")==0)
        {
           envir_command();
        }


/************ help command ********/
         else if(strcmp(args[0],"help")==0)
        {

          help_command();
        }

/*********** not internal command ********/
       else
       {  int flag=0;

       for(int i=0;i<argIndex;i++)
	  {
	    /*its pipe command*/
	    if(strcmp(args[i],"|")==0)
	    {
		  

             int pipe_pos=i;
	    /*dynamically allocates memory for argList1*/
            argList1=malloc((pipe_pos)*sizeof(char* ));
           /*if theres failure in malloc*/
            if(argList1==NULL)
            perror("malloc");

            for (int i=0;i<pipe_pos;i++)
            {/*storing tokenised commands before pipe in argList*/
                argList1[i]=args[i];
            }

           /*terminate argList by NULL*/
           argList1[pipe_pos]=NULL;
	   
           /*dynamically allocates memory for argList2*/
           argList2=malloc(((argIndex-pipe_pos)-1)*sizeof(char* ));
           /*if theres failure in malloc*/
           if(argList2==NULL)
           perror("malloc");

          int initial =pipe_pos+1;
	  
          for (int i=0;i<((argIndex-pipe_pos)-1);i++)
          {/*storing tokenised commands after pipe in argList*/
                argList2[i]=args[initial];

		printf("%s ",argList2[i]);
          }

          /*terminate argList2 by NULL*/
          argList2[((argIndex-pipe_pos)-1)]=NULL;

          pipe_command(argList1,argList2);

	  flag=1;
	  free(argList1);
	  free(argList2);
	  }
	    }
	  /*not pipe command*/
	  if(flag==0) {   
          external_command();
	  }
       }

/* to empty buffer from enterred command*/
	for(int i=0;i<readSize;i++)
	{
		command[i]='\0';
	}
}

}
