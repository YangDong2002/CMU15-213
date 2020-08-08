#include "csapp.h"

void sigint_handler(int signum) {
}

unsigned int snooze(unsigned int secs) {
	int lef = sleep(secs);
	printf("Slept for %d of %d secs.\n", secs - lef, secs);
	return lef;
}
int main(int argc, char **argv) {
	Signal(SIGINT, sigint_handler);
	int x = atoi(argv[1]);
	snooze(x);
	return 0;
}
