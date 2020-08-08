/*
 * Author      : Yang Dong
 * Created time: 2020.08.07
 */
#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define MAXBUFF 2020

long s = -1, E = -1, b = -1, S, B, verbose = 0;
long hit_count, miss_count, eviction_count, timestamp;
char *tracefile = NULL, line[MAXBUFF];
typedef struct {
	long tag;
	int valid, time;
} Line;
Line **a;

void visit(long addr) {
	long tag = addr >> (s + b), idx = addr >> b & (S - 1);
	int lru = -1, obj = -1;
	++timestamp;
	for(int i = 0; i < E; ++i) {
		if(a[idx][i].valid) {
			if(a[idx][i].tag == tag) {
				++hit_count;
				a[idx][i].time = timestamp;
				if(verbose) printf(" hit");
				return;
			}
			if(lru == -1 || a[idx][i].time < a[idx][lru].time)
				lru = i;
		} else
			obj = i;
	}
	if(verbose) printf(" miss");
	++miss_count;
	if(obj == -1) {
		obj = lru;
		++eviction_count;
		if(verbose) printf(" eviction");
	}
	a[idx][obj].tag = tag;
	a[idx][obj].valid = 1;
	a[idx][obj].time = timestamp;
}

int main(int argc, char *argv[])
{
	int opt;
	while((opt = getopt(argc, argv, "s:E:b:t:hv")) != -1) {
		if(opt == 's') s = atoi(optarg);
		else if(opt == 'E') E = atoi(optarg);
		else if(opt == 'b') b = atoi(optarg);
		else if(opt == 't') tracefile = optarg;
		else if(opt == 'v') verbose = 1;
		else {
			printf("Usage: %s [-hv] -s <s> -E <E> -b <b> -t <traceile>\n", argv[0]);
			exit(opt != 'h');
		}
	}
	if(verbose)
		printf("Config: s = %ld, E = %ld, b = %ld, verbose = %ld\n", s, E, b, verbose);
	if(s == -1 || E == -1 || b == -1) {
		printf("Usage: %s [-hv] -s <s> -E <E> -b <b> -t <traceile>\n", argv[0]);
		exit(1);
	}
	S = 1L << s, B = 1L << b;
	a = malloc(sizeof(Line*) * S);
	for(int i = 0; i < S; ++i) {
		a[i] = malloc(sizeof(Line) * E);
		for(int j = 0; j < E; ++j) a[i][j].valid = 0;
	}
	freopen(tracefile, "r", stdin);
	while(fgets(line, MAXBUFF, stdin)) {
		static char op[MAXBUFF];
		static long addr, sz;
		if(strlen(line) == 0) continue;
		if(line[0] != ' ') continue;
		sscanf(line, " %s %lx,%lx\n", op, &addr, &sz);
		if(verbose) printf("%s %lx,%lx", op, addr, sz);
		if(op[0] == 'L') visit(addr);
		else if(op[0] == 'S') visit(addr);
		else if(op[0] == 'M') visit(addr), visit(addr);
		if(verbose) printf("\n");
	}
    printSummary(hit_count, miss_count, eviction_count);
	for(int i = 0; i < S; ++i)
		free(a[i]);
	free(a);
    return 0;
}
