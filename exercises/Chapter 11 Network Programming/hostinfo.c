#include "csapp.h"

int main(int argc, char **argv) {
	struct addrinfo *p, *listp, hints;
	char buf[MAXLINE];
	int rc, flags;

	if(argc != 2) {
		fprintf(stderr, "usage: %s <domain name>\n", argv[0]);
		exit(0);
	}

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM; // Connections only
	Getaddrinfo(argv[1], NULL, &hints, &listp);

	flags = NI_NUMERICHOST;
	for(p = listp; p; p = p->ai_next) {
		printf("### ai_addr: %d  ", p->ai_addr->sa_family);
		for(int i = 0; i < p->ai_addrlen; ++i)
			printf("%d ", p->ai_addr->sa_data[i]);
		printf("\n");
		Getnameinfo(p->ai_addr, p->ai_addrlen, buf, MAXLINE, NULL, 0, flags);
		printf("%s\n", buf);
	}

	return 0;
}
