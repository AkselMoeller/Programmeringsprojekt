#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "hardware_control.h"
#include "draw_objects.h"
#include "game_control.h"

void makeLevel() {

}

striker_t initStriker(int32_t x1, int32_t x2, int32_t y2) {
    //Drawing striker
    striker_t striker;
    striker.length = (x2 - x1)/10;
    striker.x = (x1 + x2)/2 - striker.length/2;
    striker.y = y2 - 1;
    drawStriker(&striker);
    return striker;
}

ball_t initBall(striker_t striker) {
    //Drawing ball
    ball_t ball;
    //Ball position- and velocity coordinates left-shifted 14 bits in order to produce 18.14-fixed point numbers
    ball.x = FIX14_left(striker.x + striker.length/2);
    ball.y = FIX14_left(striker.y - 2);
    ball.vX = 0x00000000;
    ball.vY = 0xFFFFF000; //-0.25
    drawBall(&ball);
    return ball;
}

int main(void) {
    //Variables
    int32_t x1 = 1, y1 = 1, x2 = 120, y2 = 45; //Window size (current values will produce a 4 : 3 aspect ratio)
    x2 = (((x2 - x1 - 1) / 10) * 10) + x1 + 1; //Makes the width divisible by 10
    uint8_t k = 1; //Controlling speed of ball
    uint16_t strikerCounter = 0;
    uint16_t strikerMaxCount = 8500; //Affects striker speed
    uint8_t boxMaxColumns = 10; //Number of boxes along the x-axis
    uint8_t boxMaxRows = 10; //Number of boxes along the y-axis
    uint8_t bossKey = 0;
    uint8_t score = 0;
    uint8_t level = 0;

    //Initialization
    init_usb_uart(115200);
    initJoyStick();
    initTimer();

    //Drawing window
    window(x1, y1, x2, y2, "Breakout", 1, 1);

    //Drawing help and scoreboard labels
    drawMenuLabels((x1 + x2)/2 - (x1 + x2)/4 - 12, 25, (x1 + x2)/2 + (x1 + x2)/4, 25);

    //Initializing and drawing striker
    striker_t striker = initStriker(x1, x2, y2);

    //Initializing and drawing ball
    ball_t ball = initBall(striker);

    //Drawing boxes
    box_t boxMatrix[boxMaxColumns][boxMaxRows];
    for (uint8_t i = 0; i < sizeof(boxMatrix) / sizeof(boxMatrix[0]); i++) {
        for (uint8_t j = 0; j < sizeof(boxMatrix[0]) / sizeof(boxMatrix[0][0]); j++) {
            boxMatrix[i][j].xSize = (x2 - x1)/10;
            boxMatrix[i][j].ySize = (y2 - y1)/20;
            boxMatrix[i][j].x = (x1 + 1) +  boxMatrix[i][j].xSize * i;
            boxMatrix[i][j].y = (y1 + 3) + boxMatrix[i][j].ySize * j;
            boxMatrix[i][j].powerUp = 0;
            boxMatrix[i][j].lives = 1;
            drawBox(&boxMatrix[i][j]);
        }
    }

    while(1) {
        if (flag) { //Everything in this if-statement is executed once every 1/20 second
            TIM2->CR1 = 0x0000; //Disabling timer

            //Updating ball-position
            deleteBall(&ball);
            updateBallPos(&ball, k);
            drawBall(&ball);

            //Making ball bounce on walls
            if (ball.x <= FIX14_left(x1 + 1) || ball.x >= FIX14_left(x2 - 1)) {
                ball.vX = -ball.vX;
            }
            if (ball.y <= FIX14_left(y1 + 1)) {
                ball.vY = -ball.vY;
            }
            if (ball.y >= FIX14_left(y2 - 1)) { //Game over!!!
                k = 0; //Another way to stop the ball from moving is by using "NVIC_DisableIRQ(TIM2_IRQn);"
            }

            //Making ball bounce on striker
            if (FIX14_right(ball.y) == striker.y - 1
                && FIX14_right(ball.x + 0x2000) <= striker.x + striker.length
                && FIX14_right(ball.x + 0x2000) >= striker.x) {
                for (int i = 0; i < striker.length; i++) {
                    if (FIX14_right(ball.x + 0x2000) == striker.x + i) {
                        //Do stuff
                        if (ball.vY > 0) {
                            ball.vY = -ball.vY;
                            ball.vX += -0x2000 + (0x2000 * 2)/(striker.length - 1) * i;
                            if (ball.vX >= 0x2000) {
                                ball.vX = 0x2000;
                            }
                            if (ball.vX <= -0x2000) {
                                ball.vX = -0x2000;
                            }
                        }
                    }
                }
            }

            //Making ball bounce on boxes
            for (uint8_t i = 0; i < boxMaxColumns; i++) {
                for (uint8_t j = 0; j < boxMaxRows; j++) {
                    if (boxMatrix[i][j].lives) { //Only executed if the box is "alive"
                        //Checking if ball hits the LEFT side of the box
                        if (ball.y >= FIX14_left(boxMatrix[i][j].y)
                            && FIX14_left(ball.y < boxMatrix[i][j].y + boxMatrix[i][j].ySize)
                            && ball.x >= FIX14_left(boxMatrix[i][j].x)
                            && ball.x < FIX14_left(boxMatrix[i][j].x) + 0x2000
                            && ball.vX > 0) {
                                ball.vX = -ball.vX;
                                boxMatrix[i][j].lives--;
                                drawBox(&boxMatrix[i][j]);
                                score ++;
                                drawScore(score);
                        }
                        //Checking if ball hits the RIGHT side of the box
                        if (ball.y >= FIX14_left(boxMatrix[i][j].y)
                            && FIX14_left(ball.y < boxMatrix[i][j].y + boxMatrix[i][j].ySize)
                            && ball.x <= FIX14_left(boxMatrix[i][j].x + boxMatrix[i][j].xSize)
                            && ball.x > FIX14_left(boxMatrix[i][j].x + boxMatrix[i][j].xSize) - 0x2000
                            && ball.vX < 0) {
                                ball.vX = -ball.vX;
                                boxMatrix[i][j].lives--;
                                drawBox(&boxMatrix[i][j]);
                                score ++;
                                drawScore(score);
                        }
                        //Checking if ball hits the TOP side of the box
                        if (ball.x >= FIX14_left(boxMatrix[i][j].x)
                            && FIX14_left(ball.x < boxMatrix[i][j].x + boxMatrix[i][j].xSize)
                            && ball.y >= FIX14_left(boxMatrix[i][j].y)
                            && ball.y < FIX14_left(boxMatrix[i][j].y) + 0x2000
                            && ball.vY > 0) {
                                ball.vY = -ball.vY;
                                boxMatrix[i][j].lives--;
                                drawBox(&boxMatrix[i][j]);
                                score ++;
                                drawScore(score);
                        }
                        //Checking if ball hits the BOTTOM sides of the box
                        if (ball.x >= FIX14_left(boxMatrix[i][j].x)
                            && FIX14_left(ball.x < boxMatrix[i][j].x + boxMatrix[i][j].xSize)
                            && ball.y <= FIX14_left(boxMatrix[i][j].y + boxMatrix[i][j].ySize)
                            && ball.y > FIX14_left(boxMatrix[i][j].y + boxMatrix[i][j].ySize) - 0x2000
                            && ball.vY < 0) {
                                ball.vY = -ball.vY;
                                boxMatrix[i][j].lives--;
                                drawBox(&boxMatrix[i][j]);
                                score ++;
                                drawScore(score);
                        }
                    }
                }
            }
            TIM2->CR1 = 0x0001; //Enabling timer
            flag = 0;
        }

        //Reading joystick input
        switch (readJoyStick()) {
            case 1 : //Up
                break;
            case 2 : //Down
                if (!bossKey) { //Pause game (boss key)
                    TIM2->CR1 = 0x0000;
                    clrscr();
                    printBossKey(score);
                    bossKey = 1;
                }
                break;
            case 4 : //Left
                strikerCounter++;
                if (strikerCounter == strikerMaxCount && striker.x > x1 + 1 && !bossKey) {
                    strikerCounter = 0;
                    updateStrikerPos(&striker, 4); //Moving striker left
                }
                break;
            case 8 : //Right
                strikerCounter++;
                if (strikerCounter == strikerMaxCount && striker.x < x2 - striker.length && !bossKey) {
                    strikerCounter = 0;
                    updateStrikerPos(&striker, 8); //Moving striker right
                }
                break;
            case 16 : //Center
                if (!bossKey) { //Start game
                    TIM2->CR1 = 0x0001;
                } else { //Resume game
                    window(x1, y1, x2, y2, "Breakout", 1, 1);
                    for (uint8_t i = 0; i < sizeof(boxMatrix) / sizeof(boxMatrix[0]); i++) {
                        for (uint8_t j = 0; j < sizeof(boxMatrix[0]) / sizeof(boxMatrix[0][0]); j++) {
                            drawBox(&boxMatrix[i][j]);
                        }
                    }
                    drawScore(score);
                    drawStriker(&striker);
                    drawBall(&ball);
                    TIM2->CR1 = 0x0001;
                    bossKey = 0;
                }
                break;
            default : //When a button on the joystick is released
                break;
        }
    }
}
