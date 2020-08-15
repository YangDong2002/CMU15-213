/*
 * Project Name: Proxy lab
 * Author      : Yang Dong
 * Created Time: 2020.08.14
 */

#include "csapp.h"
#include <assert.h>
#define proxy_error(...) do { fprintf(stderr, __VA_ARGS__); exit(0); } while(0);

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

void *serve(void *fd);
void clienterror(int fd, const char *cause, const char *errnum, const char *shortmsg, const char *longmsg);

typedef struct {
	char *name, *content;
	time_t lastvisit;
} CacheEntry;

typedef struct {
	CacheEntry c[MAXLINE];
	int size, cnt;
} Cache;

Cache cache;
sem_t mutex, w;
int readcnt;

char *reader(const char* name) {
	char *ans = NULL;
	P(&mutex);
	if(readcnt++ == 0) P(&w);
	V(&mutex);

	for(int i = 0; i < cache.cnt; ++i) if(cache.c[i].lastvisit > 0 && !strcmp(name, cache.c[i].name)) {
		ans = Malloc(strlen(cache.c[i].content) + 1);
		strcpy(ans, cache.c[i].content);
		cache.c[i].lastvisit = time(NULL);
		break;
	}

	P(&mutex);
	if(--readcnt == 0)
		V(&w);
	V(&mutex);
	return ans;
}
void writer(const char *name, const char *content) {
	P(&w);
	int n = (int)strlen(content);
	/* LRU Eviction */
	while(n + cache.size > MAX_CACHE_SIZE || cache.cnt == MAXLINE) {
		int cho = -1;
		for(int i = 0; i < MAXLINE; ++i) if(cache.c[i].lastvisit > 0) {
			if(cho == -1 || cache.c[i].lastvisit < cache.c[cho].lastvisit)
				cho = i;
		}
		assert(cho != -1);
		cache.size -= strlen(cache.c[cho].content);
		cache.cnt--;
		Free(cache.c[cho].content);
		Free(cache.c[cho].name);
		cache.c[cho].lastvisit = -1;
	}
	/* Insertion */
	for(int i = 0; i < MAXLINE; ++i) if(i == cache.cnt || cache.c[i].lastvisit <= 0) {
		cache.c[i].name = Malloc(strlen(name) + 1);
		cache.c[i].content = Malloc(strlen(content) + 1);
		strcpy(cache.c[i].name, name);
		strcpy(cache.c[i].content, content);
		cache.c[i].lastvisit = time(NULL);
		cache.cnt++, cache.size += strlen(content);
		break;
	}
	fprintf(stderr, "Cached %s\n", name);
	V(&w);
}

void sigpipe_handler(int sig) {
	write(STDERR_FILENO, "Broken pipe\n", 12 + 0*sig);
}

int main(int argc, char **argv) {
	sem_init(&mutex, 0, 1);
	sem_init(&w, 0, 1);
	Signal(SIGPIPE, sigpipe_handler);
	for(int i = 0; i < MAXLINE; ++i)
		cache.c[i].lastvisit = -1;
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
	char firstline[MAXLINE], buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
	char host[MAXLINE], port[MAXLINE], new_uri[MAXLINE];
	char obj[MAX_OBJECT_SIZE], header[MAX_OBJECT_SIZE];
	int fd = *((int*) fdp);

	rio_t rio;
	Rio_readinitb(&rio, fd);
	
	/* Read request line and headers */
	if(Rio_readlineb(&rio, buf, MAXLINE) < 0) goto End;
	strcpy(firstline, buf);
	fprintf(stderr, "Request headers: %s\n", buf);
	if(sscanf(buf, "%s %s %s", method, uri, version) != 3) {
		clienterror(fd, method, "400", "Bad request", "Proxy cannot understand this request.");
		return 0;
	}

	if(strcasecmp(method, "GET")) {
		clienterror(fd, method, "501", "Not implemented", "Proxy does not implement this method");
		return 0;
	}

	/* Check cache */
	char *res = reader(firstline);
	if(res != NULL) {
		fprintf(stderr, "Cache hit!\n");
		Rio_writen(fd, res, strlen(res));
		Free(res);
	} else {
		fprintf(stderr, "Cache miss...\n");
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
		ssize_t sz, totsz = 0;
		for(;;) {
			sz = Rio_readn(destfd, obj, MAX_OBJECT_SIZE);
			if(sz == 0) break;
			if(sz < 0) goto End;
			if(Rio_writen(fd, obj, (size_t)sz) < 0) goto End;
			totsz += sz;
		}
		if(totsz <= MAX_OBJECT_SIZE)
			writer(firstline, obj);
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
