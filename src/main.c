#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "hardware_control.h"
#include "draw_objects.h"
#include "vectors.h"
#include "game_control.h"

int main(void) {
    //Variables
    uint8_t x1 = 1, y1 = 1, x2 = 120, y2 = 45; //Window size (current values will produce a 4 : 3 aspect ratio)
    x2 = (((x2 - x1 - 1) / 10) * 10) + x1 + 1; //Makes the width divisible by 10
    uint8_t k = 1; //Controlling speed of ball

    //Initialization
    init_usb_uart(115200);
    initJoyStick();
    initTimer();

    //Drawing window
    window(x1, y1, x2, y2, "Breakout", 1);

    //Drawing striker
    striker_t striker;
    striker.length = (x2 - x1)/10;
    striker.x = (x1 + x2)/2 - striker.length/2;
    striker.y = y2 - 1;
    drawStriker(&striker);

    //Drawing ball
    ball_t ball;
    ball.x = striker.x + striker.length/2;
    ball.y = striker.y - 1;
    ball.vX = -1;
    ball.vY = -1;
    drawBall(&ball);

    //Drawing boxes
    box_t boxMatrix[10][5];
    for (uint8_t i = 0; i < sizeof(boxMatrix) / sizeof(boxMatrix[0]); i++) {
        for (uint8_t j = 0; j < sizeof(boxMatrix[0]) / sizeof(boxMatrix[0][0]); j++) {
            boxMatrix[i][j].xSize = (x2 - x1)/10;
            boxMatrix[i][j].ySize = (y2 - y1)/20;
            boxMatrix[i][j].x = (x1 + 1) +  boxMatrix[i][j].xSize * i;
            boxMatrix[i][j].y = (y1 + 3) + boxMatrix[i][j].ySize * j;
            boxMatrix[i][j].powerUp = 0;
            drawBox(&boxMatrix[i][j], 7);
        }
    }

    while(1) {
        if (flag) { //Everything in this if-statement is executed once every 1/20 second
            //Updating ball-position
            deleteBall(&ball);
            updateBallPos(&ball, k);
            drawBall(&ball);

            //Making ball bounce on walls
            if (ball.x <= x1 + 1 || ball.x >= x2 - 1) {
                ball.vX = -ball.vX;
            }
            if (ball.y <= y1 + 1) {
                ball.vY = -ball.vY;
            }
            if (ball.y >= y2 - 1) { //Game over!!!
                k = 0; //Another way to stop the ball from moving is by using "NVIC_DisableIRQ(TIM2_IRQn);"
            }

            //Making ball bounce on striker
            if (ball.y == striker.y - 1 && ball.x <= striker.x + striker.length && ball.x >= striker.x) {
                ball.vY = -ball.vY;
            }

            flag = 0;
        }

        //Reading joystick input
        switch (readJoyStick()) {
            case 1 : //Up
                //Other functions?
                break;
            case 2 : //Down
                //Other functions?
                break;
            case 4 : //Left
                deleteStriker(&striker);
                updateStrikerPos(&striker, 4); //Moving striker left
                if (striker.x < x1 + 1) { //Keeping striker inside window
                    striker.x = x1 + 1;
                }
                drawStriker(&striker);
                break;
            case 8 : //Right
                deleteStriker(&striker);
                updateStrikerPos(&striker, 8); //Moving striker right
                if (striker.x > x2 - striker.length) { //Keeping striker inside window
                    striker.x = x2 - striker.length;
                }
                drawStriker(&striker);
                break;
            case 16 : //Center
                TIM2->CR1 = 0x0001; //Start game
                break;
            default : //When a button on the joystick is released
                break;
        }
    }
}
