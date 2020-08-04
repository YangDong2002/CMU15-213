long shift(long x, long n) {
    x <<= 4;
    x >>= n;
    return x;
}
