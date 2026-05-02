#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	char command[100];
	while (1) {
		printf("$ ");
		fgets(command, 100, stdin);
		command[strcspn(command, "\n")] = '\0'; // removes newline

		// splits command into an array of arguments
		char *args[10];
		int i = 0;
		args[0] = strtok(command, " ");
		while (args[i] != NULL) {
			i++;
			args[i] = strtok(NULL, " "); // strtok with NULL starts from where the last strtok left off
		}

		if (strcmp(args[0], "exit") == 0) break;
		if (strcmp(args[0], "cd") == 0) {
			chdir(args[1]);
			continue;
		}

		int pid = fork();
		if (pid == 0) {
			execvp(args[0], args);
			printf("command not found: %s\n", args[0]);
			exit(1);
		} else {
			wait(NULL);
		}
	}
	return 0;
}
