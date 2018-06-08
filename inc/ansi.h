#include "stm32f30x_conf.h"
#include "30010_io.h"
#ifndef ANSI_H_INCLUDED
#define ANSI_H_INCLUDED

void fgcolor(int foreground);
void bgcolor(int background);
void color(int foreground, int background);
void resetbgcolor();

void clrscr();
void clreol();
void gotoxy(int x, int y);
void underline(uint8_t on);
void blink(uint8_t on);
void inverse(uint8_t on);
void window(int x1, int y1, int x2, int y2, char * title, uint8_t style);

typedef struct {
    int x, y, vX, vY; //(x,y) is the position of the ball, and (vX,vY) is the velocity-vector of the ball (how much it's going to move up/down next iteration)
} ball_t;
void drawBall(ball_t * ball_p);
void updateBallPos(ball_t * ball_p, int k);

#endif /* ANSI_H_INCLUDED */
