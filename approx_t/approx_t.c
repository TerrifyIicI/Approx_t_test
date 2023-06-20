#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdlib.h>
#include "approx_t.h"

/* ������� ��������������� ����� �� ������ � �������. */
approx_t ApproxFromDouble(double center, double radius)
{
    approx_t approx;
    approx.lo = center - radius;
    approx.hi = center + radius;
    return approx;
}

/* ������� ��������������� ����� �� ����� � ������ �������. */
approx_t ApproxFromRange(double lo, double hi)
{
    approx_t approx;
    approx.lo = lo;
    approx.hi = hi;
    return approx;
}

/* ���������� ��������������� �����, ���������� ��� ���������� ������. */
void approxdestroy(approx_t* approx)
{
    if (approx) {
        memset(approx, 0, sizeof(*approx));
    }
}

/* ���������� ����� ������������� �����.*/
double ApproxAsDouble(approx_t approx)
{
    return (approx.lo + approx.hi) / 2.0;
}

/* ���������� ������ ������������� �����. */
double ApproxGetRadius(approx_t approx)
{
    return (approx.hi - approx.lo) / 2.0;
}

/* ��������� ������ � ���������� ��������������� ��������������� �����.
 * ��� ������ ������� ���������� ��������������� �������� (0.0, 0.0). */
approx_t ApproxParse(const char* str)
{
    double lo, hi;
    approx_t approx = { 0.0, 0.0 };

    if (sscanf(str, "<%.2f;%.2f>", &lo, &hi) == 2) {
        approx = ApproxFromRange(lo, hi);
    }

    return approx;
}

/* ��������� ��������������� ����� � ���� ������ � ���������� ��� � buf.
 * ������ ������ - bufsize ����. */
char* ApproxFormat(approx_t approx, char* buf, size_t bufsize)
{
    snprintf(buf, bufsize, "<%.2f;%.2f>", approx.lo, approx.hi);
    return buf;
}


/* ��������� ��������������� ����� �� �����. ���������� 1 ��� ������, 0 ��� EOF ���
 * ������ �������. */
int ApproxRead(approx_t* approx, FILE* fp)
{
    char buf[256];
    if (!fgets(buf, sizeof(buf), fp)) {
        *approx = ApproxParse(buf);
        return 0;
    }
    else return 1;
}

/* ���������� ��������������� ����� � ���� � ��������� �������. ���������� 1 ��� ������,
    * 0 ��� �������. */
int ApproxWrite(approx_t approx, FILE* fp)
{
    char buf[256];
    ApproxFormat(approx, buf, sizeof(buf));
    return fprintf(fp, "%s\n", buf) != EOF;
}

/* ��������� ��� ��������������� �����. */
approx_t ApproxAdd(approx_t a, approx_t b)
{
    return ApproxFromRange(a.lo + b.lo, a.hi + b.hi);
}

/* ��������� ���� ������������ �����. */
approx_t ApproxSub(approx_t a, approx_t b)
{
    return ApproxFromRange(a.lo - b.lo, a.hi - b.hi);
}

/* ������������ ���� ������������ �����. */
approx_t ApproxMul(approx_t a, approx_t b)
{
    return ApproxFromRange(a.lo * b.lo, a.hi * b.hi);
}

/* ����� ��� ������������ �����. */
approx_t ApproxDiv(approx_t a, approx_t b)
{
    if (b.lo == 0.0 && b.hi == 0.0) {
        /* ������� �� ���� ��� ������������� ����. */
        return ApproxFromRange(0.0, INFINITY);
    }
    return ApproxFromRange(a.lo / b.lo, a.hi / b.hi);
}

/* ���������� ������������� �������� ������������� �����. */
approx_t ApproxNegate(approx_t approx)
{
    return ApproxFromRange(-approx.hi, -approx.lo);
}

/* ���������� ���������� �������� ������������� �����. */
approx_t ApproxAbs(approx_t approx)
{
    if (approx.lo >= 0.0) {
        /* ��������� �������������. */
        return approx;
    }
    else if (approx.hi <= 0.0) {
        /* ��������� �������������. */
        return ApproxNegate(approx);
    }
    else {
        /* ������������ � ���� ��� ���������� ������ ���������. */
        double radius = fmax(-approx.lo, approx.hi);
        return ApproxFromDouble(0.0, radius);
    }
}

/* ���������� ��� ������������ �����. ���������� -1, ���� a < b, 0, ���� a == b,
    * � +1, ���� a > b. */
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