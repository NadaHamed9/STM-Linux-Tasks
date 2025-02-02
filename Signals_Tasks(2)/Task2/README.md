# Non-Blocking Wait Alternative using Signal Handling

## Overview
This project demonstrates an **alternative to the `wait` system call** by using **signal handling (`SIGCHLD`)** to track terminated child processes. The parent process does not block while waiting for children but instead receives a **signal asynchronously** when a child process terminates.

### **Key Features**
✅ **Handles child process termination non-blockingly using `SIGCHLD`.**  
✅ **Prevents zombie processes using `SA_NOCLDWAIT`.**  
✅ **Prints the PID, exit status, and termination reason (signal or normal exit).**  
✅ **Ensures the parent continues running after child processes terminate.**  

---

## Project Structure

---

## **How It Works**
1. The program **spawns multiple child processes** (`NUM_CHILDREN = 4`).
2. The **parent process registers a `SIGCHLD` handler** using `sigaction()`, allowing it to handle child process termination **asynchronously**.
3. The `SIGCHLD` handler:
   - **Identifies how the child terminated** (normal exit or killed by a signal).
   - **Prevents zombie processes** by using `SA_NOCLDWAIT` to avoid `wait()` calls.
4. The **parent continues running indefinitely** and does not exit when children terminate.

---

## **Compilation & Execution**
### **Compile the Program**
```sh
gcc non_blocking_wait.c -o non_blocking_wait
