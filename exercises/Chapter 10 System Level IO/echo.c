#include "csapp.h"
#define N 23333
int main() {
	static char c[N];
	while(Read(STDIN_FILENO, c, N) != 0)
		Write(STDOUT_FILENO, c, N);
	return 0;
}
