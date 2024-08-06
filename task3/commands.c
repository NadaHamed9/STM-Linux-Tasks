#include "commands.h"

/******* exit command **********/
void exit_command ()
{
          printf("GoodBye<3\n");
}
/*********** pwd command ********/
void pwd_command()
{
          char buff[100];
          getcwd(buff, 100);
          printf("\n");
          printf("%s\n",buff);
}
/********* help command *******/
void help_command()
{
          printf("\n");
          printf("Supported commands are:\n");
          printf("1- cp:Copy file to another file.\n");
          printf("2- mv:Move file to another place.\n");
          printf("3- pwd:Print working directory.\n");
          printf("4- echo:Print a user input string on stdout\n");
          printf("5- exit:Terminate the shell.\n");
}
/******** echo command ********/
void echo_command(char *args[])
{
         printf("\n");
        for (int i=1;args[i]!=NULL;i++)
       { 
          printf("%s ",args[i]);
       }
}
/********* copy and move command ********/
void copy_move_command(char*args[])
{
/*if user didnt give filename of targetpath use file name of src path*/
char*src_filename=basename(strdup(args[1]));

struct stat dest_stat;

int is_dir=(stat(args[2],&dest_stat)==0 && S_ISDIR(dest_stat.st_mode));
int ends_with_sepr=args[2][strlen(args[2])-1]=='/';

char final_dest[1024];
/*construct final target path*/
if(is_dir || ends_with_sepr)
{

snprintf(final_dest, sizeof(final_dest),"%s/%s",args[2],src_filename);
}
else{
snprintf(final_dest, sizeof(final_dest),"%s",args[2]);}

/*to check if target file exists or not*/
        if (strcmp(args[1],args[2])==0) {
           fprintf(stderr, "Target file is already exists\n");
           return;
        }

     int  fd_in =open(args[1], O_RDONLY);

/* to check it got source file path */
       if (fd_in<0)
       {
       perror("open error in source file:");
       return;
       }
     int fd_out =open(final_dest, O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);

 /* to check it got target file path*/
       if (fd_out<0)
       {
       perror("open error in dest file:");
       return;
       }

       char buff[100];
       int size =0;

        do
        {
                size = read(fd_in,buff,100);
                write(fd_out, buff, size);


        } while(size==100);
        close (fd_in);
        close (fd_out);

if(strcmp(args[0],"mv")==0)
{
if(remove(args[1])==0)
      {
	printf("moved successfully:) \n");
      }
}}
