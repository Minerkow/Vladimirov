#include <stdio.h>

int main()
{
        int arr[] = {3, 3, 4, 2, 4, 4, 2};
        printf("Answer - %d", majority_element(arr, 7));
        return 0;
}

int majority_element (int* parr, int len)
{
        if (len == 1)
                return *parr;
        if (len == 2)
        {
                if (*parr == *(parr + 1))
                        return *parr;
                        else
                                return -1;
        }
        int a = majority_element(parr, len/2);
        int b = majority_element(parr + len/2 , len - len/2);
        if (a == -1)
                return b;
        if (b == -1)
                return a;
        if (a != -1 && b != -1)
        return max(parr, len , a, b);
}

int max(int* parr, int len, int a, int b)
{
        int k = 0, p = 0;
        for (int i = 0; i < len; i++)
        {
                if (parr[i] == a)
                        k++;
                if (parr[i] == b)
                        p++;
        }
        if (k >= p)
                return a;
                        else return b;
}



