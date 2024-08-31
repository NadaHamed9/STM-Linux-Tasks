#include"functions.h"

void read_partition_table(PartitionEntry *table_entry_ptr)
{
	for (int i = 0; i < 4; i++) {

                /*check if its Linux file system*/
                if(table_entry_ptr[i].partition_type==0x83 ||table_entry_ptr[i].partition_type==0x93
                                ||table_entry_ptr[i].partition_type==0x43)
                {

                        printf("%s%-5d %-10c %-10u %-10u %-10u %uG      %-10X %s\n",
                                        args,i + 1,
                                        table_entry_ptr[i].status == 0x80 ? '*' : ' ',
                                        table_entry_ptr[i].lba,
                                        table_entry_ptr[i].lba + table_entry_ptr[i].sector_count - 1,
                                        table_entry_ptr[i].sector_count,
                                        (uint32_t) (((uint64_t) table_entry_ptr[i].sector_count *512)
                                                / (1024 * 1024 * 1024)),
                                        table_entry_ptr[i].partition_type,"Linux");
                }

                /*check if its extended*/
                else if(table_entry_ptr[i].partition_type ==0x05||table_entry_ptr[i].partition_type==0x0F
                                ||table_entry_ptr[i].partition_type==0x85
                                ||table_entry_ptr[i].partition_type==0xC5
                                ||table_entry_ptr[i].partition_type==0xD5)
                {

                        printf("%s%-5d %-10c %-10u %-10u %-10u %uG      %-10X %s\n",
                                        args,i + 1,
                                        table_entry_ptr[i].status == 0x80 ? '*' : ' ',
                                        table_entry_ptr[i].lba,
                                        table_entry_ptr[i].lba + table_entry_ptr[i].sector_count - 1,
                                        table_entry_ptr[i].sector_count,
                                        (uint32_t) (((uint64_t) table_entry_ptr[i].sector_count *512)
                                                / (1024 * 1024 * 1024)),
                                        table_entry_ptr[i].partition_type,"Extended");

			/*check for logical partitions where 1st EBR at address of lba of
                         * extended partition*/
			read_logical_partitions(table_entry_ptr,i);
		}
	}
}

void read_logical_partitions(PartitionEntry *table_entry_ptr,int index)
{
	char buf2[512];
	/*flag indicates theres EBR or not*/
	int flag=1;
	off_t out;
	/*starting LBA of extended partition*/
	int size=table_entry_ptr[index].lba;
	/*start from partition 5*/
	int count=5;
      
	while(flag){
		
		out=lseek(fd,size*512,SEEK_SET);
		if(out==-1)
			perror("lseek error:");
		else{
			/*read file given*/
			read(fd,buf2,512);
			PartitionEntry *logical_entry_ptr = (PartitionEntry *) &buf2[446];
			/*start from partition 5*/

			/*check if its Linux file system*/
			if(logical_entry_ptr[0].partition_type==0x83 ||logical_entry_ptr[0].partition_type==0x93
                                ||logical_entry_ptr[0].partition_type==0x43)
			{
				printf("%s%-5d %-10c %-10u %-10u %-10u %uM    %-10X %s\n",
						args,count,
						logical_entry_ptr[0].status == 0x80 ? '*' : ' ',
                                                logical_entry_ptr[0].lba+size,
                                                logical_entry_ptr[0].lba+size+logical_entry_ptr[0].sector_count - 1,
                                                logical_entry_ptr[0].sector_count,
                                                (uint32_t) (((uint64_t) logical_entry_ptr[0].sector_count *512)
							/ ( 1024 * 1024)),
						logical_entry_ptr[0].partition_type,"Linux");
			}

			/*update acummlative size*/
			size=logical_entry_ptr[1].lba +table_entry_ptr[index].lba;
			
			if(logical_entry_ptr[1].lba ==0)
			{
				
				/*that was last logical partition no more*/
				flag=0;
			}
			count++;
		}
	}
}
