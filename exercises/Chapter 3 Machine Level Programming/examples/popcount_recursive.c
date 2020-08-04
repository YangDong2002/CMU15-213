long popcount(unsigned long x) {
    return x == 0 ? 0 : (x & 1) + popcount(x >> 1);
}
