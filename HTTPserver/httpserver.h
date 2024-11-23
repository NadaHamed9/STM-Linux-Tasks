#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <sys/wait.h>  


#define BUF_SIZE 1024
#define BACKLOG 2
#define portnum 8080

/****************** Functions prototype ****************/

/*
*paramters (in):int
*paramters (out):void
*description: this function takes request from server and upon it it calls appropriate function
*according to the request. 
*/
void handle_request(int cfd);

/*
*paramters (in):int, const char* 
*paramters (out):void
*description:function to send HTTP response after doing request .
*/
void send_response(int cfd, const char *status, const char *content_type, const char *body);

/*
*paramters (in):int,const char*
*paramters (out):void
*description:if the request is regular file or .cgi file it opens it if it's regular
*and execute it by calling execute_cgi() function .
*/
void send_file(int cfd, const char *path);

/*
*paramters (in):int,const char*
*paramters (out):void
*description: this function is called if request is dir to cat it's contents.
*/
void list_directory(int cfd, const char *path);

/*
*paramters (in):int , const char*
*paramters (out):void
*description: this function is called to execute .cgi file.
*/
void execute_cgi(int cfd, const char *path);
