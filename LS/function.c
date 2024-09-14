#include "function.h"

int errno;

static int cmpstringp(const void *p1, const void *p2)
{
	//Compare directory entry names 
	return strcmp((*(struct dirent **)p1)->d_name, (*(struct dirent **)p2)->d_name);
}

//function to compare modification times (used in qsort)
int compare_mtime(const void *a, const void *b) {
	const struct dirent *entryA = *(const struct dirent **)a;
	const struct dirent *entryB = *(const struct dirent **)b;

	struct stat statA, statB;
	char pathA[1024], pathB[1024];

	// Construct full paths for the files
	snprintf(pathA, sizeof(pathA), "%s/%s", ".", entryA->d_name);
	snprintf(pathB, sizeof(pathB), "%s/%s", ".", entryB->d_name);

	stat(pathA, &statA);
	stat(pathB, &statB);

	// Compare modification times
	return statB.st_mtime - statA.st_mtime;
}


//show information
void show_stat_info(char *fname){

	struct stat info;
	int rv = lstat(fname, &info);
	if (rv == -1){
		perror("stat failed");
		exit(1);
	}
	permission(fname);
	printf(" %2ld ", info.st_nlink);
	user_id(fname);
	group_id(fname);
	printf(" %8ld ", info.st_size);
	//Last modification or access time (modify for `-u`, `-c` options)
	char time_buf[20];
	strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", localtime(&info.st_mtime));
	printf("%s ", time_buf);
	//File name with color 
	printf("%s%s%s\n", get_color(&info), fname, RESET_COLOR);
}

//ls command 
void do_ls(char * dir)
{
	struct dirent ** entry;
	int n;
	n=scandir(dir,&entry,NULL, NULL);

	if(n<0)
	{
		perror("scandir");
		return ;
	}
	qsort(entry,n,sizeof(struct dirent*),cmpstringp);

	if(sort_time)
		qsort(entry, n, sizeof(struct dirent*), compare_mtime);
	else if(!no_sort)
		qsort(entry,n,sizeof(struct dirent*),cmpstringp);

	int columns = 7;  // Number of columns for table layout
	int count = 0;

	if(long_format)
		total_blocks(dir);

	for(int i = 0; i< n;i++)
	{
		//skip hidden files if flag show_all=0
		if(entry[i]->d_name[0] == '.' && !show_all)
		{
			free(entry[i]);
			continue;
		}
		if(long_format){
			//if it have l and i option
			if(show_inode)
				printf("%lu ",entry[i]->d_ino);
			//display stat info
			show_stat_info(entry[i]->d_name);
		}
		else{
			//if it have i option
			if(show_inode)
				printf("%lu ",entry[i]->d_ino);

			struct stat info;
			lstat(entry[i]->d_name, &info);
			// Print in one col if its -1 option
			if(one_col)
				printf("%s%s%s \n", get_color(&info), entry[i]->d_name, RESET_COLOR);
			//print in table format
			else{
				                                printf("%s%s%s ", get_color(&info), entry[i]->d_name, RESET_COLOR);

                                count++;
                                if (count % columns == 0)
                                        printf("\n");  // Newline after every set of columns

			}
		}
		free(entry[i]);
	}
	free(entry);
	// Ensure newline at the end if it wasn't added in the loop
	if (count % columns != 0)
		printf("\n");

}
//prints r w x permissions for this file
void permission(char* fname)
{
	struct stat buf;
	if (lstat(fname, &buf)<0){
		perror("Error in stat");
		exit(1);}

	int flag=0;
	int mode = buf.st_mode;
	char str[11];
	strcpy(str, "---------");
	//owner  permissions
	if((mode & 0000400) == 0000400) str[0] = 'r';
	if((mode & 0000200) == 0000200) str[1] = 'w';
	if((mode & 0000100) == 0000100) {str[2] = 'x'; flag=1;}
	//group permissions
	if((mode & 0000040) == 0000040) str[3] = 'r';
	if((mode & 0000020) == 0000020) str[4] = 'w';
	if((mode & 0000010) == 0000010) {str[5] = 'x'; flag=1;}
	//others  permissions
	if((mode & 0000004) == 0000004) str[6] = 'r';
	if((mode & 0000002) == 0000002) str[7] = 'w';
	if((mode & 0000001) == 0000001) {str[8] = 'x'; flag=1;}
	//special  permissions
	if((mode & 0004000) == 0004000 && flag == 1) str[2] = 's';
	if((mode & 0004000) == 0004000 && flag == 0) str[2] = 'S';
	if((mode & 0002000) == 0002000 && flag == 1) str[5] = 's';
	if((mode & 0002000) == 0002000 && flag == 0) str[5] = 'S';
	if((mode & 0001000) == 0001000) str[8] = 't';
	printf("%s", str);

}
//print type of file
void type(char* fname)
{
	struct stat buf;
	if (lstat(fname, &buf)<0){
		perror("Error in stat");
		exit(1);}
	//to print type of file 
	if ((buf.st_mode &  0170000) == 0010000)
		printf("Pipe");
	else if ((buf.st_mode &  0170000) == 0020000)
		printf(" Character Special file");
	else if ((buf.st_mode &  0170000) == 0040000)
		printf("Directory");
	else if ((buf.st_mode &  0170000) == 0060000)
		printf("Block Special file");
	else if ((buf.st_mode &  0170000) == 0100000)
		printf("Regular file");
	else if ((buf.st_mode &  0170000) == 0120000)
		printf("Soft link");
	else if ((buf.st_mode &  0170000) == 0140000)
		printf("Socket");
	else
		printf("Unknwon mode");


}
//print id and name of group
void group_id(char* fname)
{
	struct stat info;
	int rv = lstat(fname, &info);
	if (rv == -1){
		perror("stat failed");
		exit(1);
	}
	struct group * grp = getgrgid(info.st_gid);
	errno = 0;
	if (grp == NULL){
		if (errno == 0)
			printf("Record not found in /etc/group file.\n");
		else{
			perror("getgrgid failed");
			errno--;
		}
	}
	else
		printf(" %-8s ", grp->gr_name);

}

//print id and name of user
void user_id(char* fname)
{
	struct stat info;
	int rv = lstat(fname, &info);
	if (rv == -1){
		perror("stat failed");
		exit(1);
	}
	struct passwd * pwd = getpwuid(info.st_uid);

	if (pwd == NULL){
		if (errno == 0)
			printf("Record not found in passwd file.\n");
		else{
			perror("getpwuid failed");
			errno--;
		}
	}
	else
		printf(" %-8s ",pwd->pw_name);

}
//Function to get color based on file type
const char *get_color(struct stat *info) {
	if (S_ISDIR(info->st_mode)) {
		return DIR_COLOR;
	} else if (S_ISLNK(info->st_mode)) {
		return SYMLINK_COLOR;
	} else if (info->st_mode & S_IXUSR) {
		return EXE_COLOR;
	} else {
		return RESET_COLOR;
	}
}
//to print number of 512blocks
void total_blocks(char* dir)
{
	struct dirent ** entry;
	int n;
	n=scandir(dir,&entry,NULL, NULL);

	if(n<0)
	{
		perror("scandir");
		return ;
	}

	long count=0;
	for(int i = 0;i < n;i++){
		char*fname=entry[i]->d_name;
		struct stat info;
		int rv = lstat(fname, &info);
		if (rv == -1){
			perror("stat failed");
			exit(1);
		}
		//skip hidden files if flag show_all=0
		if(entry[i]->d_name[0] == '.' && !show_all)
		{
			free(entry[i]);
			continue;
		}

		count=count+info.st_blocks;
	}
	printf("total: %ld\n",count/2);
}
