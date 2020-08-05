#include<vector>
using namespace std;

vector<double> sum(const vector< vector<double> >& a) {
    size_t n = (int)a.size();
    vector<double> ret(n);
    for(size_t i = 0; i < n; ++i)
        for(size_t j = 0; j < n; ++j)
            ret[i] += a[i][j];
    return ret;
}
