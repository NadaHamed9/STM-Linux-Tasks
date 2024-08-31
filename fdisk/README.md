
# Implement Fdisk (`fdisk -l`)

This project involves implementing a version of the `fdisk -l` command that reads and displays the partition table information from the Master Boot Record (MBR). The program has been extended to show logical partitions if they exist.
## Files Overview

- **main.c**: The entry point of the program. It handles the initialization and calls functions to read and display partition tables.
- **functions.c**: Contains the implementation of functions to read and display the partition and logical partition tables.
- **functions.h**: Header file that includes necessary includes, macros, data structures, and function prototypes.


## Compilation

To compile the project, use the following command:

```bash
gcc -o fdisk main.c functions.c
sudo ./fdisk /dec/sdb
