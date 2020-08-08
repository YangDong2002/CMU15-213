#include <stdio.h>
int main(int argc, char *argv[], char *envp[]) {
	puts("Command-line arguments:");
	for(int i = 0; argv[i]; ++i)
		printf("    argv[%d]: %s\n", i, argv[i]);
	puts("Environment variables:");
	for(int i = 0; envp[i]; ++i)
		printf("    envp[%d]: %s\n", i, envp[i]);
	return 0;
}
