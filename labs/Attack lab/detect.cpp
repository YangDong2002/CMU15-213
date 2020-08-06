/*
 * Author       : YangDavid
 * Created Time : 2020.08.06 22:05:57
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
int main() {
    freopen("hack1_1.txt", "r", stdin);
    int ch;
    while((ch = getchar()) != EOF)
        printf("%x ", ch);

    return 0;
}

