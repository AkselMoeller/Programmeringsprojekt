#include "stm32f30x_conf.h"
#include "30010_io.h"
#ifndef ANSI_H_INCLUDED
#define ANSI_H_INCLUDED

void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void color(uint8_t foreground, uint8_t background);
void resetbgcolor();
void clrscr();
void clreol();
void gotoxy(int x, int y);
void underline(uint8_t on);
void blink(uint8_t on);
void inverse(uint8_t on);

#endif /* ANSI_H_INCLUDED */
