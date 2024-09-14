#include"function.h"

char *args[10];
int argIndex;
int show_all ,long_format ,sort_time ,access_time;
int create_time ,show_inode ,no_sort ,dir_only,one_col ;

static void usageError(char *progName, char *msg, int opt)
{
	if (msg != NULL && opt != 0)
		fprintf(stderr, "%s (-%c)\n", msg, printable(opt));
	fprintf(stderr, "Usage: %s [-p arg] [-x]\n", progName);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){

	int opt, xfnd;
	char *pstr;

	xfnd = 0;
	pstr = NULL;
	//flags for each option of ls
	show_all = 0,long_format = 0,sort_time = 0,access_time = 0;
	create_time = 0,show_inode = 0,no_sort = 0,dir_only = 0,one_col = 0;


	//store argv in shared array
	for(int i=0;i<argc;i++)
	{
		args[i]=argv[i];
	}
	argIndex=argc;

	//take option of ls
	while ((opt = getopt(argIndex, args, "latucifd1")) != -1) {

		switch (opt) {
			case 'l': long_format = 1;             break;
			case 'a': show_all = 1;                break;
			case 't': sort_time = 1;               break;
			case 'u': access_time = 1;             break;                        
			case 'c': create_time = 1;             break;
			case 'i': show_inode = 1;              break;
			case 'f': show_all = 1; no_sort = 1;   break;
			case 'd': dir_only = 1;                break;                        
			case '1': one_col = 1;                 break;
			case ':': usageError(args[0], "Missing argument", optopt);
			case '?': usageError(args[0], "Unrecognized option", optopt);
			default:  printf("Unexpected case in switch()"); return -1;
		}
	}

	//check if option is valid and theres no error
	if (xfnd != 0)
		printf("-x was specified (count=%d)\n", xfnd);
	if (pstr != NULL)
		printf("-p was specified with the value \"%s\"\n", pstr);
	if (optind < argIndex)
		printf("First nonoption argument is \"%s\" at argv[%d]\n",
				args[optind], optind);
	if(optind == argIndex)
		do_ls(".");

	else{
		for(int i = optind; i < argIndex;i++)
		{
			//to support multiple dir
			printf("\n%s:\n",args[i]);
			do_ls(args[i]);
		}
	}
	return 0;
}

