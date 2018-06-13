#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "hardware_control.h"
#include "draw_objects.h"

#define MAX_COLUMNS 10
#define MAX_ROWS 10

void makeLevel(box_t boxMatrix[MAX_COLUMNS][MAX_ROWS], ball_t * ball_p, striker_t * striker_p, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t level) {
    TIM2->CR1 = 0x0000;
    deleteBall(*ball_p);
    deleteStriker(*striker_p);

    //level making
    for (uint8_t i = 0; i < MAX_COLUMNS; i++) { //init all boxes size and position
        for (uint8_t j = 0; j < MAX_ROWS; j++) {
            boxMatrix[i][j].xSize = (x2 - x1)/10;
            boxMatrix[i][j].ySize = (y2 - y1)/20;
            boxMatrix[i][j].x = (x1 + 1) +  boxMatrix[i][j].xSize * i;
            boxMatrix[i][j].y = (y1 + 3) + boxMatrix[i][j].ySize * j;
            boxMatrix[i][j].powerUp = 0;

            //level design
            switch (level) {
                case 1 :
                    if ((j == 5 || j == 4) && i == 4){
                        boxMatrix[i][j].lives = 1;
                    } else {
                        boxMatrix[i][j].lives = 0;
                    }
                    break;
                case 2 :
                    if (j < 4 && ((j%2 && (i+1)%2) || ((j+1)%2 && i%2))) {
                         boxMatrix[i][j].lives = 1;
                    } else {
                        boxMatrix[i][j].lives = 0;
                    }
                    break;
            }
            drawBox(boxMatrix[i][j]); //draw all boxes
        }
    }

    (*striker_p).x = (x1 + x2)/2 - (*striker_p).length/2;
    drawStriker(*striker_p);

    initBall(ball_p, *striker_p);

}

void initStriker(striker_t * striker_p, int32_t x1, int32_t x2, int32_t y2) {
    //Drawing striker
    (*striker_p).color = 7;
    (*striker_p).length = (x2 - x1)/10;
    (*striker_p).x = (x1 + x2)/2 - (*striker_p).length/2;
    (*striker_p).y = y2 - 1;
    drawStriker(*striker_p);
}

void initBall(ball_t * ball_p, striker_t striker) {
    //Ball position- and velocity coordinates left-shifted 14 bits in order to produce 18.14-fixed point numbers
    (*ball_p).x = FIX14_left(striker.x + striker.length/2);
    (*ball_p).y = FIX14_left(striker.y - 2);
    (*ball_p).vX = 0x00000000;
    (*ball_p).vY = 0xFFFFF000; //-0.25
    drawBall(*ball_p);
}

int main(void) {
    //Variables
    int32_t x1 = 1, y1 = 1, x2 = 120, y2 = 45; //Window size (current values will produce a 4 : 3 aspect ratio)
    x2 = (((x2 - x1 - 1) / 10) * 10) + x1 + 1; //Makes the width divisible by 10
    uint8_t k = 1; //Controlling speed of ball
    uint16_t strikerCounter = 0;
    uint16_t strikerMaxCount = 8500; //Affects striker speed
    uint8_t bossKey = 0;
    uint16_t score = 0;
    uint8_t playerLives = 3;
    uint8_t level = 1;
    uint8_t boxesAlive;
    uint8_t menuOpen = 1; //1 = YES, 0 = NO
    uint8_t scoreboardX = (x1 + x2)/2 - (x1 + x2)/4, scoreboardY = 25;
    uint8_t startX = (x1 + x2)/2 - 6, startY = 25;
    uint8_t helpX = (x1 + x2)/2 + (x1 + x2)/4 - 12, helpY = 25;
    uint8_t scoreboardSelected = 0, startSelected = 0, helpSelected = 0;
    uint8_t inGameStart = 0;


    //Initialization
    init_usb_uart(115200);
    initJoyStick();
    initTimer();

    //Drawing window
    window(x1, y1, x2, y2, "Breakout", 1, 1);

    //Drawing menu labels
    drawScoreboardLabel(scoreboardX, scoreboardY, 0); //0 = black bgcolor
    drawStartLabel(startX, startY, 0);
    drawHelpLabel(helpX, helpY, 0);
    drawPlayerLivesLable(playerLives);

    //Initializing and drawing striker
    striker_t striker;
    initStriker(&striker, x1, x2, y2);

    //Initializing and drawing ball
    ball_t ball;
    initBall(&ball, striker);

    //Drawing boxes
    box_t boxMatrix[MAX_COLUMNS][MAX_ROWS];
    makeLevel(boxMatrix, &ball, &striker, x1, y1, x2, y2, level); //Drawing boxes for level 1
    deleteBall(ball); //Ball should not be visible yet

    while(1) {
        if (flag && !menuOpen) { //Everything in this if-statement is executed once every 1/20 second
            TIM2->CR1 = 0x0000; //Disabling timer

            //Updating ball-position
            deleteBall(ball);
            updateBallPos(&ball, k);
            drawBall(ball);

            //Making ball bounce on walls
            if (ball.x <= FIX14_left(x1 + 1) || ball.x >= FIX14_left(x2 - 1)) {
                ball.vX = -ball.vX;
            }
            if (ball.y <= FIX14_left(y1 + 1)) {
                ball.vY = -ball.vY;
            }
            if (ball.y >= FIX14_left(y2 - 1) && k) { //Game over!!!
                playerLives --; //Decrement player lives
                drawPlayerLivesLable(playerLives); //Output player lives to putty

                //Resets ball and striker
                deleteStriker(striker);
                striker.x = (x1 + x2)/2 - striker.length/2;
                striker.y = y2 - 1;
                deleteBall(ball);
                ball.vY = -ball.vY;
                ball.x = FIX14_left(striker.x + striker.length/2);
                ball.y = FIX14_left(striker.y - 2);
                drawBall(ball);
                drawStriker(striker);

                if (!playerLives) {
                gameOver(x1, x2, y1, y2);
                menuOpen = 1;
                }
                inGameStart = 1;
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
            boxesAlive = 0;
            for (uint8_t i = 0; i < MAX_COLUMNS; i++) {
                for (uint8_t j = 0; j < MAX_ROWS; j++) {
                    if (boxMatrix[i][j].lives > 0) { //Only executed if the box is "alive"
                        boxesAlive++;
                        //Checking if ball hits the TOP side of the box
                        if (ball.x >= FIX14_left(boxMatrix[i][j].x)
                            && FIX14_left(ball.x < boxMatrix[i][j].x + boxMatrix[i][j].xSize)
                            && ball.y >= FIX14_left(boxMatrix[i][j].y)
                            && ball.y < FIX14_left(boxMatrix[i][j].y) + 0x2000
                            && ball.vY > 0) {
                                ball.vY = -ball.vY;
                                boxMatrix[i][j].lives--;
                                drawBox(boxMatrix[i][j]);
                                score++;
                                drawScoreLabel(score);
                        }
                        //Checking if ball hits the BOTTOM sides of the box
                        else if (ball.x >= FIX14_left(boxMatrix[i][j].x)
                            && FIX14_left(ball.x < boxMatrix[i][j].x + boxMatrix[i][j].xSize)
                            && ball.y <= FIX14_left(boxMatrix[i][j].y + boxMatrix[i][j].ySize)
                            && ball.y > FIX14_left(boxMatrix[i][j].y + boxMatrix[i][j].ySize) - 0x2000
                            && ball.vY < 0) {
                                ball.vY = -ball.vY;
                                boxMatrix[i][j].lives--;
                                drawBox(boxMatrix[i][j]);
                                score++;
                                drawScoreLabel(score);
                        }
                        //Checking if ball hits the LEFT side of the box
                        else if (ball.y >= FIX14_left(boxMatrix[i][j].y)
                            && FIX14_left(ball.y < boxMatrix[i][j].y + boxMatrix[i][j].ySize)
                            && ball.x >= FIX14_left(boxMatrix[i][j].x)
                            && ball.x < FIX14_left(boxMatrix[i][j].x) + 0x2000
                            && ball.vX > 0) {
                                ball.vX = -ball.vX;
                                boxMatrix[i][j].lives--;
                                drawBox(boxMatrix[i][j]);
                                score++;
                                drawScoreLabel(score);
                        }
                        //Checking if ball hits the RIGHT side of the box
                        else if (ball.y >= FIX14_left(boxMatrix[i][j].y)
                            && FIX14_left(ball.y < boxMatrix[i][j].y + boxMatrix[i][j].ySize)
                            && ball.x <= FIX14_left(boxMatrix[i][j].x + boxMatrix[i][j].xSize)
                            && ball.x > FIX14_left(boxMatrix[i][j].x + boxMatrix[i][j].xSize) - 0x2000
                            && ball.vX < 0) {
                                ball.vX = -ball.vX;
                                boxMatrix[i][j].lives--;
                                drawBox(boxMatrix[i][j]);
                                score++;
                                drawScoreLabel(score);
                        }
                    }
                }
            }
            if (!boxesAlive){
                level++;
                makeLevel(boxMatrix, &ball, &striker, x1, y1, x2, y2, level);
                drawLevelLabel(level);
                inGameStart = 1;
            }
            if (!inGameStart) {
                TIM2->CR1 = 0x0001; //Enabling timer
            }
            flag = 0;
        }

        //Reading joystick input
        switch (readJoyStick()) {
            case 1 : //Up
                break;
            case 2 : //Down
                if (!bossKey) { //Pause game (boss key)
                    TIM2->CR1 = 0x0000;
                    printBossKey(score, playerLives);
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
                if (!bossKey && (menuOpen || inGameStart)) {
                    if (scoreboardSelected) { //Show scoreboard
                        deleteMenuLabels(scoreboardX, scoreboardY, startX, startY, helpX, helpY);
                        drawBackLabel(scoreboardX, scoreboardY, 0);
                    } else if (startSelected || inGameStart) { //Start game
                        deleteMenuLabels(scoreboardX, scoreboardY, helpX, helpY, startX, startY);
                        drawScoreLabel(score);
                        drawLevelLabel(level);
                        drawPlayerLivesLable(playerLives);
                        inGameStart = 0;
                        TIM2->CR1 = 0x0001;
                    } else if (helpSelected) { //Show help page
                        deleteMenuLabels(helpX, scoreboardY, startX, startY, helpX, helpY);
                        drawBackLabel(helpX, helpY, 0);
                    }
                    menuOpen = 0;
                } else if (bossKey && !menuOpen) { //Resume game
                    window(x1, y1, x2, y2, "Breakout", 1, 1);
                    for (uint8_t i = 0; i < MAX_COLUMNS; i++) {
                        for (uint8_t j = 0; j < MAX_ROWS; j++) {
                            drawBox(boxMatrix[i][j]);
                        }
                    }
                    drawScoreLabel(score);
                    drawLevelLabel(level);
                    drawStriker(striker);
                    drawBall(ball);
                    TIM2->CR1 = 0x0001;
                    bossKey = 0;
                } else if (bossKey && menuOpen) { //When the menu-page should be open
                    window(x1, y1, x2, y2, "Breakout", 1, 1);
                    for (uint8_t i = 0; i < MAX_COLUMNS; i++) {
                        for (uint8_t j = 0; j < MAX_ROWS; j++) {
                            drawBox(boxMatrix[i][j]);
                        }
                    }
                    drawStriker(striker);
                    drawScoreboardLabel(scoreboardX, scoreboardY, 0);
                    drawStartLabel(startX, startY, 0);
                    drawHelpLabel(helpX, helpY, 0);
                    scoreboardSelected = 0;
                    startSelected = 0;
                    helpSelected = 0;
                    bossKey = 0;
                }
                break;
            default : //When a button on the joystick is released
                break;
        }

        if (menuOpen) { //Enabling player to choose between menu items
            if (striker.x + striker.length/2 >= scoreboardX && striker.x + striker.length/2 <= scoreboardX + 11 && !scoreboardSelected) {
                //Scoreboard selected
                drawScoreboardLabel(scoreboardX, scoreboardY, 4);
                scoreboardSelected = 1;
                startSelected = 0;
                helpSelected = 0;
            } else if (striker.x + striker.length/2 >= startX && striker.x + striker.length/2 <= startX + 11 && !startSelected) {
                //Start selected
                drawStartLabel(startX, startY, 4);
                startSelected = 1;
                scoreboardSelected = 0;
                helpSelected = 0;
            } else if (striker.x + striker.length/2 >= helpX && striker.x + striker.length/2 <= helpX + 11 && !helpSelected) {
                //Help selected
                drawHelpLabel(helpX, helpY, 4);
                helpSelected = 1;
                scoreboardSelected = 0;
                startSelected = 0;
            } else if ((scoreboardSelected || startSelected || helpSelected)
                        && (striker.x + striker.length/2 < scoreboardX
                        || (striker.x + striker.length/2 > scoreboardX + 11 && striker.x + striker.length/2 < startX)
                        || (striker.x + striker.length/2 > startX + 11 && striker.x + striker.length/2 < helpX)
                        || striker.x + striker.length/2 > helpX + 11)) {
                //When NO items should be selected
                drawScoreboardLabel(scoreboardX, scoreboardY, 0);
                drawStartLabel(startX, startY, 0);
                drawHelpLabel(helpX, helpY, 0);
                scoreboardSelected = 0;
                startSelected = 0;
                helpSelected = 0;
            }
        }
    }
}

