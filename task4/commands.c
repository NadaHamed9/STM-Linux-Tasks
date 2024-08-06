#include "commands.h"
char **argList; 


/******* exit command function**********/
void exit_command ()
{
          printf("GoodBye<3\n");
}
/***********internal pwd command function ********/
void pwd_command()
{
          char buff[100];
          getcwd(buff, 100);
          printf("\n");
          printf("%s\n",buff);
}
/********* help command function*******/
void help_command()
{
          printf("\n");
          printf("Supported internal commands are:\n");
          printf("1- mycp:Copy file to another file.\n");
          printf("2- mymv:Move file to another place.\n");
          printf("3- mypwd:Print working directory.\n");
          printf("4- myecho:Print a user input string on stdout\n");
          printf("5- exit:Terminate the shell.\n");
	  printf("6- cd:change the current directory.\n");
	  printf("7- type:return the type of the command.\n");
	  printf("8- envir:print all enviroment variables.\n");
}
/********internal echo command function********/
void echo_command(char *args[])
{
         printf("\n");
        for (int i=1;args[i]!=NULL;i++)
       { 
          printf("%s ",args[i]);
       }
}

/*********internal copy and move command function********/
void copy_move_command(char*args[])
{
    /*if user didnt give filename of targetpath use file name of src path*/
    char*src_filename=basename(strdup(args[1]));

    struct stat dest_stat;

    int is_dir=(stat(args[2],&dest_stat)==0 && S_ISDIR(dest_stat.st_mode));
    int ends_with_sepr=args[2][strlen(args[2])-1]=='/';

    char final_dest[1024];

   /***construct final target path***/

    if(is_dir || ends_with_sepr)
    /*put file name in src path in target path*/
    snprintf(final_dest, sizeof(final_dest),"%s/%s",args[2],src_filename);

    else/*leave target path as it is*/
    snprintf(final_dest, sizeof(final_dest),"%s",args[2]);

     /*to check if source path is the same as target path*/
     if (strcmp(args[1],args[2])==0) 
           fprintf(stderr, "Target file is already exists\n");
           
	   else{

       int  fd_in =open(args[1], O_RDONLY);

       /* to check it got source file path */
       if (fd_in<0)
       perror("open error in source file:");
       else{
       int fd_out =open(final_dest, O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);

       /* to check it got target file path*/
       if (fd_out<0)
       perror("open error in dest file:");
       else{
       char buff[100];
       int size =0;

        do
        {
                size = read(fd_in,buff,100);
                write(fd_out, buff, size);


        } while(size==100);
        close (fd_in);
        close (fd_out);

       /* if its move command delete file in src address*/
       if(strcmp(args[0],"mymv")==0)
        {
          if(remove(args[1])==0)

	  printf("moved successfully:) \n");
        }   
}}}}

/***************** cd command function*************/
void cd_command(const char*path)
{
      
	static char prevDir[1024]="";
	char currDir[1024];
	char newPath[1024];

       /*get current directory*/
	if(getcwd(currDir,sizeof(currDir))==NULL)
	{
		perror("getcwd");
		return;
	}
	/*check if path is empty , put it default to home directory*/
        if(path==NULL || strcmp(path,"")==0 )
		path="~";

	/*change to prev dir*/
	if(path!=NULL && strcmp(path,"-")==0)
	{       
		if(prevDir[0]=='\0')
		{
			fprintf(stderr,"cd: oldpwd not set\n");
		}
		else if(chdir(prevDir)==0){
//                printf("directory changed to prev dir %s\n",prevDir);
		/*update prev dir to the current dir*/
		strncpy(prevDir,currDir,sizeof(prevDir));
		/*store it to global var to appear in pwd function and shell message*/
		strncpy(pwd,currDir,sizeof(pwd));
		}
                else
                perror("chdir");

	}
        /*if path have ~*/
	else if(path[0]=='~' )
        {
		struct passwd *pw=getpwuid(getuid());
		const char *homeDir=pw->pw_dir;
                /*path is only ~ so change to home dir*/
		if(path[1]=='\0'){

                if(chdir(homeDir)==0){
//                printf("directory changed to home dir %s\n",homeDir);
                /*update prev dir to the current dir*/
                strncpy(prevDir,currDir,sizeof(prevDir));
                }
                else
                perror("chdir");
                }

               else{/*path have ~ plus path to new dir*/
	       snprintf(newPath,sizeof(newPath),"%s%s",homeDir,path+1);
               path=newPath;
		       
               if(chdir(path)==0){
//		printf("directory changed to %s\n",path);
		strncpy(prevDir,currDir,sizeof(prevDir));

	       }
	       else
		perror("chdir");
        }}
	else {/*if path is not null,~,-*/
		if(chdir(path)==0){
//                printf("directory changed to %s\n",path);
                strncpy(prevDir,currDir,sizeof(prevDir));

               }
               else
                perror("chdir");
	}
}

/**************check if command is external function**************/
int is_external(const char* cmd)
{

	char *path=getenv("PATH");
	/*check if path enviroment var not set*/
	if(path==NULL)
	return 0;
	
	char *path_cp=strdup(path);
	char *dir=strtok(path_cp,":");
       
	while(dir){
		char fullPath[1024];
		snprintf(fullPath,sizeof(fullPath),"%s/%s",dir,cmd);
		if(access(fullPath,X_OK)==0)
		{/*this command found in path means its external*/
			free(path_cp);
			return 1;
		}

		dir =strtok(NULL,":");
	}
	/*command not found in enviroment so its unsupported*/
	free(path_cp);
	return 0;
	}

/************** type command function ***********/
void type_command(char *args[])
{ /*if user enterred type without command */
	if(args[1]==NULL)
	   fprintf(stderr, "please enter it in form type (command)\n");
        
	else{
        int flag=0;
        char *internal_commands[9]={"mycp","mymv","myecho","mypwd","help",                   "exit", "type","cd","envir"};
        for(int i=0;i<9;i++)
        {      /*internal command*/
                if(strcmp(args[1],internal_commands[i])==0)
                { 
                 flag=1;
                 printf("this is internal command\n");
                }
        }
       /* flag=0 means that maybe external or unsupported command*/        
        if(flag==0)
         {
          int state =is_external(args[1]);
	  if(state==1)
		  printf("this is external command\n");
	  else
		  printf("this is unsupported command\n");
	 }

	}
}

/************ envir command function*************/
void envir_command()
{
/* print enviroment variables */
    int i=0;
    while(environ[i]!= NULL)
    {
	    printf("%s\n",environ[i++]);
    }   
}

/********* child handle function**********/
void child( )
{       /*dynamically allocates memory for argList*/
	argList=malloc((argIndex)*sizeof(char* ));
        /*if theres failure in malloc*/
        if(argList==NULL)
	perror("malloc");
	
	for (int i=0;i<argIndex;i++)
	{/*storing tokenised commands in argList*/
		argList[i]=args[i];
	}

       /*terminate argList by NULL*/	
	argList[argIndex]=NULL;
        execvpe(args[0], argList,environ);

        perror("execvpe failed");
     /*in case of non supported command or execvpe fails*/
        exit(EXIT_FAILURE);

}
/*********** external command function**********/
void  external_command( )
{
int retpid =fork();
/***************parent**************/
        if(retpid>0);
        {
        int status =0;
	/*wait for child process to finish*/
        wait(&status);

        if(WIFEXITED(status))
        {
        int exit_status=WEXITSTATUS(status);
      /*if status =0 means succefully exit other than 0 means fail*/
        if (exit_status!=0)
        {
           printf("sorry thats is wrong/unsupported command\n");
        } //return exit_status;

        }else if (WIFSIGNALED(status))
        {
	   printf("child terminated by signal%d\n",WTERMSIG(status));
        }
        else printf("child terminated abnormally\n");
	free(argList);
        }

/******************child****************/
        if(retpid==0)
        {
        child();
        }
/*in case failure in fork */
        if(retpid<0)
        perror("fork");
}

/****************** pipe command function **********/
void pipe_command(char **argList1,char **argList2)
{
     
        int pipefd[2];
	int retpid1;
	int retpid2;

	if(pipe(pipefd)==-1)
		perror("pipe");

        retpid1=fork();

	if(retpid1==-1)
		perror("fork");

        /*child1 process*/
	if(retpid1==0)
	{
		close(pipefd[0]);
		dup2(pipefd[1],STDOUT_FILENO);
		close(pipefd[1]);

		execvpe(argList1[0],argList1,environ);
		perror("execvpe");
	}
	/*parent1 process*/
	else 
        {
                retpid2=fork();

		if(retpid2==-1)
		perror("fork");

		/*child2 process*/
		if(retpid2==0){

                close(pipefd[1]);
                dup2(pipefd[0],STDIN_FILENO);
                close(pipefd[0]);

                execvpe(argList2[0],argList2,environ);
                perror("execvpe");
		}
        
	else{/*parent2 process*/
		close(pipefd[0]);
		close(pipefd[1]);

		/*wait for child1 to finish*/
		if(waitpid(retpid1,NULL,0)==-1)
		{
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
		/*wait for child2 to finish*/
		if(waitpid(retpid2,NULL,0)==-1)
                {
                        perror("waitpid");
                        exit(EXIT_FAILURE);
                }

	}

}}

