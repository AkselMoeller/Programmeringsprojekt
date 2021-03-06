#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "hardware_control.h"
#include "draw_objects.h"
#include "build_game.h"

#define MAX_COLUMNS 10
#define MAX_ROWS 10


int main(void) {
    //Variables
    int32_t x1 = 1, y1 = 1, x2 = 100, y2 = 42; //Window size
    uint16_t strikerCounter = 0;
    uint16_t strikerMaxCount = 5000; //Affects striker speed
    uint8_t bossKey = 0;
    uint16_t score = 0;
    uint8_t playerLives = 3;
    uint8_t level = 1;
    uint8_t gameIsDone = 0;
    uint8_t boxesAlive;
    uint8_t scoreboardX, scoreboardY, startX, startY, helpX, helpY;
    uint8_t menuOpen = 1; //0 = NO, 1 = YES; 2 = scoreboard open, 3 = help open
    uint8_t scoreboardSelected = 0, startSelected = 0, helpSelected = 0;
    uint8_t inGameStart = 0;
    uint8_t centerPressed = 0;
    uint8_t newHighScore = 0;
    //Init FLASH-memory
    uint32_t address = 0x0800F800; // Starting-address of the last page
    uint16_t scoreData[10] = {0};
    uint8_t writtenToScoreboard = 0;
    uint16_t lastVal = 0;

    //Initialization
    init_usb_uart(576000);
    initJoyStick();
    initPotentiometer();
    initTemperature();
    initTimer();

    //Drawing window
    x2 = readPotentiometer1();
    x2 = (((x2 - x1 - 1) / 10) * 10) + x1 + 1; //Makes the width divisible by 10
    y2 = readPotentiometer2();
    window(x1, y1, x2, y2, "Reflex Ball", 1, 1);
    deactivatePotentiometer();
    scoreboardX = (x1 + x2)/2 - (x1 + x2)/4, scoreboardY = 30;
    startX = (x1 + x2)/2 - 6, startY = 30;
    helpX = (x1 + x2)/2 + (x1 + x2)/4 - 12, helpY = 30;

    //Drawing menu labels
    drawScoreboardLabel(scoreboardX, scoreboardY, 0); //0 = black bgcolor
    drawStartLabel(startX, startY, 0);
    drawHelpLabel(helpX, helpY, 0);

    //Initializing and drawing striker
    striker_t striker;
    initStriker(&striker, x1, x2, y2);

    //Initializing and drawing ball
    ball_t ball;
    initBall(&ball, striker);
    ball.active = 1;

    ball_t ball2;
    initBall(&ball2, striker);

    //Drawing boxes
    box_t boxMatrix[MAX_COLUMNS][MAX_ROWS];
    makeLevel(boxMatrix, &ball, &ball2, &striker, x1, y1, x2, y2, level); //Drawing boxes for level 1
    deleteBall(ball); //Ball should not be visible yet
    deleteBall(ball2);

/*
    //Reset scoreboard
    FLASH_Unlock(); // Unlock FLASH for writing
    FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
    FLASH_ErasePage( address ); // Erase entire page before writing
    for (int i = 0; i < 10; i++) {
        FLASH_ProgramHalfWord(address + i * 2, 0);
    }
    FLASH_Lock();
*/

    //Initializing scoreboard
    for (int i = 0; i < 10; i++) {
        scoreData[i] = *(uint16_t *)(address + i * 2); //Reading from flash-memory
    }

    while(1) {
        if (flag && !menuOpen) { //Everything in this if-statement is executed once every 1/64 second (64 Hz refresh-rate)
            disableTimer();

            //Updating ball-position
            deleteBall(ball);
            updateBallPos(&ball);
            drawBall(ball);

            deleteBall(ball2);
            updateBallPos(&ball2);
            drawBall(ball2);

            //Making ball bounce on walls
            ballWallsCollision(&ball, &striker, x1, y1, x2, y2);
            ballWallsCollision(&ball2, &striker, x1, y1, x2, y2);

            if (!ball.active && !ball2.active) {
                playerDead(&ball, &striker, &playerLives, &inGameStart, &menuOpen, &gameIsDone, x1, y1, x2, y2, &level);
            }

            //Making ball bounce on striker
            strikerCollision(&ball, striker, boxMatrix, &score, x2);
            strikerCollision(&ball2, striker, boxMatrix, &score, x2);

            //Making ball bounce on boxes
            boxesAlive = 0;
            ballBoxesCollision(&ball, boxMatrix, &score, &boxesAlive, x2, y2);
            ballBoxesCollision(&ball2, boxMatrix, &score, &boxesAlive, x2, y2);

            if (!boxesAlive){ //When all boxes are "dead" (level-up!)
                level++;
                makeLevel(boxMatrix, &ball, &ball2, &striker, x1, y1, x2, y2, level);
                drawLevelLabel(level, x2);
                inGameStart = 1;
                drawBall(ball);
            }
            if (!inGameStart) {
                enableTimer();
            }
            flag = 0;
        }

        //Reading joystick input
        switch (readJoyStick()) {
            case 1 : //Up
                break;
            case 2 : //Down
                if (!bossKey) { //Pause game (boss key)
                    disableTimer();
                    printBossKey(score, level, playerLives);
                    bossKey = 1;
                }
                break;
            case 4 : //Left
                if (!gameIsDone) { //Only move striker when game is running
                    strikerCounter++;
                    if (striker.x > x1 + 1 && strikerCounter == strikerMaxCount && !bossKey) {
                        strikerCounter = 0;
                        updateStrikerPos(&striker, 4); //Moving striker left
                    }
                }
                break;
            case 8 : //Right
                if (!gameIsDone) { //Only move striker when game is running
                    strikerCounter++;
                    if (striker.x < x2 - striker.length && strikerCounter == strikerMaxCount && !bossKey) {
                        strikerCounter = 0;
                        updateStrikerPos(&striker, 8); //Moving striker right
                    }
                }
                break;
            case 16 : //Center
                if (!centerPressed) {
                    center(&bossKey, &menuOpen, &inGameStart, &scoreboardSelected, &newHighScore, scoreboardX, scoreboardY, startX, startY, helpX,
                        helpY, &startSelected, &helpSelected, score, level, x1, x2, y1, y2, &writtenToScoreboard, playerLives, boxMatrix, &ball, &striker, &gameIsDone, &ball2);
                    centerPressed = 1;
                }
                break;
            default : //When a button on the joystick is released
                centerPressed = 0;
                break;
        }

        if (menuOpen == 1) { //Menu is open and no item (help or scoreboard) has been selected
            if (striker.x + striker.length/2 >= scoreboardX && striker.x + striker.length/2 <= scoreboardX + 11 && !scoreboardSelected) {
                //Scoreboard selected
                drawScoreboardLabel(scoreboardX, scoreboardY, 4);
                scoreboardSelected = 1;
            } else if (striker.x + striker.length/2 >= startX && striker.x + striker.length/2 <= startX + 11 && !startSelected) {
                //Start selected
                drawStartLabel(startX, startY, 4);
                startSelected = 1;
            } else if (striker.x + striker.length/2 >= helpX && striker.x + striker.length/2 <= helpX + 11 && !helpSelected) {
                //Help selected
                drawHelpLabel(helpX, helpY, 4);
                helpSelected = 1;
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
        } else if (menuOpen == 2 && centerPressed) { //Scoreboard-button has been selected
            drawBackMessage((x2 - x1)/2, 25);
            drawScoreboard((x2 - x1)/8, 28, address);
        } else if (menuOpen == 3 && centerPressed) { //Help-button has been selected
            drawBackMessage((x2 - x1)/2, 25);
            drawHelp((x2 - x1)/16, 28);
        }

        //Writes score to scoreboard when the game has finished and if the score is great enough
        if (gameIsDone && score > scoreData[9]) {
            FLASH_Unlock(); // Unlock FLASH for writing
            FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
            FLASH_ErasePage( address ); // Erase entire page before writing
            for (int i = 0 ; i < 10 ; i++ ) {
                if (score > scoreData[i] && !writtenToScoreboard) {
                    lastVal = scoreData[i];
                    scoreData[i] = score;
                    for (int j = i; j < 10; j++) {
                        swapScores(&scoreData[1 + j], &lastVal);
                    }
                    writtenToScoreboard = 1;
                }
                FLASH_ProgramHalfWord(address + i * 2, scoreData[i]);
            }
            FLASH_Lock();
            score = 0;
            level = 1;
        }

        //Checks if the the current score is greater than the high score
        if (score > scoreData[0] && !newHighScore) {
            drawHighscoreLabel(x2); //Notifies the player if it is a new high score
            newHighScore = 1;
        }
    }
}

