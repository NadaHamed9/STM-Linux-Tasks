#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define EXTRA_SIZE     (1024*1024)               // 1MB 
#define METADATA_SIZE  (sizeof(Block))          // Metadata size
#define DEBUGGING      0                       //turn off debugging functions

/*Metadata structure for a memory block.*/
typedef struct Block {
    size_t size;
    struct Block *next;
    struct Block *prev;
    int free;
} Block;


/****************** Functions prototype ****************/

/*
*paramters (in):size_t
*paramters (out):Block*
*description: this function search for first fit block instead of allocating new area. 
*/
Block *find_free_block(size_t size);

/*
*paramters (in):void
*paramters (out):void
*description: this function prints free list with its metadata it used for debugging.
*/
void print_freelist(void);

/*
*paramters (in):Block*,size_t
*paramters (out):void
*description: this function in case of using free block and it exceeds needed size
*it split block for needed size and extra size for next alloc.
*/
void split_block(Block *block, size_t size);

/*
*paramters (in):void
*paramters (out):void
*description: this function merge two adjacent free blocks in one block.
*/
void merge_blocks(void);

/*
*paramters (in):void*
*paramters (out):void
*description: this function free dynamically allocated area in sim heap and
*update metadata and cleanup free list.
*/
void HmmFree (void* ptr);

/*
*paramters (in):size_t
*paramters (out):void*
*description: this function allocates memory in sim heap but first check if theres free block
*suitable for allocation size and if there check if need to split it or not.
*/
void* HmmAlloc(size_t size);

/*
*paramters (in):void
*paramters (out):void
*description: this function is called each time when HmmAlloc is called as it allocates extra space 1kb
*and mark it as free so next time of allocation take space from this area if suitable and no need to 
*increment program break again.
*/
void alloc_extra(void);

/*
*paramters (in):void
*paramters (out):void
*description: this function used to cleaan free list from used blocks to make search
*for freed blocks faster and calls remove_from_freelist if found block is used.
*/
void cleanup_freelist(void);

/*
*paramters (in):Block*
*paramters (out):void
*description: this function used to remove block from free list.
*/
void remove_from_freelist(Block* block);

/*
*paramters (in):Block*
*paramters (out):void
*description: this function is called by HmmFree to insert free block in free list .
*/
void insert_free_block(Block* free_block);

/*
*paramters (in):size_t
*paramters (out):void*
*description: this function is wrapper for HmmAlloc and named as libc function to use
*my lib instead of libc.
*/
void *malloc(size_t size);

/*
*paramters (in):void*
*paramters (out):void
*description: this function is wrapper for HmmFree and named as libc function to use
*my lib instead of libc.
*/
void free(void *ptr);

/*
*paramters (in):size_t
*paramters (out):void*
*description: this function is wrapper for HmmCalloc to allocates area for array 
*and named as libc function to use my lib instead of libc.
*/
void *calloc(size_t nmemb, size_t size);

/*
*paramters (in):size_t , void*
*paramters (out):void*
*description: this function is wrapper for HmmRealloc to reallocates area 
*and named as libc function to use my lib instead of libc.
*/
void *realloc(void *ptr, size_t size);

/*
*paramters (in):size_t
*paramters (out):size_t
*description: this function is used to allign adresses and sizes to 8bytes
*to avoid fragmentation and enhace memory perfomnace.
*/
size_t allign_size(size_t size);


#endif
