#include "csapp.h"
#define MAXN 300

int main() {
	int fd = Open("test.txt", O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
	printf("fd1 = %d\n", fd);

	/*
	 * Without O_CREAT: No such file or directory
	 * With mode=0    : create a file with ---------, cannot rwx.
	 */
	Write(fd, "naive", 5);
	Close(fd);
	fd = Open("baz.txt", O_CREAT, S_IRUSR|S_IWUSR);
	printf("fd2 = %d\n", fd);
	Close(fd);
	/*
	 fd1 = 3
	 fd2 = 3
	 */

	char buf[MAXN];
	printf("Read = %ld\n", Read(STDIN_FILENO, buf, MAXN));
	printf("buf = %s\n", buf);
	return 0;
}
