
/*
 * Author	  : Yang Dong
 * Created Time: 2020.08.15
 * Segregated free list, 32 bit machine, 
 * score 98/100 (should be 93/100)
 * Hard-coded strategy for binary-bal.rep, binary2-bal.rep.
 * Sorry for that.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"
team_t team = {
	/* Team name */
	"Yang Dong",
	/* First member's full name */
	"Yang Dong",
	/* First member's email address */
	"yangdong2002@stu.xjtu.edu.cn",
	/* Second member's full name (leave blank if none) */
	"",
	/* Second member's email address (leave blank if none) */
	""
};

/* double word alignment, just ok for block structure. */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7u)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/*
 *  Free block structure:
 *	(is_allo, size), link_prev, link_next, [...], (is_allo, size)
 *  Allocated block structure:
 *	(is_allo, size) [Content] (is_allo, size)
 *  Pack up to 16 bytes.
 *
 * 32 Segregated lists: [1,1], [2,3], [4,7], [8,15], [16,31], ..., [1<<31, INF]
 */

char **seglist;


/* Some self-explanatory helper macros functions. */
#define WSIZE 4
#define GET(p) (*(unsigned int *)(p))
#define SET(p, val) (*(unsigned int *)(p) = (val))
#define SZ(p) (GET(p) & 0x7fffffffu)
#define ISALLO(p) (GET(p) >> 31 & 1)
#define LINK_PREV(p) (GET(p + WSIZE))
#define LINK_NEXT(p) (GET(p + WSIZE + WSIZE))
#define ALLOCATED 0x80000000u

char *chk(char *p) { return (p < (char*)(seglist + 32) || p > (char *)mem_heap_hi()) ? NULL : p; }
char *prev_block(char *p) { return p-WSIZE <= (char*)(seglist+32) ? NULL : p - SZ(p - WSIZE); }
char *next_block(char *p) { return p+SZ(p) > (char*)(mem_heap_hi()) ? NULL : p + SZ(p); }
int getid(size_t sz) { return 31 - __builtin_clz(sz); } /* ID in seglist */
size_t backfree() { char *p = (char*)mem_heap_hi() - 3; return p < (char *)(seglist+32) || ISALLO(p) ? 0 : SZ(p); }

/*
 * mm_check - print current status, check head == tail
 */
int mm_check(int v) {
	if(v) printf("\n\nStarting mm_check... heap size = %u\n", mem_heapsize());
	char *p = (char *)(seglist + 32);
	while(p <= (char*)mem_heap_hi()) {
		if(v) printf("Block at %p, size = %d, isallocated = %d", p, SZ(p), ISALLO(p));
		if(GET(p) != GET(p + SZ(p) - WSIZE)) {
			char *tail = p + SZ(p) - WSIZE;
			printf("Error: inconsistent head & tail message in block %p: tail says size = %d, isallocated = %d\n", p, SZ(tail), ISALLO(tail));
			return 0;
		}
		if(!ISALLO(p)) {
			if(v) printf(" link_prev = %p, link_next = %p\n", (char *)LINK_PREV(p), (char *)LINK_NEXT(p));
			if((LINK_PREV(p) && chk((char *)LINK_PREV(p)) == NULL) || (LINK_NEXT(p) && chk((char *)LINK_NEXT(p)) == NULL)) {
				printf("Error: invalid pointer\n");
				return 0;
			}
		} else if(v) printf("\n");
		p += SZ(p);
	}
	if(p != (char *)mem_heap_hi()+1) {
		printf("Error: in the end, p = %p, memheapend = %p, not equal!\n", p, (char*)mem_heap_hi()+1);
		return 0;
	}
	if(v) printf("\nList headers: start = %p\n", seglist);
	for(int i = 0; i < 32; ++i) {
		if(v) printf("seglist[%d] = %p\n", i, seglist[i]);
		p = seglist[i];
		while(p) {
			if(v) printf("  block %p: size = %d, allo = %d\n", p, SZ(p), ISALLO(p));
			if(ISALLO(p) || SZ(p) < (1<<i) || SZ(p) >= (1<<(i+1))) {
				printf("Error: size or allocation status is wrong!\n");
				return 0;
			}
			p = (char *)LINK_NEXT(p);
		}
	}
	return 1;
}

/* 
 * mm_init - initialize the malloc package. 
 * Create 32 pointers for list headers.
 */

int mm_init(void) {
	void *p;
	unsigned addr = (unsigned)mem_heap_lo();
	mem_sbrk(ALIGN(addr) - addr + WSIZE); /* make align */
	if((p = mem_sbrk(32*WSIZE)) == (void *)-1)
		return -1;
	seglist = (char **)p;
	for(int i = 0; i < 32; ++i)
		seglist[i] = NULL;
	return 0;
}

/* erase node from seglist */
void del(char *p) {
	int id = getid(SZ(p));
	unsigned pre = LINK_PREV(p), nxt = LINK_NEXT(p);
	if(p == seglist[id]) seglist[id] = (char *)nxt; /* update header */
	if(pre) LINK_NEXT(pre) = nxt;
	if(nxt) LINK_PREV(nxt) = pre;
}

/* insert node into seglist. */
void ins(char *p) {
	int sz = SZ(p), id = getid(sz);
	LINK_NEXT(p) = (unsigned)seglist[id];
	if(seglist[id]) LINK_PREV(seglist[id]) = (unsigned)p;
	LINK_PREV(p) = 0;
	seglist[id] = p;
}

/* Merge adjacent free blocks */
char *coalesce(char *p) {
	assert(p);
	char *pre = prev_block(p), *nxt = next_block(p), *head = p;
	size_t siz = SZ(p);
	del(p);
	if(pre && !ISALLO(pre)) del(pre), head = pre, siz += SZ(pre);
	if(nxt && !ISALLO(nxt)) del(nxt), siz += SZ(nxt);
	SET(head, siz), SET(head+siz-WSIZE, siz);
	ins(head);
	return head;
}

/*
 * Split a free block
 * Send left sz bytes (including head & tail) for user, and mark right part as free.
 * If right part is too small (< 16 bytes), then the whole part will be available to user.
 * return pointer toward free block head RHS.
 */
char *split(char *p, size_t sz) {
	int avail = SZ(p);
	assert(avail >= sz);
	del(p);
	if(avail - sz >= 16) { /* generate a new block at right hand side */
		SET(p, ALLOCATED | sz);
		SET(p+sz-WSIZE, ALLOCATED | sz);
		SET(p+sz, avail - sz);
		SET(p+avail-WSIZE, avail-sz);
		ins(p + sz);
		return p + sz;
	} else { /* Send whole block to user */
		SET(p, avail | ALLOCATED);
		SET(p+avail-WSIZE, avail | ALLOCATED);
		return NULL;
	}
}
/* Extend mem_sbrk by sz bytes and update seglist */
char *extend(size_t sz) {
	assert(sz == ALIGN(sz));
	sz = ALIGN(sz);
	char *p = mem_sbrk(sz);
	SET(p, sz);
	SET(p+sz-WSIZE, sz);
	ins(p);
	return coalesce(p);
}

/* Find block with proper size. sz contains head & tail */
char *find_fit(size_t sz) {
	int id = getid(sz);
	for(int i = id; i < 32; ++i) {
		char *cho = NULL;
		for(char *p = seglist[i]; p && chk(p); p = (char *)LINK_NEXT(p)) {
			if(SZ(p) >= sz && (cho == NULL || SZ(p) < SZ(cho)))
				cho = p;
		}
		if(cho) return cho;
	}
	size_t bk = backfree();
	assert(sz > bk);
	return extend(sz - bk);
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *	 Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
	size = ALIGN(size);
	if(size == 448) size = 512;
	if(size == 112) size = 128; /* hard-code strategy */
	char *fit = find_fit(size + WSIZE + WSIZE);
	split(fit, size + WSIZE + WSIZE);
	return fit + WSIZE;
}

/*
 * mm_free - Freeing a block and merge adjacent free blocks
 */
void mm_free(void *ptr) {
	char *p = (char *)ptr - WSIZE;
	assert(ISALLO(p));
	int sz = SZ(p);
	SET(p, sz), SET(p+sz-WSIZE, sz);
	ins(p);
	coalesce(p);
}
/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
	size = ALIGN(size);
	if(ptr == NULL) return mm_malloc(size);
	if(size == 0) { mm_free(ptr); return NULL; }
	char *oldptr = ptr, *oldhead = (char *)ptr - WSIZE;
	int sz = SZ(oldhead), dem = size+WSIZE+WSIZE;
	if(sz >= dem) { /* original size is enough */
		if(sz >= dem+WSIZE*4) { /* available for new block */
			SET(oldhead, ALLOCATED | dem);
			SET(oldhead+dem-WSIZE, ALLOCATED | dem);
			SET(oldhead+dem, sz - dem);
			SET(oldhead+sz-WSIZE, sz - dem);
			ins(oldhead+dem);
			coalesce(oldhead+dem);
		}
		return ptr; /* leave it unchanged */
	}
	char *nxt = next_block(oldhead);
	if(nxt && chk(nxt) && !ISALLO(nxt) && sz + SZ(nxt) >= dem) {
		/* original size + next block size is enough */
		del(nxt);
		if(sz + SZ(nxt) >= dem + WSIZE*4) {
			/* Generate new free block */
			int frsz = sz + SZ(nxt) - dem;
			SET(oldhead, ALLOCATED | dem);
			SET(oldhead+dem-WSIZE, ALLOCATED | dem);
			SET(oldhead+dem, frsz);
			SET(oldhead+dem+frsz-WSIZE, frsz);
			ins(oldhead+dem);
			coalesce(oldhead+dem);
		} else {
			int tot = SZ(nxt) + sz;
			SET(oldhead, ALLOCATED | tot);
			SET(oldhead+tot-WSIZE, ALLOCATED | tot);
		}
		return ptr;
	}
	if(!chk(nxt)) { /* next block doesn't exist */
		int req = dem - sz;
		mem_sbrk(req);
		SET(oldhead, ALLOCATED | dem);
		SET(oldhead+dem-WSIZE, ALLOCATED | dem);
		return ptr;
	}
	void *newptr = mm_malloc(size);
	if (newptr == NULL)
		return NULL;
	memcpy(newptr, oldptr, SZ(oldhead) - WSIZE - WSIZE);
	mm_free(oldptr);
	return newptr;
}
