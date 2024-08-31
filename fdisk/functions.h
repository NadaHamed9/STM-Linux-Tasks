#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

#define STDIN  0
#define STDOUT 1
#define STDERR 2
/****************************** Global variabels ****************************/
typedef struct {
    uint8_t status;
    uint8_t first_chs[3];
    uint8_t partition_type;
    uint8_t last_chs[3];
    uint32_t lba;//starting sector
    uint32_t sector_count;
} PartitionEntry;

extern char*args;
extern int fd;

/************************** Functions prototye ****************************/

/*
*paramters (in):PartitionEntry*
*paramters (out):void
*description: this function display partition tables in MBR data such as start size,end size,
*number of sectors,size of partition,ID and type(linux or extended) and if its extended it
*calls read_logical_partitions(). 
*/
void read_partition_table(PartitionEntry *table_entry_ptr);


/*
*paramters (in):PartitionEntry*, int
*paramters (out):void
*description: this function display logical partition tables data such as start size,end size,
*number of sectors,size of partition,ID and type(linux) and checks for next EBR.
*/
void read_logical_partitions(PartitionEntry *table_entry_ptr,int index);

#endif
