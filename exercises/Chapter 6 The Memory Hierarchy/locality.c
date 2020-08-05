/*
 * Author       : YangDavid
 * Created Time : 2020.08.05 16:50:12
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define N 5000
typedef long long ll;

ll sum_good(int a[N][N]) {
    ll sum = 0;
    for(int i = 0; i < N; ++i)
        for(int j = 0; j < N; ++j)
            sum = (sum + a[i][j]) ^ i ^ j;
    return sum;
}
ll sum_bad(int a[N][N]) {
    ll sum = 0;
    for(int j = 0; j < N; ++j)
        for(int i = 0; i < N; ++i)
            sum = (sum + a[i][j]) ^ i ^ j;
    return sum;
}
ll sum_bad2(int a[N][N]) {
    ll sum = 0;
    for(int len = 0; len < N+N; ++len)
        for(int i = len>N ? len-N : 0; i <= len; ++i) {
            int j = len - i;
            sum = (sum + a[i][j]) ^ i ^ j;
        }
    return sum;
}

int a[N][N];
int main() {
    size_t st = clock();
    for(int i = 0; i < N; ++i)
        for(int j = 0; j < N; ++j)
            a[i][j] = rand();

    double rat = CLOCKS_PER_SEC / 1000.0;
    printf("Rand take clock: %f ms\n", (clock() - st) / rat);
    st = clock();

    ll bad = sum_bad(a);
    printf("Bad take clock: %f ms. val = %lld\n", (clock() - st) / rat, bad);
    st = clock();

    ll bad2 = sum_bad(a);
    printf("Bad2 take clock: %f ms. val = %lld\n", (clock() - st) / rat, bad2);
    st = clock();
    
    ll good = sum_good(a);
    printf("Good take clock: %f ms. val = %lld\n", (clock() - st) / rat, good);
    return 0;
}

