
# Heap Memory Manager

## Project Overview

This project implements a Heap Memory Manager (HMM) to provide dynamic memory allocation services to user-space programs.The HMM replaces the libc dynamic memory manager by implementing standard memory management functions and utilizing `sbrk()` to interact with the kernel.

## Features

- **Dynamic Memory Allocation:** Allocate memory using `void *HmmAlloc(size_t size)`.
- **Memory Deallocation:** Free allocated memory using `void HmmFree(void *ptr)`.
- **Program Break Simulation:** Simulates the program break with a variable, now extended to use `sbrk()` for heap space reservation.
- **Standard Memory Management Functions:** Implements `malloc`, `free`, `calloc`, and `realloc` to manage memory dynamically.
- **Shared Library:** The HMM is compiled as a shared library, allowing it to replace the standard memory manager in Linux programs.

## Files
- **`hmm.c`**: Contains the implementation of all memory management functions.
- **`functions.h`**: Header file with function prototypes, constants, and the `Block` structure definition.

## Functionality

### `void *HmmAlloc(size_t size)`

Allocates a block of memory of the specified size. If a suitable free block is found, it is reused; otherwise, a new block is allocated. The program break is adjusted as needed.

### `void HmmFree(void *ptr)`

Frees the memory block pointed to by `ptr`. The block is marked as free and merged with adjacent free blocks if possible. The program break is adjusted if the block is the last allocated one.

### Standard Memory Management Functions

- **`void *malloc(size_t size)`**: Allocates memory using `HmmAlloc`.
- **`void free(void *ptr)`**: Frees memory using `HmmFree`.
- **`void *calloc(size_t num, size_t size)`**: Allocates memory for an array and initializes it to zero.
- **`void *realloc(void *ptr, size_t size)`**: Resizes previously allocated memory.

### Other Functions

- **`Block *find_free_block(size_t size)`**: Searches for a free block of the required size.
- **`void print_freelist(void)`**: Prints the free list for debugging purposes.
- **`void split_block(Block *block, size_t size)`**: Splits a free block if it is larger than needed.
- **`void merge_blocks(void)`**: Merges adjacent free blocks.
- **`void alloc_extra(void)`**: Allocates extra space in the heap to minimize program break adjustments.
- **`void cleanup_freelist(void)`**: Cleans up the free list by removing used blocks.
- **`void insert_free_block(Block *free_block)`**: Inserts a free block into the free list.
- **`void remove_from_freelist(Block* block)`**: Removes a block from the free list.

## Design

### Memory Management

- **Metadata**: Each block of memory includes metadata (size, next/previous block pointers, free status).
- **Free List**: A doubly linked list maintains free blocks for efficient allocation.

## Flowcharts

### `HmmAlloc()`
![Flowchart for HmmAlloc](https://github.com/NadaHamed9/STM-Linux-Tasks/blob/main/HMM%20Phase1/HmmAlloc.png)

### `HmmFree()`
![Flowchart for HmmFree](https://github.com/NadaHamed9/STM-Linux-Tasks/blob/main/HMM%20Phase1/HmmFree.png)

## Video Illustration

To get a visual demonstration of the HMM in action, you can watch the following video:

[Watch the Shell Demonstration]()


## Usage

### Make my code replace libc:

```bash
gcc --shared -fPIC -o libhmm.so hmm.c
export LD_PRELOAD= realpath libhmm.so
