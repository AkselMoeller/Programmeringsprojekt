#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "lut-sin.h"
#include "vectors.h"

void printFix(int32_t i) {
    if((i & 0x80000000) != 0) {
        printf("-");
        i = ~i + 1;
    }
    printf("%ld.%04ld", i >> 16, 10000 * (uint32_t) (i & 0xFFFF) >> 16);
}

int32_t expand(int32_t i) {
    return i << 2;
}

int32_t sin(int x) {
    x = x % 512;
    if (x < 0) {
        x += 512;
    }
    //printFix(expand(SIN[x]));
    return SIN[x];
}

int32_t cos(int x) {
    return sin(x + 128);
}

void rot(int32_t * x, int32_t * y, int angle) {
    *x = FIX14_right((*x)*cos(angle)) - FIX14_right((*y)*sin(angle));
    *y = FIX14_right((*x)*sin(angle)) + FIX14_right((*y)*cos(angle));

    gotoxy(10, 2);
    printf("(");
    printFix(expand(*x));
    printf(",");
    printFix(expand(*y));
    printf(")\n");
    gotoxy(10, 3);
    printFix(expand(FIX14_right((*x) * (*x)) + FIX14_right((*y) * (*y))));
}

