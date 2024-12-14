# Signal Tasks (1)

This project demonstrates the use of real-time signals to simulate terminate and abort operations between two applications: a sender (`tx.c`) and a receiver (`rx.c`).

## Overview

- The **receiver** (`rx.c`) listens for real-time signals.
- The **sender** (`tx.c`) sends signals with associated data to the receiver.
- Based on the signal data:
  - If the data is `0`, the receiver **terminates** gracefully.
  - If the data is `1`, the receiver **aborts** (terminates with a core dump).

## Features

- **Real-time Signal Handling**: Uses POSIX real-time signals (`SIGRTMIN`).
- **Signal Data**: Passes data (`0` or `1`) with the signal to determine the receiver's behavior.
- **Two Applications**:
  - `tx.c` (Sender): Sends the signal, PID, and data.
  - `rx.c` (Receiver): Listens for signals and performs actions based on the signal data.

## Prerequisites

- A Linux environment with GCC installed.
- Knowledge of process IDs (`PID`) to send signals.

## Code Files

1. **`rx.c`** (Receiver): Handles incoming real-time signals.
2. **`tx.c`** (Sender): Sends real-time signals to a specified process.

## Compilation

To compile the code, use the following commands:

```bash
gcc -o rx rx.c
gcc -o tx tx.c

