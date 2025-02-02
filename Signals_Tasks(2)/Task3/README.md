# Dynamic Logger Module (Real-Time Signal Based)

## Overview
This project implements a **dynamically configurable logging system** in C that allows real-time adjustment of log levels using **POSIX real-time signals**. 

- The logger can operate at different log levels: `LOG_DISABLE`, `LOG_ERROR`, `LOG_WARNING`, `LOG_INFO`, and `LOG_DEBUG`.
- Another application (`set_log_level`) sends real-time signals (`SIGRTMIN`) to change the log level dynamically without restarting the logger.

## Features
✅ Supports **real-time log level changes** using signals.  
✅ Uses **POSIX real-time signals (`SIGRTMIN`)** to pass log level values.  
✅ **Another application** (`set_log_level`) sends signals to control logging dynamically.  
✅ Implements **hierarchical logging**, where setting a log level includes **all higher-priority** logs.  
✅ **Lightweight and efficient**, suitable for embedded systems and real-time applications.  

---

## Project Structure

---

## Log Levels

| **Log Level**  | **Description**                         | **Value** |
|---------------|-------------------------------------|-----------|
| `LOG_DISABLE` | Disables all logging              | `0`       |
| `LOG_ERROR`   | Logs only error messages          | `1`       |
| `LOG_WARNING` | Logs warning and error messages   | `2`       |
| `LOG_INFO`    | Logs info, warning, and error messages | `3`       |
| `LOG_DEBUG`   | Logs all messages (debug, info, warning, error) | `4`       |

> **Note:**  
> By default, when a log level is set, it logs that level and all higher-priority messages.  
> Example: If the level is set to `LOG_WARNING`, both `WARNING` and `ERROR` messages are logged.

---

## How It Works

1. **Start the Logger**
   - The `logger` application runs continuously and logs messages based on the current log level.
   - It waits for **real-time signals** to adjust the log level dynamically.

2. **Send a Signal to Change Log Level**
   - The `set_log_level` application is used to send a real-time signal (`SIGRTMIN`) along with an integer value representing the log level.

---

## Compilation

Compile both programs using `gcc`:

```sh
gcc logger.c -o logger
gcc set_log_level.c -o set_log_level
