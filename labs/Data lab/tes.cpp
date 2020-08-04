/*
 * Author       : YangDavid
 * Created Time : 2020.08.04 19:48:35
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
int cond(int x, int y, int z) {
    cout << y << endl;
    int nx = !x, ns = nx << 4;
    int nnx = !nx, nns = nnx << 4;
    cout << nns << endl;
    cout << ((z >> nns) >> nns) << endl;
    return ((y >> ns) >> ns) | ((z >> nns) >> nns);
}
int main() {
    int z = 0x80000000;
    cout << cond(z, z, z);

    return 0;
}

