/*
 * Project Name: Proxy lab
 * Author      : Yang Dong
 * Created Time: 2020.08.14
 */

#include "csapp.h"
#define proxy_error(...) do { fprintf(stderr, __VA_ARGS__); exit(0); } while(0);

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

void serve(int fd);
void clienterror(int fd, const char *cause, const char *errnum, const char *shortmsg, const char *longmsg);


void sigpipe_handler(int sig) {
	write(STDERR_FILENO, "Broken pipe!", 12);
	exit(0);
}

int main(int argc, char **argv) {
	Signal(SIGPIPE, sigpipe_handler);
	int listenfd, connfd;
	char hostname[MAXLINE], port[MAXLINE];
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;

	/* Check command-line args */
	if(argc != 2) {
		proxy_error("Usage: %s <port>\n", argv[0]);
	}
	listenfd = Open_listenfd(argv[1]);
	for(;;) {
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);
		Getnameinfo((SA*)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
		fprintf(stderr, "Accepted connection from (%s, %s)\n", hostname, port);
		serve(connfd);
		Close(connfd);
		fprintf(stderr, "Closed connection from (%s, %s)\n", hostname, port);
		fflush(stderr);
	}
	
    return 0;
}

int parse_uri(int fd, const char method[MAXLINE], const char uri[MAXLINE], char host[MAXLINE], char port[MAXLINE], char new_uri[MAXLINE]) {
	if(strstr(uri, "http://") != uri) {
		clienterror(fd, method, "404", "Not found", "Not a valid HTTP url");
		return 0;
	}
	char *slash = strchr(uri + 7, '/');
	if(slash != NULL) {
		strcpy(new_uri, slash);
		strncpy(host, uri + 7, (slash - uri) - 7);
	} else {
		strcpy(new_uri, "/");
		strcpy(host, uri + 7);
	}
	if(host[0] == 0) { /* empty host */
		clienterror(fd, method, "404", "Not found", "Not a valid HTTP url");
		return 0;
	}
	char *colon = strchr(host, ':');
	if(colon != NULL) strcpy(port, colon+1), *colon = 0;
	else strcpy(port, "80");
	return 1;
}

void serve(int fd) {
	char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
	char host[MAXLINE], port[MAXLINE], new_uri[MAXLINE];
	char obj[MAX_OBJECT_SIZE], header[MAX_OBJECT_SIZE];

	rio_t rio;
	Rio_readinitb(&rio, fd);
	
	/* Read request line and headers */
	Rio_readlineb(&rio, buf, MAXLINE);
	fprintf(stderr, "Request headers: %s\n", buf);
	if(sscanf(buf, "%s %s %s", method, uri, version) != 3) {
		clienterror(fd, method, "400", "Bad request", "Proxy cannot understand this request.");
		return;
	}

	if(strcasecmp(method, "GET")) {
		clienterror(fd, method, "501", "Not implemented", "Proxy does not implement this method");
		return;
	}

	/* Deal with headers */
	int o = 0;
	if(!parse_uri(fd, method, uri, host, port, new_uri)) return;
	fprintf(stderr, "  Parsed host = %s, port = %s, new_uri = %s\n", host, port, new_uri);
	o += sprintf(header + o, "%s %s HTTP/1.0\r\n", method, new_uri);
	while(rio_readlineb(&rio, buf, MAXLINE) > 0) {
		if(!strncmp(buf, "\r\n", 2)) {
			break;
		} else if(!strncasecmp(buf, "host", 4)) {
			char *ptr = buf + 4, *ho = host;
			while(*ptr && !isalnum(*ptr)) ptr++;
			while(*ptr && *ptr != ':' && *ptr != '\r' && *ptr != '\n') {
				*ho = *ptr;
				ptr++, ho++;
			}
			*ho = 0;
		} else if(strncasecmp(buf, "connection", 10) && strncasecmp(buf, "proxy-connection", 16)
			   && strncasecmp(buf, "user-agent", 10)) {
			o += sprintf(header + o, "%s", buf);
		}
	}
	o += sprintf(header + o, "Host: %s\r\n", host);
	o += sprintf(header + o,  "%s", user_agent_hdr);
	o += sprintf(header + o, "Connection: close\r\n");
	o += sprintf(header + o, "Proxy-Connection: close\r\n\r\n");

	fprintf(stderr, "Sent header:\n%s", header);
	
	/* Connect to destination server */
	int destfd;
	printf("host = %s  (mannualy end of line)\r\n", host);
	if((destfd = open_clientfd(host, port)) == -1) {
		clienterror(fd, method, "403", "Forbidden", "Proxy cannot connect to the required url");
		return;
	}
	Rio_writen(destfd, header, o);

	ssize_t sz;
	while((sz = Rio_readn(destfd, obj, MAX_OBJECT_SIZE)) > 0) {
		Rio_writen(fd, obj, sz);
	}
}

void clienterror(int fd, const char *cause, const char *errnum, const char *shortmsg, const char *longmsg) {
	fprintf(stderr, "ERROR: %s %s\r\n%s\r\n", errnum, shortmsg, longmsg);
	char buf[MAXLINE], body[MAXBUF];
	int o = 0; /* offset of sprintf */
	o =  sprintf(body, "<html><title>Proxy error</title>");
	o += sprintf(body+o, "<body bgcolor=""ffffff"">\r\n");
	o += sprintf(body+o, "%s: %s\r\n", errnum, shortmsg);
	o += sprintf(body+o, "<p>%s: %s\r\n", longmsg, cause);
	o += sprintf(body+o, "<hr><em>Proxy lab</em>\r\n");

	sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
	Rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Content-type: text/html\r\n");
	Rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
	Rio_writen(fd, buf, strlen(buf));
	Rio_writen(fd, body, strlen(body));
}
