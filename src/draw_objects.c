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
#include <string.h>

void window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char * title, uint8_t style, uint8_t reset) {
    resetbgcolor();
    if (reset) {
        clrscr();
    }
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
    for (uint8_t i = x1 + 1; i < x2; i++) {
        gotoxy(i, y2);
        printf("%c", hSide);
    }

    //Drawing vertical sides (without corners)
    for (uint8_t i = y1 + 1; i < y2; i++) {
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
        for (uint8_t i = x1 + 2; i <= x2 - 2; i++) {
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

void drawBall(ball_t ball) {
    fgcolor(15);
    //Below 0.5 (0x2000 in 2.14 fixed point format) is added to coordinates in order to properly round off the numbers
    gotoxy(FIX14_right(ball.x + 0x2000), FIX14_right(ball.y + 0x2000)); //The coordinates are right-shifted 14 bits in order to draw the ball correctly
    printf("%c", 15+96); // "o"
}

void deleteBall(ball_t ball) {
    gotoxy(FIX14_right(ball.x + 0x2000), FIX14_right(ball.y + 0x2000));
    printf(" ");
}

void updateBallPos(ball_t * ball_p, uint8_t k) {
    (*ball_p).x = (*ball_p).x + (*ball_p).vX * k;
    (*ball_p).y = (*ball_p).y + (*ball_p).vY * k;
}

void drawStriker (striker_t striker) {
    //Draw striker from current position to length of striker
    fgcolor(striker.color);
    for (uint8_t i = 0; i < striker.length; i++) {
        gotoxy(striker.x + i, striker.y);
        printf("%c", 11 + 208);
    }
}

void deleteStriker(striker_t striker) {
    //Delete previous striker before new coordinates
    for (uint8_t i = 0; i < striker.length; i++) {
        gotoxy(striker.x + i, striker.y);
        printf(" ");
    }
}

void updateStrikerPos (striker_t * striker_p, uint8_t joyStickState) { //Updating the striker position with joystick
    if (joyStickState == 8) { //Move right
        gotoxy((*striker_p).x, (*striker_p).y);
        printf(" "); //Deleting leftmost element
        (*striker_p).x++; //Moving striker one element right
        gotoxy((*striker_p).x + (*striker_p).length - 1, (*striker_p).y); //Placing a new rightmost element
        fgcolor((*striker_p).color);
        printf("%c", 11 + 208);
    } else if (joyStickState == 4) { //Move left
        gotoxy((*striker_p).x + (*striker_p).length - 1, (*striker_p).y);
        printf(" "); //Deleting rightmost element
        (*striker_p).x--; //Moving striker one element left
        gotoxy((*striker_p).x, (*striker_p).y); //Placing a new leftmost element
        fgcolor(7);
        printf("%c", 11 + 208);
    }
}

void drawBox(box_t box) { //Set lives to 0 in order to delete boxes
    switch (box.lives) {
        case 0 : bgcolor(0);
            break;
        case 1 : bgcolor(3);
            break;
        case 2 : bgcolor(7);
            break;
        default :
            break;
    }
    for (uint8_t i = 0; i < box.xSize; i++) {
        for (uint8_t j = 0; j < box.ySize; j++) {
            gotoxy(box.x + i, box.y + j);
            if (box.powerUp){
                printf("o")
            } else{
                printf(" ");
            }
        }
    }
    resetbgcolor();
}

void drawScoreLabel(uint16_t score) {
    gotoxy(100, 2);
    fgcolor(7);
    printf("Score: %i", score);
}

void drawLevelLabel(uint8_t level) {
    gotoxy(100, 3);
    fgcolor(7);
    printf("Level: %i", level);

}

void drawPlayerLivesLabel (uint8_t playerLives) {
    gotoxy (100,1);
    bgcolor(4);
    if (playerLives == 3) {
        fgcolor(9);
        printf("<3 <3 <3");
    } else if (playerLives == 2) {
        fgcolor(9);
        printf("<3 <3");
        fgcolor(7);
        printf("  X");
    } else if (playerLives == 1) {
        fgcolor(9);
        printf("<3");
        fgcolor(7);
        printf("  X  X");
    } else {
        fgcolor(7);
        printf(" X  X  X");
    }
    resetbgcolor();
    fgcolor(15);
}

void drawScoreboardLabel(uint8_t scoreboardX, uint8_t scoreboardY, uint8_t color, uint8_t backButton) {
    gotoxy(scoreboardX , scoreboardY);
    window(scoreboardX, scoreboardY, scoreboardX + 11, scoreboardY + 2, "", 0, 0);
    bgcolor(color);
    gotoxy(scoreboardX + 1, scoreboardY + 1);
    if (!backButton) {
        printf("Scoreboard");
    } else if (backButton == 1) {
        printf("   Back   ");
    }
    resetbgcolor();
}

void drawStartLabel(uint8_t startX, uint8_t startY, uint8_t color) {
    gotoxy(startX, startY);
    window(startX, startY, startX + 11, startY + 2, "", 0, 0);
    bgcolor(color);
    gotoxy(startX + 1, startY + 1);
    printf("Start game");
    resetbgcolor();
}

void drawHelpLabel(uint8_t helpX, uint8_t helpY, uint8_t color, uint8_t backButton) {
    gotoxy(helpX, helpY);
    window(helpX, helpY, helpX + 11, helpY + 2, "", 0, 0);
    bgcolor(color);
    gotoxy(helpX + 1, helpY + 1);
    if (!backButton) {
        printf("   Help   ");
    } else if (backButton == 2) {
        printf("   Back   ");
    }
    resetbgcolor();
}

void deleteMenuLabels(uint8_t scoreboardX, uint8_t scoreboardY, uint8_t startX, uint8_t startY, uint8_t helpX, uint8_t helpY) {
    resetbgcolor();
    for (uint8_t i = 0; i <= 2; i++) {
        gotoxy(scoreboardX, scoreboardY + i);
        printf("            ");
        gotoxy(startX, startY + i);
        printf("            ");
        gotoxy(helpX, helpY + i);
        printf("            ");
    }
}

void printBossKey(uint16_t score, uint8_t playerLives) {
    //Clear screen and move courser to (1,1)
    clrscr();
    gotoxy(1,1);

    //fgcolor(2) Sets foreground color green (used for "code text")
    //fgcolor(8) Sets foreground color dark gray (used for "commentary text")
    fgcolor(2);
    printf ("#include \"stm32f30x_conf.h\"\n #include \"30010_io.h\"\n#include \"hardware_control.h\"\n#include \"draw_objects.h\"\n#include \"game_control.h\"");

    //Prints striker code
    printf("\n\nstriker_t initStriker(int32_t x1, int32_t x2, int32_t y2) {\n");
    fgcolor(8);
    printf("    //Drawing striker\n" );
    fgcolor(2);
    printf("    striker_t striker;\n");
    printf("    striker.length = (x2 - x1)/10;\n");
    printf("    striker.x = (x1 + x2)/2 - striker.length/2;\n");
    printf("    striker.y = y2 - 1;\n");
    printf("    drawStriker(&striker);\n");
    printf("    return striker;\n}\n");
    printf("\n");

    //Prints initBall code
    printf("ball_t initBall(striker_t striker) {\n");
    fgcolor(8);
    printf("    //Drawing Ball\n");
    fgcolor(2);
    printf("    ball_t ball;\n");
    fgcolor(8);
    printf("    //Ball position- and velocity coordinates left-shifted 14 bits in order to produce 18.14-fixed point numbers\n");
    fgcolor(2);
    printf("    ball.x = FIX14_left(striker.x + striker.length/2);\n");
    printf("    ball.y = FIX14_left(striker.y - 2);\n");
    printf("    ball.vX = 0x00000000;\n");
    printf("    ball.vY = 0xFFFFF000; //-0.25\n");
    printf("    drawBall(&ball);\n");
    printf("    return ball;\n}\n");
    printf("\n");

    //Prints main() code ll. 29 - 40
    printf("int main(void) {\n");
    fgcolor(8);
    printf("    //Variables\n");
    fgcolor(2);
    printf("    int32_t x1 = 1, y1 = 1, x2 = 120, y2 = 45;");
    fgcolor(8);
    printf("//Window size (current values will produce a 4 : 3 aspect ratio)\n");
    fgcolor(2);
    printf("    x2 = (((x2 - x1 - 1) / 10) * 10) + x1 + 1;");
    fgcolor(8);
    printf("//Makes the width divisible by 10\n");
    fgcolor(2);
    printf("    uint8_t k = 1;");
    fgcolor(8);
    printf("//Controlling speed of ball\n");
    fgcolor(2);
    printf("    uint16_t strikerCounter = 0;\n    uint16_t strikerMaxCount = 8500;");
    fgcolor(8);
    printf("//Affects striker speed\n");
    fgcolor(2);
    printf("    uint8_t boxColumns = 10;");
    fgcolor(8);
    printf("//Number of boxes along the x-axis\n");
    fgcolor(2);
    printf("    uint8_t boxRows = 5");
    fgcolor(8);
    printf("//Number of boxes along the y-axis\n");
    fgcolor(2);
    printf("    uint8_t bossKey = ");
    fgcolor(4);
    printf("SFW-mode (safe for work) is now ACTIVE, press center-buttom to resume game\n");
    fgcolor (2);
    printf("    uint8_t score = ");
    fgcolor(4);
    printf("%i\n", score);
    fgcolor(4);
    printf("    uint8_t playerLives = ");
    fgcolor(2);
    printf("%i\n", playerLives);
}

void gameOver(int32_t x1, int32_t x2, int32_t y1, int32_t y2) {

    //Coordinates for center-point of screen
    uint8_t xm = (x2 - x1)/2;
    uint8_t ym = (y2 - y1)/2;

    //Sets filled box character
    uint8_t box = 208+11;
    uint8_t halfbox = 208+12;
    uint8_t upperhalfbox = 208 + 15;

    //start coordinates for G
    uint8_t xGs = xm - (5*5);
    uint8_t yGs = ym - 3;

    //Sets color for G (Cyan)
    fgcolor(6);

    //Drawing the G
    gotoxy(xGs + 1, yGs);
    printf("%c%c%c\n", box, box, box);
    gotoxy(xGs, yGs + 1);
    printf("%c\n", box);
    gotoxy(xGs, yGs + 2);
    printf("%c %c%c\n", box, halfbox, halfbox);
    gotoxy(xGs, yGs + 3);
    printf("%c   %c\n", box, box);
    gotoxy(xGs + 1, yGs + 4);
    printf("%c%c%c\n", box, box, box);


    //Start-coordinates for A
    uint8_t xAs = xGs + 6;
    uint8_t yAs = yGs;

    //Sets color for A (Cyan)
    fgcolor(6);

    //Drawing the A
    gotoxy(xAs + 1, yAs);
    printf("%c%c%c\n", box, box, box);
    gotoxy(xAs, yAs + 1);
    printf("%c   %c\n", box, box);
    gotoxy(xAs, yAs + 2);
    printf("%c%c%c%c%c\n", box, halfbox, halfbox, halfbox, box);
    gotoxy(xAs, yAs + 3);
    printf("%c   %c\n", box, box);
    gotoxy(xAs, yAs + 4);
    printf("%c   %c\n", box, box);


    //Start-coordinates for M
    uint8_t xMs = xAs + 6;
    uint8_t yMs = yAs;

    //Sets color for M (Cyan)
    fgcolor(6);

    //Drawing the M
    gotoxy(xMs, yMs);
    printf("%c   %c\n", box, box);
    gotoxy(xMs, yMs + 1);
    printf("%c%c %c%c\n", box, box, box, box);
    gotoxy(xMs, yMs + 2);
    printf("%c %c %c\n", box, box, box);
    gotoxy(xMs, yMs + 3);
    printf("%c   %c\n", box, box);
    gotoxy(xMs, yMs + 4);
    printf("%c   %c\n", box, box);


    //Start-coordinates for E
    uint8_t xEs = xMs + 6;
    uint8_t yEs = yMs;

    //Sets color for E (Cyan)
    fgcolor(6);

    //Drawing the E
    gotoxy(xEs, yEs);
    printf("%c%c%c%c%c\n", box, box, box, box, box);
    gotoxy(xEs, yEs + 1);
    printf("%c\n", box);
    gotoxy(xEs, yEs + 2);
    printf("%c%c%c\n", box, halfbox, halfbox);
    gotoxy(xEs, yEs + 3);
    printf("%c\n", box);
    gotoxy(xEs, yEs + 4);
    printf("%c%c%c%c%c\n", box, box, box, box, box);

    //start coordinates for O
    uint8_t xOs = xEs + 13;
    uint8_t yOs = yEs;

    //Sets color for O (Cyan)
    fgcolor(6);

    //Drawing the O
    gotoxy(xOs + 1, yOs);
    printf("%c%c%c\n", box, box, box);
    gotoxy(xOs, yOs + 1);
    printf("%c   %c\n", box, box);
    gotoxy(xOs, yOs + 2);
    printf("%c   %c\n", box, box);
    gotoxy(xOs, yOs + 3);
    printf("%c   %c\n", box, box);
    gotoxy(xOs + 1, yOs + 4);
    printf("%c%c%c\n", box, box, box);


    //Start-coordinates for V
    uint8_t xVs = xOs + 6;
    uint8_t yVs = yOs;

    //Sets color for V (Cyan)
    fgcolor(6);

    //Drawing the V
    gotoxy(xVs, yVs);
    printf("%c   %c\n", box, box);
    gotoxy(xVs, yVs + 1);
    printf("%c   %c\n", box, box);
    gotoxy(xVs + 1, yVs + 2);
    printf("%c %c\n", box, box);
    gotoxy(xVs + 1, yVs + 3);
    printf("%c %c\n", box, box);
    gotoxy(xVs + 2, yVs + 4);
    printf("%c\n", box);


    //Start-coordinates for E2
    uint8_t xE2s = xVs + 6;
    uint8_t yE2s = yVs;

    //Sets color for E2 (Cyan)
    fgcolor(6);

    //Drawing the E2
    gotoxy(xE2s, yE2s);
    printf("%c%c%c%c%c\n", box, box, box, box, box);
    gotoxy(xE2s, yE2s + 1);
    printf("%c\n", box);
    gotoxy(xE2s, yE2s + 2);
    printf("%c%c%c\n", box, halfbox, halfbox);
    gotoxy(xE2s, yE2s + 3);
    printf("%c\n", box);
    gotoxy(xE2s, yE2s + 4);
    printf("%c%c%c%c%c\n", box, box, box, box, box);


    //Start-coordinates for R
    uint8_t xRs = xE2s + 6;
    uint8_t yRs = yE2s;

    //Sets color for R (Cyan)
    fgcolor(6);

    //Drawing the R
    gotoxy(xRs, yRs);
    printf("%c%c%c%c\n", box, box, box, halfbox);
    gotoxy(xRs, yRs + 1);
    printf("%c   %c\n", box, box);
    gotoxy(xRs, yRs + 2);
    printf("%c %c%c\n", box, halfbox, upperhalfbox);
    gotoxy(xRs, yRs + 3);
    printf("%c  %c\n", box, box);
    gotoxy(xRs, yRs + 4);
    printf("%c   %c\n", box, box);

}


void printHelp(uint8_t x, uint8_t y) {
    //This prints help instructions to screen
    gotoxy(x, y);
    fgcolor(7);
    printf("Here is some help: ");
    gotoxy(x, y + 2);
    printf("Move the joystick left end right to move the striker in th bottom of the screen.");
    gotoxy(x, y + 3);
    printf("Press the center of joystick to either select any marked button, or start the game and get the ball moving again.");
    gotoxy(x, y + 4);
    printf("Press down on the joystick to activate SW (safe for work) mode. Also known as the \"Boss Key\".");
    gotoxy(x, y + 5);
    printf("If Scoreboard is selected (using the center button) a list of the highest scores will be shown.")

}

void printScoreboard(uint8_t x, uint8_t y, int scoreboard[5]) {
    gotoxy(x, y);
    fgcolor(7);
    printf("Here is the top scores: ");
    gotoxy(x, y + 2);
    printf("1: %i", scoreboard[0]);
    gotoxy(x, y + 3);
    printf("2: %i", scoreboard[1]);
    gotoxy(x, y + 4);
    printf("3: %i", scoreboard[2]);
    gotoxy(x, y + 5);
    printf("4: %i", scoreboard[3]);
    gotoxy(x, y + 6);
    printf("4: %i", scoreboard[4]);
}
