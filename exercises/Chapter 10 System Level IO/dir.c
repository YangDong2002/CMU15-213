#include "csapp.h"
#define MAXBUFF 8192

int main(int argc, char **argv) {
	if(argc != 2) {
		printf("Usage: %s <dirname>", argv[0]);
		exit(1);
	}
	DIR *dir = Opendir(argv[1]);
	struct dirent *dp;
	errno = 0;
	while((dp = readdir(dir)) != NULL) {
		static char path[MAXBUFF];
		static struct stat st;
		printf("Found file %40s\n", dp->d_name);
	}
	if(errno != 0) {
		unix_error("readdir error");
	}
	Closedir(dir);
	return 0;
}
