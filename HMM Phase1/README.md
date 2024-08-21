
# Heap Memory Manager

## Project Overview

This project implements a Heap Memory Manager (HMM) to provide dynamic memory allocation services to user-space programs. The HMM is designed to simulate heap operations without relying on system calls like `sbrk()`. Instead, the heap is simulated using a statically allocated array, and a variable is used to simulate the program break.

## Features

- **Dynamic Memory Allocation:** Allocate memory using `void *HmmAlloc(size_t size)`.
- **Memory Deallocation:** Free allocated memory using `void HmmFree(void *ptr)`.
- **Heap Simulation:** Simulates the hepeeap in user space with no kernel dndencies, using a large statically allocated array.
- **Program Break Simulation:** Simulates the program break by adjusting a variable instead of using `sbrk()`.

## Files

- **`main.c`**: Contains the main function and test cases to validate the HMM implementation.
- **`functions.c`**: Contains the implementation of all memory management functions.
- **`functions.h`**: Header file with function prototypes, constants, and the `Block` structure definition.

## Functionality

### `void *HmmAlloc(size_t size)`

Allocates a block of memory of the specified size. If a suitable free block is found, it is reused; otherwise, a new block is allocated. The program break is adjusted as needed.

### `void HmmFree(void *ptr)`

Frees the memory block pointed to by `ptr`. The block is marked as free and merged with adjacent free blocks if possible. The program break is adjusted if the block is the last allocated one.

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

### Heap Simulation

- **Static Array**: A large statically allocated array simulates the heap area.
- **Program Break**: A variable simulates the program break, initially pointing to the start of the static array.

### Memory Management

- **Metadata**: Each block of memory includes metadata (size, next/previous block pointers, free status).
- **Free List**: A doubly linked list maintains free blocks for efficient allocation.

## Flowcharts

### `HmmAlloc()`
![Flowchart for HmmAlloc](https://github.com/NadaHamed9/STM-Linux-Tasks/blob/main/HMM%20Phase1/HmmAlloc.png)

### `HmmFree()`
![Flowchart for HmmFree](https://github.com/NadaHamed9/STM-Linux-Tasks/blob/main/HMM%20Phase1/HmmFree.png)

## Testing

- **Test Program**: A test program (`main.c`) simulates various allocation and deallocation scenarios to validate the HMM.
- **Random Allocation Test**: The test program includes random allocation sizes and orders to ensure robustness.

## Usage
 **Compile the Project:**
   ```bash
 gcc -o hmm main.c functions.c
