# ELF Header Information for `task1`

## Overview

This document provides details on the ELF (Executable and Linkable Format) header for the `task1` executable file.

## ELF Header Details

- **Magic Number:** `7f 45 4C 46 2 1 1 3 0 0`
- **Class:** ELF64
- **Data Encoding:** 2's complement, little endian
- **Version:** 1 (current)
- **OS/ABI:** Linux
- **ABI Version:** 0
- **Type:** EXEC (Executable file)
- **Machine:** AMD x86-64
- **Version:** 1
- **Entry Point Address:** `0x4200080`
- **Start of Program Header Table:** 64 bytes in file
- **Start of Section Header Table:** 902616 bytes in file
- **Flags:** 0
- **Size of ELF Header:** 64 bytes
- **Size of Program Header:** 56 bytes
- **Number of Program Headers:** 10
- **Size of Section Headers:** 64 bytes
- **Number of Section Headers:** 32
- **Section Header String Table Index:** 31

## Usage

To view the ELF header information for `task1`, compile the source code with the `gcc` command:

```bash
gcc -static task1.c -o task1

