#include "functions.h"

char* args;
int fd;
int main(int argc, char **argv)
{
	char buf1[512];
	char* token;
	int size=0;
	
	fd = open(argv[1], O_RDONLY);

	if(fd<2)
		perror("open error");

	else{
		args=argv[1];

		/*read file given */
		read(fd,buf1,512);
		PartitionEntry *table_entry_ptr = (PartitionEntry *) & buf1[446];
		
		printf("%-10s %-13s %-10s %-10s %-10s %-7s %-10s %-10s\n", "Device",
				"Boot", "Start", "End", "Sectors", "Size", "Id", "Type");

		read_partition_table(table_entry_ptr);
	}

	return 0;
}
