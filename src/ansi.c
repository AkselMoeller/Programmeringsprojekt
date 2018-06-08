#include "ansi.h"
#define ESC 0x1B
#include <string.h>

void fgcolor(int foreground) {
/*  Value      foreground     Value     foreground
    ------------------------------------------------
      0        Black            8       Dark Gray
      1        Red              9       Light Red
      2        Green           10       Light Green
      3        Brown           11       Yellow
      4        Blue            12       Light Blue
      5        Purple          13       Light Purple
      6        Cyan            14       Light Cyan
      7        Light Gray      15       White
*/
    int type = 22;             // normal text
	if (foreground > 7) {
        type = 1;            // bold text
        foreground -= 8;
	}
    printf("%c[%d;%dm", ESC, type, foreground+30);
}

void bgcolor(int background) {
/* IMPORTANT:   When you first use this function you cannot get back to true white background in HyperTerminal.
   Why is that? Because ANSI does not support true white background (ANSI white is gray to most human eyes).
                The designers of HyperTerminal, however, preferred black text on white background, which is why
                the colors are initially like that, but when the background color is first changed there is no
                way coming back.
   Hint:        Use resetbgcolor(); clrscr(); to force HyperTerminal into gray text on black background.

    Value      Color
    ------------------
      0        Black
      1        Red
      2        Green
      3        Brown
      4        Blue
      5        Purple
      6        Cyan
      7        Gray
*/
    printf("%c[%dm", ESC, background+40);
}

void color(int foreground, int background) {
// combination of fgcolor() and bgcolor() - uses less bandwidth
    int type = 22;            // normal text
    if (foreground > 7) {
        type = 1;             // bold text
        foreground -= 8;
    }
    printf("%c[%d;%d;%dm", ESC, type, foreground+30, background+40);
}

void resetbgcolor() {
    // gray on black text, no underline, no blink, no reverse
    printf("%c[m", ESC);
}

void clrscr() {
    printf("%c[2J", ESC);
}

void clreol() {
    printf("%c[K", ESC);
}

void gotoxy(int x, int y) {
    printf("%c[%d;%dH", ESC, y, x);
}

void underline(uint8_t on) { //0 = off, 1-255 = on
    printf("%c[%dm", ESC, on ? 4 : 24);
}

void blink(uint8_t on) { //0 = off, 1-255 = on
    printf("%c[%dm", ESC, on ? 5 : 25);
}

void inverse(uint8_t on) {//0 = off, 1-255 = on
    printf("%c[%dm", ESC, on ? 7 : 27);
}

void window(int x1, int y1, int x2, int y2, char * title, uint8_t style) {
    uint8_t hSide, vSide, lt, lb, rt, rb, lBorder, rBorder; //r/l = right/left, t/b = top/bottom, v/h = horizontal/vertical
    //Two different styles
    if (style) { //Wide style
        vSide = 10 + 176;
        hSide = 13 + 192;
        lt = 9 + 192;
        lb = 8 + 192;
        rt = 11 + 176;
        rb = 12 + 176;
        lBorder = 9 + 176;
        rBorder = 12 + 192;
    } else { //Narrow style
        vSide = 3 + 176;
        hSide = 4 + 192;
        lt = 10 + 208;
        lb = 0 + 192;
        rt = 15 + 176;
        rb = 9 + 208;
        lBorder = 4 + 176;
        rBorder = 3 + 192;
    }

    //Drawing bottom horizontal side (without corners)
    for (int i = x1 + 1; i < x2; i++) {
        gotoxy(i, y2);
        printf("%c", hSide);
    }

    //Drawing vertical sides (without corners)
    for (int i = y1 + 1; i < y2; i++) {
        gotoxy(x1, i);
        printf("%c", vSide);
        gotoxy(x2, i);
        printf("%c", vSide);
    }

    //Drawing corners
    gotoxy(x1,y1);
    printf("%c", lt);
    gotoxy(x2,y1);
    printf("%c", rt);
    gotoxy(x1,y2);
    printf("%c", lb);
    gotoxy(x2,y2);
    printf("%c", rb);

    //Drawing title (if a title is given)
    if (title[0] != '\0') {
        gotoxy(x1 + 1,y1);
        printf("%c", lBorder);
        gotoxy(x2 - 1,y1);
        printf("%c", rBorder);

        bgcolor(4);
        for (int i = x1 + 2; i <= x2 - 2; i++) {
            gotoxy(i, y1);
            if (i == x1 + 2) {
                printf(" ");
            } else if (i > x1 + 2 && i <= x1 + 3 + strlen(title) - 1) {
                printf("%c", title[i - (x1 + 3)]);
            } else {
                printf(" ");
            }
        }
        bgcolor(0);
    } else { //Drawing top line when no title is given
        for (int i = x1 + 1; i < x2; i++) {
        gotoxy(i, y1);
        printf("%c", hSide);
        }
    }
}

void drawBall(ball_t * ball_p) {
    gotoxy((*ball_p).x, (*ball_p).y);
    printf("%c", 15+96);
}

void updateBallPos(ball_t * ball_p, int k) {
    (*ball_p).x = (*ball_p).x + (*ball_p).vX * k;
    (*ball_p).y = (*ball_p).y + (*ball_p).vY * k;
}

