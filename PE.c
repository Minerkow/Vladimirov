

int maxim(int* parr, int len, int a, int b)
{
    int k = 0, p = 0;
    for (int i = 0; i < len; i++)
    {
        if (parr[i] == a)
            k++;
        if (parr[i] == b)
            p++;
    }
    if (k < p) return b;
    if (k == p) return a;

    return a;
}


int majority_element (const int* parr, int len)
{
    int a, b;
    if (len == 1)
        return *parr;
    if (len == 2)
    {
        if (*parr == *(parr + 1))
            return *parr;
        else
            return -1;
    }
    a = majority_element(parr, len/2);
    b = majority_element(parr + len/2 , len - len/2);
    if (a == -1)
    {
        return b;
    }
    if (b == -1)
    {
        return a;
    }

    return maxim(parr, len , a, b);
}
