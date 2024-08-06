# Shell Command Project

## Overview

This project is a simple shell command implementation written in C. It parses user input and translates it into commands. The supported commands are:
- `pwd`: Print the working directory.
- `echo`: Print a user input string on stdout.
- `exit`: Terminate the shell.
- `mv`: Move a file from one location to another.
- `cp`: Copy a file from one location to another.
- `help`: Display a list of supported commands.

## Files

- `main.c`: Contains the main function which checks the input command and calls the appropriate function from `commands.c`.
- `commands.c`: Contains the implementation of the supported commands.
- `commands.h`: Header file declaring the functions defined in `commands.c`.

## Compilation

To compile the project, use the following command:

```sh
gcc -o shell main.c commands.c




