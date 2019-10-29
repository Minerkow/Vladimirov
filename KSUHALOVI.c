#include <stdio.h>

int main() {
    int arr[] = {3, 3, 4, 2, 4, 4, 2, 4};
    printf("|%d|", majority_element(arr, 8));
}

int majority_element(int *parr, int len) {
    int l = len / 2, L = len, a = 0, b = 0, p = -1;
    if (len == 2)
    {if (*parr == *(parr + 1))
            return *parr;
        return -1;}
    if (len == 3) {
        if (*parr == *(parr + 1)) return *parr;
        if (*parr == *(parr + 2)) return *parr;
        if (*(parr + 1) == *(parr + 2)) return *(parr + 1);
        return -1;
    }
    if (len > 3)
    {a = majority_element(parr, l);
    printf("%d ", a);
    b = majority_element((parr + l), (l + len % 2));
    printf("%d\n", b);}
    if (len == 8) return max_element(parr, a, b, len);

}

int max_element(int *parr, int a, int b, int len) {
    int a1 = 0, b1 = 0;
    printf("|%d %d|", a, b);
    for (int i = 0; i < len; i++) {
        if (*(parr + i) == a)
            a1 += 1;
        if (*(parr + i) == b)
            b1 += 1;
    }
    if (a1 >= b1)
        return a;
    if (a1 < b1)
        return b;
}
