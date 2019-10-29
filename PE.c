#include <stdio.h>

int main() {
    int arr[] = {3, 3, 9, 9, 9, 3};
    printf("%d", majority_element(arr, 6));
}

int majority_element(int *parr, int len) {
    int l = len/2;
    if (l == 1)
        if (*parr == *(parr + 1))
            return *parr;
        else return -1;
    int L = len;
    int a = majority_element(parr, len / 2);
    int b = majority_element((parr + len / 2), (len / 2 + len % 2));
    if (l == 0)
        return max_element(parr, a, b, L);
}

int max_element(int *parr, int a, int b, int len) {
    int a1, b1;
    for (int i = 0; i < len; i++) {
        if ( parr[i] = a ) a1 += 1;
        if ( parr[i] = b ) b1 += 1;
    }
    if (a1 >= b1) return a1;
    if (a1 < b1) return b1;
}
