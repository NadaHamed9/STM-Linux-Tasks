#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

/*defined according to priority highest to lowest*/
#define LOG_DISABLE 0
#define LOG_ERROR   1
#define LOG_WARNING 2
#define LOG_INFO    3
#define LOG_DEBUG   4

/* default level info */
volatile sig_atomic_t log_level = LOG_INFO;

void handle_signal(int sig, siginfo_t *info, void *context) {
	/* check value within 0 to 4 */
	if (info->si_value.sival_int >= LOG_DISABLE && info->si_value.sival_int <= LOG_DEBUG) 
	{

		log_level = info->si_value.sival_int;
		printf("Log level changed to: %d\n", log_level);
	}
	else
		printf("INVALID LEVEL\n");
}

void setup_signal_handler() {
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handle_signal;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

void log_message(int level, const char *message) 
{
	/*prints certain level+higher priority*/
	if (level <= log_level) {
		switch (level) {
			case LOG_ERROR:   printf("[ERROR] %s\n", message); break;
			case LOG_WARNING: printf("[WARNING] %s\n", message); break;
			case LOG_INFO:    printf("[INFO] %s\n", message); break;
			case LOG_DEBUG:   printf("[DEBUG] %s\n", message); break;
			default: break;
		}
	}
}

void  main() {
	printf("Logger PID: %d\n", getpid());
	setup_signal_handler();

	while (1) {
		log_message(LOG_DEBUG, "Debugging message.");
		log_message(LOG_INFO, "Informational message.");
		log_message(LOG_WARNING, "Warning message.");
		log_message(LOG_ERROR, "Error message.");
		sleep(3);
	}

}

