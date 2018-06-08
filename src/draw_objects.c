//
//  draw_objects.c
//  Programmeringsprojekt
//
//  Created by Alexander Ohrt on 08/06/2018.
//  Copyright © 2018 Alexander Ohrt. All rights reserved.
//

#include <stdio.h>
#include "draw_objects.h"
#include "ansi.h"

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

void drawStriker (striker_t * striker_p) {
<<<<<<< HEAD
  fgcolor(0);
  for (int i = (*striker_p).x ; i < (*striker_p).x + length; i++) {
      gotoxy((*striker_p).x + i, (*striker_p).y);
      printf(" ");
}

void updateStrikerPos (striker_t * striker_p, int joyStickState) {
  if (joyStickState == 8) {
    (*striker_p).x ++;
  }
  else if (joyStickState == 4) {
      (*striker_p).x --;
  }
  else {
    (*striker_p).x = (*striker_p).x;
  }
=======
    fgcolor(0)
    for (int i = (*striker_p).x ; i < (*striker_p).x + length; i++) {
        gotoxy((*striker_p).x + i, (*striker_p).y);
        printf(" ");
}

void updateStrikerPos (striker_t * striker_p) {
    
>>>>>>> 4ea89d93d788ae1d0c21b015520386bfacd16b53
}
