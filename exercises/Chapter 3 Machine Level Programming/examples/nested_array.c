int pgh[233][5];
int *univ[5];

// global variables in .LFE0: .comm
int *get_pgh_zip(long index) { return pgh[index]; }
int get_pgh_digit(long x, long y) { return pgh[x][y]; }
int get_univ_digit(long x, long y) { return univ[x][y]; }

int nxn_matrix_access(int n, int a[n][n], int i, int j) {
    return a[i][j];
}
