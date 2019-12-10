#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

void patpreproc(char const *needle, int *needle_lps);

char *strstrci(char const *needle, int const *needle_lps, char const *haystack);

int main() {
    char str[] = "qrtfCISeEDNOBUeEDNOBUHRSqqUPZHRSqqUPZKjjQGePZARIuHQCkZHlnbbBnlcmNeEDNeEDNOBUHRSqqUPZOBUHRSqeEDNOeEDNOBUHRSqqUPZBUHRSqqUPZqUPZjjuSjILbdYAiJxbpunWVkuXeEDNOBUHRSqqUPZnltPmbCAxTkvFvYUfahQCgiQIhZFbHvINBeAfCXafa";// строка , где выполняется поиск
    char form[] = "eednobuhrsqqupz"; // образ
    int lenform = strlen(form);
    int *SufPref = (int *) calloc(lenform,
                                  sizeof(int));// массив для сохранения наибольшей длины совпадения суффикса и префикса
    patpreproc(form, SufPref);
    char *p = strstrci(form, SufPref, str);
    printf("%p\n", p);
    printf("%c%c", *p, *(p + 1));
    return 0;
}


void patpreproc(char const *needle, int *needle_lps) {
    int len = strlen(needle);
    int j = 0;
    needle_lps[0] = 0;
    for (int i = 1; i < len; ) {
        if (tolower(needle[i]) == tolower(needle[j])) {
            needle_lps[i] = j + 1;
            i++;
            j++;
        } else {
            if (j == 0) {
                needle_lps[i] = 0;
                i++;
            } else {
                j = needle_lps[j - 1];
            }
        }
    }
}

char *strstrci(char const *needle, int const *needle_lps, char const *haystack) {
    int j = 0; // индекс символа в образе
    char *copyhaystack = NULL;
    int lenstr = strlen(haystack);
    int lenform = strlen(needle);
    for (int i = 0; i < lenstr; ) // i - индекс символа в строке
    {
        if (tolower(haystack[i]) == tolower(needle[j])) {
            i++;
            j++;
            if (j == lenform) {
                copyhaystack = haystack;
                return copyhaystack + i - j;
            }
        } else {
            if (j == 0) {
                i++;
                if (i == lenstr) {
                    return NULL;
                }
            } else {
                j = needle_lps[j - 1];
            }
        }
    }
    return NULL;
}
