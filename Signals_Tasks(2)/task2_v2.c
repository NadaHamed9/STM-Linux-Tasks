#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define NUM_CHILDREN 4

pid_t children[NUM_CHILDREN]; 
/*to track children active*/
volatile sig_atomic_t active_children = NUM_CHILDREN;
/* SIGCHLD handler */
void handle_sigchld(int sig, siginfo_t *info, void *context) {
    if (info->si_code == CLD_EXITED) {  
	    /*if  Child exited normally */
        printf("Child %d exited normally with status %d.\n", info->si_pid, info->si_status);
    } 
    else if (info->si_code == CLD_KILLED || info->si_code == CLD_DUMPED) {  
	    /*if Child killed by signal */
        printf("Child %d terminated by signal %d.\n", info->si_pid, info->si_status);
    }
      active_children--;

    if (active_children == 0) {
        printf("All children are terminated. Parent is still runing.\n");
        
    }
}

int main() {
    struct sigaction sa;
    sa.sa_sigaction = handle_sigchld;  
    sigemptyset(&sa.sa_mask);
    /*to not consider stop child as terminated and avoid zombie*/
    sa.sa_flags = SA_SIGINFO | SA_NOCLDSTOP | SA_NOCLDWAIT; 
   
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /*Fork child processes*/
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) { 
		/* Child*/
            printf("Child %d (PID: %d) started.\n", i, getpid());
            if (i % 2 == 0) {
                sleep(5);
                exit(i);
            }
            while (1) sleep(1); /* Others keep running */
        }
        children[i] = pid; 
    }


    /*Parent keeps running*/
    while (1) {
        sleep(1);
    }

    return 0;
}

