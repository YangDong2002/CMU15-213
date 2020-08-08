#include "csapp.h"
#define N 20
int main() {
	pid_t pid;
	int status;

	printf("Parent pid = %d\n", getpid());

	for(int i = 0; i < N; ++i) {
		if((pid = Fork()) == 0) {
			Sleep(10);
			exit(i + N);
		} else
			printf("Created child %d\n", pid);
	}
	while((pid = wait(&status)) > 0) {
		if(WIFEXITED(status)) {
			printf("Child %d exited with status %d\n", pid, status);
		} else printf("Child %d exited abnormally.\n", pid);
	}

	if(errno != ECHILD)
		unix_error("waitpid error");

	return 0;
}
