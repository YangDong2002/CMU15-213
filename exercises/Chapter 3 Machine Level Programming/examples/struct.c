struct Rec {
    int a[4];
    long i;
    struct Rec *next;
};

void set_val(struct Rec *r, int val) {
    while(r) {
        int i = r->i;
        r->a[i] = val;
        r = r->next;
    }
}

void what(struct Rec p) {
    p.i = 1;
    set_val(&p, 233);
}
