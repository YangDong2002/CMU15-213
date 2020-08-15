/*
 * Author       : YangDavid
 * Created Time : 2020.08.15 18:55:51
 */

#include<bits/stdc++.h>
#define rep(i, n) for(int i = 1; i <= n; ++i)
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
template<typename... Args> auto ndim(size_t sz0, Args&&... args) {
    if constexpr(sizeof...(args) == 1) return vector(sz0, args...);
    else return vector(sz0, ndim(args...));
}
int A[32][32], B[32][32];
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
	int i, j, x, y, tmp;
	if(M == 61) {
		for (i = 0; i < N; i += 16) {
			for (j = 0; j < M; j += 16) {
				for(x = i; x < i + 16 && x < N; ++x) {
					for(y = j; y < j + 16 && y < M; ++y)
						B[y][x] = A[x][y];
				}
			}
		}
	} else {
		memcpy(B, A, sizeof(int)*N*M);
		for(i = 0; i < N; i += 4) {
			for(x = i; x < i + 4; ++x) {
				for(y = i; y < x; ++y) {
					tmp = B[x][y];
					B[x][y] = B[y][x];
					B[y][x] = tmp;
				}
			}
		}
		for(i = 0; i < N; i += 4) {
			for(j = 0; j < i; j += 4) {
				for(x = i; x < i+4; ++x) {
					for(y = j; y < j+4; ++y) {
						tmp = B[x][y];
						B[x][y] = B[y][x];
						B[y][x] = tmp;
					}
				}
			}
		}
	}
}
int main() {
	for(int i = 0; i < 1024; ++i) A[0][i] =  i;
	transpose_submit(32, 32, A, B);
	for(int i = 0; i < 32; ++i)
		for(int j = 0; j < 32; ++j) if(A[i][j] != B[j][i]) {
			printf("A[%d,%d] = %d, B[%d,%d] = %d\n", i, j, A[i][j], j, i, B[j][i]);
		}
    return 0;
}

