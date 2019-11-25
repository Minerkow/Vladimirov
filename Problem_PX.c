#include <stdio.h>
#include <assert.h>

#define N 5

void powNxN (unsigned (*A)[N], unsigned k, unsigned m);
void MultMatr(unsigned (*A)[N], unsigned m);
void ProdMatr(unsigned (*prod)[N]);
void Xmatr(unsigned (*A)[N], unsigned (*B)[N], unsigned m);

PrintMatr(unsigned (*A)[N])
{
        for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
            printf ("%u ", A[i][j]);
        printf("\n");
    }
    printf("\n");
}

int main()
{
    unsigned A[N][N] = {{100, 123, 127, 19, 235}, {1, 0, 1 , 15, 17}};
    PrintMatr(A);
    powNxN(A, 100000, 10);
    PrintMatr(A);
    //Xmatr(A, A, 10);
    //PrintMatr(A);
    return 0;
}

unsigned pow_mod (unsigned n, unsigned k, unsigned m)
{
    unsigned long long mult = n % m;
    unsigned long long prod = 1;
    while (k > 0)
    {
        if ((k % 2) == 1)
        {
            prod = (prod * mult) % m;
            k = k - 1;
        }
        else
        {
            mult = (mult * mult) % m;
            k = k / 2;
        }
    }
    return prod;
}

void powNxN (unsigned (*A)[N], unsigned k, unsigned m)
{
    unsigned prod[N][N] = {{0}, {0}};
    MultMatr(A, m);
    //PrintMatr(A);
    ProdMatr(prod);
    //printf("Check\n");
    //PrintMatr(prod);
    while (k > 0)
    {
        if (k % 2 == 1)
        {
            Xmatr(prod, A, m);
            //PrintMatr(prod);
            k = k - 1;
        }
        else
        {
            Xmatr(A, A, m);
            k = k / 2;
            //PrintMatr(A);
        }
    }
    //PrintMatr(prod);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            A[i][j] = prod[i][j];
}

void MultMatr(unsigned (*A)[N], unsigned m)
{
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            A[i][j] = A[i][j] % m;

}

void ProdMatr(unsigned (*prod)[N])
{
    unsigned flag = 0;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (i == flag && j == flag)
            {
                //printf("[%d][%d]", i, j);
                prod[i][j] = 1;
                //printf("[%u]", prod[i][j]);
            }
        }
        flag++;
    }
}

void Xmatr(unsigned (*A)[N], unsigned (*B)[N], unsigned m)
{
    unsigned copy[N][N] = {{0}, {0}};
    for (int i = 0; i < N; ++i)
    {
        for (int b = 0; b < N; ++b)
        {
            for (int j = 0; j < N; ++j)
            {
                copy[i][b] = (copy[i][b] + A[i][j] * B[j][b]) % m;
                // printf("[i - %d][j - %d][b - %d][copy[%d][%d] = %u]\n", i, j , b, i, b, copy[i][b]);
            }
        }
    }
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
        {
            A[i][j] = copy[i][j];
            //printf("[%u]", A[i][j]);
        }
}

