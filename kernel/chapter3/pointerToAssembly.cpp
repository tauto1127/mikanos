void foo() {
    int i = 42;
    int* p = &i;
    int r1 = *p;
    *p = 1;
    int r2 = i;
}