#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

/*defined according to priority highest to lowest*/
#define LOG_DISABLE 0
#define LOG_ERROR   1
#define LOG_WARNING 2
#define LOG_INFO    3
#define LOG_DEBUG   4

int get_log_level(const char *level_str) {
	if (strcmp(level_str, "disable") == 0) return LOG_DISABLE;
	if (strcmp(level_str, "error") == 0) return LOG_ERROR;
	if (strcmp(level_str, "warning") == 0) return LOG_WARNING;
	if (strcmp(level_str, "info") == 0) return LOG_INFO;
	if (strcmp(level_str, "debug") == 0) return LOG_DEBUG;
	return -1;
}

int  main(int argc, char *argv[]) 
{
	union sigval value;

	if (argc != 3) {
		printf("Usage: %s <PID> <log_level>\n", argv[0]);
		return EXIT_FAILURE;
	}
	/*store pid*/
	pid_t pid = atoi(argv[1]);
	/*store log level */
	int log_level = get_log_level(argv[2]);

	if (log_level == -1) {
		perror("Invalid log level: ");

	}


	value.sival_int = log_level;

	/*send level value through realtime signal */
	if (sigqueue(pid, SIGRTMIN, value) == -1) {
		perror("sigqueue failed");

	}

	printf("Sent signal to change log level to %s for PID %d\n", argv[2], pid);

}

