void sum_rows2(double *a, double *b, long n) {
    long i, j;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j)
            b[i] += a[i*n+j];
    }
}
void sum_rows(double *a, double *b, long n) { // no aliasing
    long i, j;
    for(int i = 0; i < n; ++i) {
        double val = 0;
        for(int j = 0; j < n; ++j)
            val += a[i*n+j];
        b[i] = val;
    }
}
