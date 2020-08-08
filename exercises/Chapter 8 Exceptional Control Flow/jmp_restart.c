#include "csapp.h"

sigjmp_buf buf;

void sigint_handler(int sig) {
	siglongjmp(buf, 1);
}
int main() {
	if(!sigsetjmp(buf, 1)) {
		Signal(SIGINT, sigint_handler);
		Sio_puts("Starting...\n");
	} else Sio_puts("Restarting...\n");

	for(;;) {
		Sleep(1);
		Sio_puts("processing...\n");
	}
	exit(0);
	return 0;
}
