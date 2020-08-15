/* 
 * Author      : Yang Dong
 * Created Time: 2020.08.15
 *
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include <string.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 *
 * s = 5, E = 1, b = 5 -> 32 bytes = 8 ints for a block
 */


char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, v[8], x, y;
	
	if(M == 61) {
		for (i = 0; i < N; i += 16) {
			for (j = 0; j < M; j += 16) {
				for(x = i; x < i + 16 && x < N; ++x) {
					for(y = j; y < j + 16 && y < M; ++y)
						B[y][x] = A[x][y];
				}
			}
		}
	} else if(M == 32) {
		for(i = 0; i < N; i += 8) {
			for(j = 0; j < M; j += 8) {
				for(x = i; x < i + 8; ++x) {
					memcpy(v, A[x] + j, sizeof v);
					for(y = j; y < j + 8; ++y)
						B[y][x] = v[y - j];
				}
			}
		}
	} else if(M == 64) {
		for(i = 0; i < N; i += 4) {
			for(j = 0; j < M; j += 4) {
				for(x = i; x < i + 4; ++x) {
					memcpy(v, A[x] + j, sizeof(int)*4);
					for(y = j; y < j + 4; ++y)
						B[y][x] = v[y - j];
				}
			}
		}
	}
}

#define BLK 4
char transpose_blk4_desc[] = "Transpose block size 4";
void transpose_blk4(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i += BLK) {
        for (j = 0; j < M; j += BLK) {
			for(int x = i; x < i + BLK && x < N; ++x) {
				for(int y = j; y < j + BLK && y < M; ++y)
					B[y][x] = A[x][y];
			}
        }
    }    

}
#undef BLK
#define BLK 2
char transpose_blk2_desc[] = "Transpose block size 2";
void transpose_blk2(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i += BLK) {
        for (j = 0; j < M; j += BLK) {
			for(int x = i; x < i + BLK && x < N; ++x) {
				for(int y = j; y < j + BLK && y < M; ++y)
					B[y][x] = A[x][y];
			}
        }
    }    

}
#undef BLK
#define BLK 20
char transpose_blk16_desc[] = "Transpose block size 16";
void transpose_blk16(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i += BLK) {
        for (j = 0; j < M; j += BLK) {
			for(int x = i; x < i + BLK && x < N; ++x) {
				for(int y = j; y < j + BLK && y < M; ++y)
					B[y][x] = A[x][y];
			}
        }
    }    

}
#undef BLK
/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 
    registerTransFunction(transpose_blk4, transpose_blk4_desc); 
    registerTransFunction(transpose_blk2, transpose_blk2_desc); 
    registerTransFunction(transpose_blk16, transpose_blk16_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

