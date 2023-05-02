int n;
    int p[12];
    int r;
    int d;
    int i;
    int x;
    int j;
    int base;
    int e;

    p[0] = 2;
    p[1] = 3;
    p[2] = 5;
    p[3] = 7;
    p[4] = 11;
    p[5] = 13;
    p[6] = 17;
    p[7] = 19;
    p[8] = 23;
    p[9] = 29;
    p[10] = 31;
    p[11] = 37;

    scanf(n);
    r = 0;
    i = 0;
    d = n - 1;

    if (n < 2 || (n > 2 && ((n & 1) == 0))) {
        printf(0);
        return;
    }

    while ((d & 1) == 0) {
        d = d >> 1;
        r = r + 1;
    }

    while (i < 12) {
        if (n == p[i]) {
            printf(1);
            return;
        }
        x = 1;
        base = p[i] - n * (p[i] / n);
        e = d;
        while (e) {
            if (e & 1) {
                x = x * base;
                x = x - n * (x / n);
            }
            base = base * base;
            base = base - n * (base / n);
            e = e >> 1;
        }
        if (x != 1 && x != n - 1) {
            j = 1;
            while (j < r && x != n - 1) {
                j = j + 1;
                x = x * x;
                x = x - n * (x / n);
            }
            if (x != n - 1) {
                printf(0);
                return;
            }
        }
        i = i + 1;
    }

    printf(1);

    return;