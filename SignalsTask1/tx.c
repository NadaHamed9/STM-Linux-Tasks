#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

/******************************* TX OF SIGNAL *******************************/

int main(int argc, char *argv[])
{
    int sig, sigData;
    pid_t pid;
    union sigval sv;

    // Check for the correct number of arguments
    if (argc < 4 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s <PID> <SIGNAL> <SIGDATA>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse arguments
    pid = (pid_t) atoi(argv[1]);  // Convert PID to integer
    sig = atoi(argv[2]);          // Convert signal number to integer
    sigData = atoi(argv[3]);      // Convert signal data to integer
    sv.sival_int = sigData;       // Assign signal data to the sigval union

    // Print process and user information
    printf("%s: PID is %ld, UID is %ld\n", argv[0], (long)getpid(), (long)getuid());

    // Send signal with sigqueue
    if (sigqueue(pid, sig, sv) == -1) {
        perror("sigqueue");
        exit(EXIT_FAILURE);
    }

    return 0;
}

