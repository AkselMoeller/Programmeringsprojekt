#ifndef VECTORS_H_INCLUDED
#define VECTORS_H_INCLUDED

void printFix(int32_t i);
int32_t expand(int32_t i);

int32_t sin(int x);
int32_t cos(int x);

typedef struct {
    int32_t x, y;
} vector_t;

void rot(int32_t * x, int32_t * y, int angle);

#endif
/*VECTORS_H_INCLUDED*/
