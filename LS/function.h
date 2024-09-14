#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>

/*************** Macros and Defined var ********************/

#define RESET_COLOR "\033[0m"
#define DIR_COLOR "\033[1;34m"
#define EXE_COLOR "\033[1;32m"
#define SYMLINK_COLOR "\033[1;36m"
#define STDIN  0
#define STDOUT 1
#define STDERR 2
#define printable(ch) (isprint((unsigned char) ch) ? ch : '#')

/********************** global var ********************/
extern int  argIndex;
extern char *args[10];
extern int errno;
/*flags for each option of ls*/
extern int show_all ,long_format ,sort_time ,access_time;
extern int create_time ,show_inode ,no_sort ,dir_only , one_col;

/***************** Functions prototype ****************/

/*
 *paramters (in):char*
 *paramters (out):void
 *description: this function show stat info if its -l option.
 */
void show_stat_info(char *fname);

/*
 *paramters (in):char*
 *paramters (out):void
 *description: this function manage ls command and its options.
 */
void do_ls(char * dir);

/*
 *paramters (in):char*
 *paramters (out):void
 *description: this function display permission r w x s t of each file.
 */
void permission(char* fname);

/*
 *paramters (in):char*
 *paramters (out):void
 *description: this function shows type of the file.
 */
void type(char* fname);

/*
 *paramters (in):char*
 *paramters (out):void
 *description: this function display name of group.
 */
void group_id(char* fname);

/*
 *paramters (in):char*
 *paramters (out):void
 *description: this function display name of user.
 */
void user_id(char* fname);

/*
 *paramters (in):struct stat
 *paramters (out):const char*
 *description: this Function to get color based on file type
 */
const char *get_color(struct stat *info);

/*
 *paramters (in):char*
 *paramters (out):void
 *description: this function display total number of 512b blocks of files
 * and it calls if -l option.
 */
void total_blocks(char* dir);

#endif
