#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdlib.h>
#include "approx_t.h"

/* Создает приблизительное число из центра и радиуса. */
approx_t ApproxFromDouble(double center, double radius)
{
    approx_t approx;
    approx.lo = center - radius;
    approx.hi = center + radius;
    return approx;
}

/* Создает приблизительное число из левой и правой границы. */
approx_t ApproxFromRange(double lo, double hi)
{
    approx_t approx;
    approx.lo = lo;
    approx.hi = hi;
    return approx;
}

/* Уничтожает приблизительное число, освобождая всю выделенную память. */
void approxdestroy(approx_t* approx)
{
    if (approx) {
        memset(approx, 0, sizeof(*approx));
    }
}

/* Возвращает центр приближенного числа.*/
double ApproxAsDouble(approx_t approx)
{
    return (approx.lo + approx.hi) / 2.0;
}

/* Возвращает радиус приближенного числа. */
double ApproxGetRadius(approx_t approx)
{
    return (approx.hi - approx.lo) / 2.0;
}

/* Разбирает строку и возвращает соответствующее приблизительное число.
 * При ошибке разбора возвращает приблизительное значение (0.0, 0.0). */
approx_t ApproxParse(const char* str)
{
    double lo, hi;
    approx_t approx = { 0.0, 0.0 };

    if (sscanf(str, "<%.2f;%.2f>", &lo, &hi) == 2) {
        approx = ApproxFromRange(lo, hi);
    }

    return approx;
}

/* Формирует приблизительное число в виде строки и записывает его в buf.
 * Размер буфера - bufsize байт. */
char* ApproxFormat(approx_t approx, char* buf, size_t bufsize)
{
    snprintf(buf, bufsize, "<%.2f;%.2f>", approx.lo, approx.hi);
    return buf;
}


/* Считывает приблизительное число из файла. Возвращает 1 при успехе, 0 при EOF или
 * ошибка разбора. */
int ApproxRead(approx_t* approx, FILE* fp)
{
    char buf[256];
    if (!fgets(buf, sizeof(buf), fp)) {
        *approx = ApproxParse(buf);
        return 0;
    }
    else return 1;
}

/* Записывает приблизительное число в файл в текстовом формате. Возвращает 1 при успехе,
    * 0 при неудаче. */
int ApproxWrite(approx_t approx, FILE* fp)
{
    char buf[256];
    ApproxFormat(approx, buf, sizeof(buf));
    return fprintf(fp, "%s\n", buf) != EOF;
}

/* Добавляет два приблизительных числа. */
approx_t ApproxAdd(approx_t a, approx_t b)
{
    return ApproxFromRange(a.lo + b.lo, a.hi + b.hi);
}

/* Вычитание двух приближенных чисел. */
approx_t ApproxSub(approx_t a, approx_t b)
{
    return ApproxFromRange(a.lo - b.lo, a.hi - b.hi);
}

/* Перемножение двух приближенных чисел. */
approx_t ApproxMul(approx_t a, approx_t b)
{
    return ApproxFromRange(a.lo * b.lo, a.hi * b.hi);
}

/* Делит два приближенных числа. */
approx_t ApproxDiv(approx_t a, approx_t b)
{
    if (b.lo == 0.0 && b.hi == 0.0) {
        /* Деление на ноль или аппроксимация нуля. */
        return ApproxFromRange(0.0, INFINITY);
    }
    return ApproxFromRange(a.lo / b.lo, a.hi / b.hi);
}

/* Возвращает отрицательное значение приближенного числа. */
approx_t ApproxNegate(approx_t approx)
{
    return ApproxFromRange(-approx.hi, -approx.lo);
}

/* Возвращает абсолютное значение приближенного числа. */
approx_t ApproxAbs(approx_t approx)
{
    if (approx.lo >= 0.0) {
        /* Полностью положительное. */
        return approx;
    }
    else if (approx.hi <= 0.0) {
        /* Полностью отрицательный. */
        return ApproxNegate(approx);
    }
    else {
        /* Приближается к нулю или пересекает начало координат. */
        double radius = fmax(-approx.lo, approx.hi);
        return ApproxFromDouble(0.0, radius);
    }
}

/* Сравнивает два приближенных числа. Возвращает -1, если a < b, 0, если a == b,
    * и +1, если a > b. */
int ApproxCompare(approx_t a, approx_t b)
{
    if (a.hi < b.lo) {
        return -1;
    }
    else if (a.lo > b.hi) {
        return 1;
    }
    else {
        return 0;
    }
}