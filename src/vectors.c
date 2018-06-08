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

void rot(vector_t * v, int angle) {
    (*v).x = (*v).x*cos(angle) - (*v).y*sin(angle);
    (*v).y = (*v).x*sin(angle) + (*v).y*cos(angle);

    printf("(");
    printFix(expand((*v).x));
    printf(",");
    printFix(expand((*v).y));
    printf(")\n");
}

