#define GNU_SOURCE
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*********************************** RX OF SIGNAL ******************************/

static void handler (int sig,siginfo_t *info, void *ucontext)
{
	printf("Caught signal %d (%s)\n",sig,strsignal(sig));
	/*terminates if real signal value 0*/
	if(info->si_value.sival_int == 0 )
	{
		printf("Action: Terminating process\n");
		exit(0);
	}
	/*terminates and generating core dump file if real signal value 1*/
	else if(info->si_value.sival_int == 1)
	{
		printf("Action: Aborting the process with core dump file\n");
		abort();
	}
	/*other data value*/
	else
		printf("Action: Executing default behavior\n");
}

int main(int argc ,char *argv[])
{

	struct sigaction termSigAct;

	termSigAct.sa_sigaction = handler;
	sigemptyset(&termSigAct.sa_mask);
	termSigAct.sa_flags = SA_SIGINFO;
	/*Register all real-time signals (SIGRTMIN to SIGRTMAX)*/
	for (int sig = SIGRTMIN; sig <= SIGRTMAX; sig++) {
		if (sigaction(sig, &termSigAct, NULL) == -1) {
			perror("sigaction");

		}
	}

	while(1)
	{	
		pause();	}
}

