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

void *serve(void *fd);
void clienterror(int fd, const char *cause, const char *errnum, const char *shortmsg, const char *longmsg);

struct Cache {

};

void sigpipe_handler(int sig) {
	write(STDERR_FILENO, "Broken pipe\n", 12 + 0*sig);
}

int main(int argc, char **argv) {
	Signal(SIGPIPE, sigpipe_handler);
	int listenfd, *connfdp;
	char hostname[MAXLINE], port[MAXLINE];
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;
	pthread_t tid;

	/* Check command-line args */
	if(argc != 2) {
		proxy_error("Usage: %s <port>\n", argv[0]);
	}
	listenfd = Open_listenfd(argv[1]);
	for(;;) {
		clientlen = sizeof(clientaddr);
		connfdp = Malloc(sizeof(int));
		*connfdp = Accept(listenfd, (SA*)&clientaddr, &clientlen);
		Getnameinfo((SA*)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
		fprintf(stderr, "Accepted connection from (%s, %s), fd = %d\n", hostname, port, *connfdp);
		Pthread_create(&tid, NULL, serve, connfdp);
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
		strncpy(host, uri + 7, (size_t)(slash - uri) - 7);
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

void *serve(void *fdp) {
	Pthread_detach(Pthread_self());
	char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
	char host[MAXLINE], port[MAXLINE], new_uri[MAXLINE];
	char obj[MAX_OBJECT_SIZE], header[MAX_OBJECT_SIZE];
	int fd = *((int*) fdp);

	rio_t rio;
	Rio_readinitb(&rio, fd);
	
	/* Read request line and headers */
	if(Rio_readlineb(&rio, buf, MAXLINE) < 0) goto End;
	fprintf(stderr, "Request headers: %s\n", buf);
	if(sscanf(buf, "%s %s %s", method, uri, version) != 3) {
		clienterror(fd, method, "400", "Bad request", "Proxy cannot understand this request.");
		return 0;
	}

	if(strcasecmp(method, "GET")) {
		clienterror(fd, method, "501", "Not implemented", "Proxy does not implement this method");
		return 0;
	}

	/* Deal with headers */
	int o = 0, has_host = 0;
	if(!parse_uri(fd, method, uri, host, port, new_uri)) return 0;
	fprintf(stderr, "  Parsed host = %s, port = %s, new_uri = %s\n", host, port, new_uri);
	o += sprintf(header + o, "%s %s HTTP/1.0\r\n", method, new_uri);
	
	while(rio_readlineb(&rio, buf, MAXLINE) > 0) {
		if(!strncmp(buf, "\r\n", 2)) {
			break;
		} else if(strncasecmp(buf, "connection", 10) && strncasecmp(buf, "proxy-connection", 16)
			   && strncasecmp(buf, "user-agent", 10)) {
			o += sprintf(header + o, "%s", buf);
			if(!strncasecmp(buf, "host", 4)) {
				has_host = 1;
				char *ptr = buf + 4, *ho = host;
				while(*ptr && !isalnum(*ptr)) ptr++;
				while(*ptr && *ptr != ':' && *ptr != '\r' && *ptr != '\n') {
					*ho = *ptr;
					ptr++, ho++;
				}
				*ho = 0;
			}
		}
	}
	/* Send request header */
	if(!has_host)
		o += sprintf(header + o, "Host: %s\r\n", host);
	o += sprintf(header + o,  "%s", user_agent_hdr);
	o += sprintf(header + o, "Connection: close\r\n");
	o += sprintf(header + o, "Proxy-Connection: close\r\n\r\n");
	fprintf(stderr, "Sent header:\n%s", header);
	
	/* Connect to destination server */
	int destfd;
	if((destfd = open_clientfd(host, port)) == -1) {
		clienterror(fd, method, "403", "Forbidden", "Proxy cannot connect to the required url");
		return 0;
	}
	if(Rio_writen(destfd, header, (size_t)o) < 0) goto End;
	ssize_t sz;
	for(;;) {
		sz = Rio_readn(destfd, obj, MAX_OBJECT_SIZE);
		if(sz == 0) break;
		if(sz < 0) goto End;
		if(Rio_writen(fd, obj, (size_t)sz) < 0) goto End;
	}
End:;
	fprintf(stderr, "Closed connection in fd %d\n", fd);
	Close(fd);
	Free(fdp);
	fflush(stderr);
	return 0;
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
	if(Rio_writen(fd, buf, strlen(buf)) < 0) return;
	sprintf(buf, "Content-type: text/html\r\n");
	if(Rio_writen(fd, buf, strlen(buf)) < 0) return;
	sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
	if(Rio_writen(fd, buf, strlen(buf)) < 0) return;
	if(Rio_writen(fd, body, strlen(body)) < 0) return;
}
