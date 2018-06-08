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

#endif /* ANSI_H_INCLUDED */
