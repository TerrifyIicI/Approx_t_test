#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#pragma comment (lib, "approx_t.lib")
#include "approx_t.h"

int main()
{
    approx_t a = ApproxFromDouble(-1.26, 0.66);
    approx_t b = ApproxFromRange(2.86, 3.66);
    approx_t c;
    char buf[128];

    c = ApproxAdd(a, b);
    printf("a + b = %s\n", ApproxFormat(c, buf, sizeof(buf)));
    c = ApproxSub(a, b);
    printf("a - b = %s\n", ApproxFormat(c, buf, sizeof(buf)));
    c = ApproxMul(a, b);
    printf("a * b = %s\n", ApproxFormat(c, buf, sizeof(buf)));
    c = ApproxDiv(a, b);
    printf("a / b = %s\n", ApproxFormat(c, buf, sizeof(buf)));
    c = ApproxNegate(a);
    printf("-a = %s\n", ApproxFormat(c, buf, sizeof(buf)));
    c = ApproxAbs(a);
    printf("|a| = %s\n", ApproxFormat(c, buf, sizeof(buf)));
    int cmp = ApproxCompare(a, b);
    printf("a <=> b = %d\n", cmp);

    FILE* fwrite = fopen("approx.txt", "wt");
    if (!ApproxWrite(a, fwrite))
        printf("Не удалось записать approx в файл\n");
    if (!ApproxWrite(b, fwrite))
        printf("Не удалось записать approx в файл\n");
    if (!ApproxWrite(c, fwrite))
        printf("Не удалось записать approx в файл\n");
    fclose(fwrite);

    FILE* fread = fopen("approx.txt", "rt");
    if (!ApproxRead(&c, fread))
        printf("Не удалось прочитать approx из файла\n");
    printf("c from file = %s\n", ApproxFormat(c, buf, sizeof(buf)));
    if (!ApproxRead(&b, fread))
        printf("Не удалось прочитать approx из файла\n");
    printf("b from file = %s\n", ApproxFormat(b, buf, sizeof(buf)));
    if (!ApproxRead(&a, fread))
        printf("Не удалось прочитать approx из файла\n");
    printf("a from file = %s\n", ApproxFormat(a, buf, sizeof(buf)));
    fclose(fwrite);

    //approxdestroy(&a);
    //approxdestroy(&b);
    return 0;
}