int a[4];
int res[4];
int temp[4];
int n;
scanf(n);

a[0] = 1;
a[1] = 1;
a[2] = 1;
a[3] = 0;

res[0] = 1;
res[1] = 0;
res[2] = 0;
res[3] = 1;

while (n) {
    if (n & 1) {
        temp[0] = res[0] * a[0] + res[1] * a[2];
        temp[1] = res[0] * a[1] + res[1] * a[3];
        temp[2] = res[2] * a[0] + res[3] * a[2];
        temp[3] = res[2] * a[1] + res[3] * a[3];
        res[0] = temp[0] & 32767;
        res[1] = temp[1] & 32767;
        res[2] = temp[2] & 32767;
        res[3] = temp[3] & 32767;  
    }
    n = n >> 1;
    temp[0] = a[0] * a[0] + a[1] * a[2];
    temp[1] = a[0] * a[1] + a[1] * a[3];
    temp[2] = a[2] * a[0] + a[3] * a[2];
    temp[3] = a[2] * a[1] + a[3] * a[3];
    a[0] = temp[0] & 32767;
    a[1] = temp[1] & 32767;
    a[2] = temp[2] & 32767;
    a[3] = temp[3] & 32767;
}

printf(res[2]);