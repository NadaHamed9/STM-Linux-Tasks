#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HEAP_SIZE      (1024 * 1024 * 1024)  // 1GB
#define EXTRA_SIZE     (1024)               // 1KB 
#define METADATA_SIZE  (sizeof(Block)) 

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
*description: this function for debugging where test scenario[if allocated ptr1,ptr2,ptr3
*then free ptr2 then ptr1 then ptr3, it checks merge,update head,split].
*/
void test1(void);

/*
*paramters (in):void
*paramters (out):void
*description: this function for debugging test if i allocate area then free it and then allocate within
*space of extra size.
*/
void normal(void);

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
*paramters (in):void
*paramters (out):void
*description: this function for debugging test if i allocate space and used it all and then freed it 
*test thet program break decrements .
*/
void test2();

#endif
