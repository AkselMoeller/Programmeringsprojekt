#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "hardware_control.h"
#include "draw_objects.h"
#include "build_game.h"

#define MAX_COLUMNS 10
#define MAX_ROWS 10

void ballWallsCollision(ball_t * ball_p, striker_t * striker_p,
                        uint8_t * playerLives_p, uint8_t * inGameStart_p, uint8_t * menuOpen_p, uint8_t k, uint8_t * level_p, uint8_t * gameIsDone,
                        int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    if ((*ball_p).x <= FIX14_left(x1 + 1) || (*ball_p).x >= FIX14_left(x2 - 1)) {
        (*ball_p).vX = -(*ball_p).vX;
    }
    if ((*ball_p).y <= FIX14_left(y1 + 1)) {
        (*ball_p).vY = -(*ball_p).vY;
    }
    if ((*ball_p).y >= FIX14_left(y2 - 1) && k) {
        (*playerLives_p)--; //Decrement player lives
        drawPlayerLivesLabel(*playerLives_p, x2); //Output player lives to putty

        //Resets ball and striker
        deleteStriker(*striker_p);
        (*striker_p).x = (x1 + x2)/2 - (*striker_p).length/2;
        (*striker_p).y = y2 - 1;
        deleteBall((*ball_p));
        (*ball_p).vY = -(*ball_p).vY;
        (*ball_p).x = FIX14_left((*striker_p).x + (*striker_p).length/2);
        (*ball_p).y = FIX14_left((*striker_p).y - 2);
        drawBall((*ball_p));
        drawStriker((*striker_p));

        if (!(*playerLives_p)) { //Game over!!!
            gameOver(x1, x2, y1, y2);
            (*level_p) = 1;
            (*menuOpen_p) = 1;
            (*gameIsDone) = 1;
        }
        (*inGameStart_p) = 1;
    }
}

void ballStrikerCollision(ball_t * ball_p, striker_t striker) {
    if (FIX14_right((*ball_p).y) == striker.y - 1
        && FIX14_right((*ball_p).x + 0x2000) <= striker.x + striker.length
        && FIX14_right((*ball_p).x + 0x2000) >= striker.x) {
        for (int i = 0; i < striker.length; i++) {
            if (FIX14_right((*ball_p).x + 0x2000) == striker.x + i) {
                //Do stuff
                if ((*ball_p).vY > 0) {
                    (*ball_p).vY = -(*ball_p).vY;
                    (*ball_p).vX += -0x2000 + (0x2000 * 2)/(striker.length - 1) * i;
                    if ((*ball_p).vX >= 0x2000) {
                        (*ball_p).vX = 0x2000;
                    }
                    if ((*ball_p).vX <= -0x2000) {
                        (*ball_p).vX = -0x2000;
                    }
                }
            }
        }
    }
}

void ballBoxesCollision(ball_t * ball_p, box_t boxMatrix[MAX_COLUMNS][MAX_ROWS], uint16_t * score_p, uint8_t * boxesAlive_p, int32_t x2) {
    for (uint8_t i = 0; i < MAX_COLUMNS; i++) {
        for (uint8_t j = 0; j < MAX_ROWS; j++) {
            if (boxMatrix[i][j].lives > 0) { //Only executed if the box is "alive"
                (*boxesAlive_p)++;
                //Checking if ball hits the TOP side of the box
                if ((*ball_p).x >= FIX14_left(boxMatrix[i][j].x)
                    && FIX14_left((*ball_p).x < boxMatrix[i][j].x + boxMatrix[i][j].xSize)
                    && (*ball_p).y >= FIX14_left(boxMatrix[i][j].y)
                    && (*ball_p).y < FIX14_left(boxMatrix[i][j].y) + 0x2000
                    && (*ball_p).vY > 0) {
                        (*ball_p).vY = -(*ball_p).vY;
                        boxMatrix[i][j].lives--;
                        drawBox(boxMatrix[i][j]);
                        (*score_p)++;
                        drawScoreLabel(*score_p, x2);
                        /*
                        if (score >= *(uint16_t *)(address + 0 * 2) ) {
                            drawNewHighscoreLabel;
                        }
                        */
                }
                //Checking if ball hits the BOTTOM sides of the box
                else if ((*ball_p).x >= FIX14_left(boxMatrix[i][j].x)
                    && FIX14_left((*ball_p).x < boxMatrix[i][j].x + boxMatrix[i][j].xSize)
                    && (*ball_p).y <= FIX14_left(boxMatrix[i][j].y + boxMatrix[i][j].ySize)
                    && (*ball_p).y > FIX14_left(boxMatrix[i][j].y + boxMatrix[i][j].ySize) - 0x2000
                    && (*ball_p).vY < 0) {
                        (*ball_p).vY = -(*ball_p).vY;
                        boxMatrix[i][j].lives--;
                        drawBox(boxMatrix[i][j]);
                        (*score_p)++;
                        drawScoreLabel(*score_p, x2);
                        /*
                        if (score >= *(uint16_t *)(address + 0 * 2) ) {
                            drawNewHighscoreLabel;
                        }
                        */
                }
                //Checking if ball hits the LEFT side of the box
                else if ((*ball_p).y >= FIX14_left(boxMatrix[i][j].y)
                    && FIX14_left((*ball_p).y < boxMatrix[i][j].y + boxMatrix[i][j].ySize)
                    && (*ball_p).x >= FIX14_left(boxMatrix[i][j].x)
                    && (*ball_p).x < FIX14_left(boxMatrix[i][j].x) + 0x2000
                    && (*ball_p).vX > 0) {
                        (*ball_p).vX = -(*ball_p).vX;
                        boxMatrix[i][j].lives--;
                        drawBox(boxMatrix[i][j]);
                        (*score_p)++;
                        drawScoreLabel(*score_p, x2);
                        /*
                        if (score >= *(uint16_t *)(address + 0 * 2) ) {
                            drawNewHighscoreLabel;
                        }
                        */
                }
                //Checking if ball hits the RIGHT side of the box
                else if ((*ball_p).y >= FIX14_left(boxMatrix[i][j].y)
                    && FIX14_left((*ball_p).y < boxMatrix[i][j].y + boxMatrix[i][j].ySize)
                    && (*ball_p).x <= FIX14_left(boxMatrix[i][j].x + boxMatrix[i][j].xSize)
                    && (*ball_p).x > FIX14_left(boxMatrix[i][j].x + boxMatrix[i][j].xSize) - 0x2000
                    && (*ball_p).vX < 0) {
                        (*ball_p).vX = -(*ball_p).vX;
                        boxMatrix[i][j].lives--;
                        drawBox(boxMatrix[i][j]);
                        (*score_p)++;
                        drawScoreLabel(*score_p, x2);
                        /*
                        if (score >= *(uint16_t *)(address + 0 * 2) ) {
                            drawNewHighscoreLabel;
                        }
                        */
                }
            }
        }
    }
}

int main(void) {
    //Variables
    int32_t x1 = 1, y1 = 1, x2 = 120, y2 = 45; //Window size (current values will produce a 4 : 3 aspect ratio)
    x2 = (((x2 - x1 - 1) / 10) * 10) + x1 + 1; //Makes the width divisible by 10
    uint8_t k = 1; //Controlling speed of ball
    uint16_t strikerCounter = 0;
    uint16_t strikerMaxCount = 5000; //Affects striker speed
    uint8_t bossKey = 0;
    uint16_t score = 0;
    uint8_t playerLives = 3;
    uint8_t level = 1;
    uint8_t gameIsDone = 0;
    uint8_t boxesAlive;
    uint8_t menuOpen = 1; //0 = NO, 1 = YES; 2 = scoreboard open, 3 = help open
    uint8_t scoreboardX = (x1 + x2)/2 - (x1 + x2)/4, scoreboardY = 30;
    uint8_t startX = (x1 + x2)/2 - 6, startY = 30;
    uint8_t helpX = (x1 + x2)/2 + (x1 + x2)/4 - 12, helpY = 30;
    uint8_t scoreboardSelected = 0, startSelected = 0, helpSelected = 0;
    uint8_t inGameStart = 0;
    uint8_t centerPressed = 0;
    //Init FLASH-memory
    uint32_t address = 0x0800F800; // Starting-address of the last page
    uint16_t scoreData[10] = {0};
    uint16_t tempVal;
    uint16_t nextTemp;
    uint8_t writtenToScoreboard = 0;
    uint16_t lastVal = 0;

    //Initialization
    init_usb_uart(115200);
    initJoyStick();
    initTimer();

    //Initializing scoreboard
    for (int i = 0; i < 10; i++) {
        scoreData[i] = *(uint16_t *)(address + i * 2); //Reading from flash-memory
    }

    //Drawing window
    window(x1, y1, x2, y2, "Breakout", 1, 1);

    //Drawing menu labels
    drawScoreboardLabel(scoreboardX, scoreboardY, 0); //0 = black bgcolor
    drawStartLabel(startX, startY, 0);
    drawHelpLabel(helpX, helpY, 0);
    drawPlayerLivesLabel(playerLives, x2);

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
        if (flag && !menuOpen) { //Everything in this if-statement is executed once every 1/64 second (64 Hz refresh-rate)
            TIM2->CR1 = 0x0000; //Disabling timer

            //Updating ball-position
            deleteBall(ball);
            updateBallPos(&ball, k);
            drawBall(ball);

            //Making ball bounce on walls
            ballWallsCollision(&ball, &striker, &playerLives, &inGameStart, &menuOpen, k, &level, &gameIsDone, x1, y1, x2, y2);

            //Making ball bounce on striker
            ballStrikerCollision(&ball, striker);

            //Making ball bounce on boxes
            boxesAlive = 0;
            ballBoxesCollision(&ball, boxMatrix, &score, &boxesAlive, x2);

            if (!boxesAlive){ //When all boxes are "dead" (level-up!)
                level++;
                makeLevel(boxMatrix, &ball, &striker, x1, y1, x2, y2, level);
                drawLevelLabel(level, x2);
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
                    printBossKey(score, level, playerLives);
                    bossKey = 1;
                }
                break;
            case 4 : //Left
                strikerCounter++;
                if (striker.x > x1 + 1 && strikerCounter == strikerMaxCount && !bossKey) {
                    strikerCounter = 0;
                    updateStrikerPos(&striker, 4); //Moving striker left
                }
                break;
            case 8 : //Right
                strikerCounter++;
                if (striker.x < x2 - striker.length && strikerCounter == strikerMaxCount && !bossKey) {
                    strikerCounter = 0;
                    updateStrikerPos(&striker, 8); //Moving striker right
                }
                break;
            case 16 : //Center
                if (!centerPressed) {
                    if (!bossKey && (menuOpen == 1 || inGameStart)) {
                        if (scoreboardSelected) { //Show scoreboard
                            deleteMenuLabels(scoreboardX, scoreboardY, startX, startY, helpX, helpY);
                            menuOpen = 2;
                        } else if ((startSelected || inGameStart)) { //Start game
                            deleteMenuLabels(scoreboardX, scoreboardY, helpX, helpY, startX, startY);
                            drawScoreLabel(score, x2);
                            drawLevelLabel(level, x2);
                            drawPlayerLivesLabel(playerLives, x2);
                            inGameStart = 0;
                            menuOpen = 0;
                            TIM2->CR1 = 0x0001;
                        } else if (helpSelected) { //Show help
                            deleteMenuLabels(scoreboardX, scoreboardY, startX, startY, helpX, helpY);
                            menuOpen = 3;
                        }
                    } else if (bossKey && !menuOpen) { //Resume game
                        window(x1, y1, x2, y2, "Breakout", 1, 1);
                        for (uint8_t i = 0; i < MAX_COLUMNS; i++) {
                            for (uint8_t j = 0; j < MAX_ROWS; j++) {
                                drawBox(boxMatrix[i][j]);
                            }
                        }
                        drawScoreLabel(score, x2);
                        drawLevelLabel(level, x2);
                        drawPlayerLivesLabel(playerLives, x2);
                        drawStriker(striker);
                        drawBall(ball);
                        TIM2->CR1 = 0x0001;
                        bossKey = 0;
                    } else if (bossKey && menuOpen) { //When the menu-page should be opened
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
                        menuOpen = 1;
                    } else if (menuOpen == 2 || menuOpen == 3) { //Return to home-page from scoreboard or help-page
                        deleteHelp((x2 - x1)/4, 28);
                        deleteBackMessage((x2 - x1)/2, 25);
                        drawScoreboardLabel(scoreboardX, scoreboardY, 0);
                        drawStartLabel(startX, startY, 0);
                        drawHelpLabel(helpX, helpY, 0);
                        scoreboardSelected = 0;
                        startSelected = 0;
                        helpSelected = 0;
                        menuOpen = 1;
                    }
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
            drawScoreboard((x2 - x1)/4, 28, address);
        } else if (menuOpen == 3 && centerPressed) { //Help-button has been selected
            drawBackMessage((x2 - x1)/2, 25);
            drawHelp((x2 - x1)/4, 28);
        }

        //Writes score to scoreboard when the game has finished and if the score is great enough
        if (gameIsDone) {
            FLASH_Unlock(); // Unlock FLASH for writing
            FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
            FLASH_ErasePage( address ); // Erase entire page before writing
            for (int i = 0 ; i < 10 ; i++ ){
                if (score > scoreData[i] && !writtenToScoreboard) {
                    lastVal = score;
                    for (int j = i; j < 10; j++ ){
                        lastVal = swapWithReturnVal(scoreData[j], lastVal, j);
                    }
                    writtenToScoreboard = 1;
                }
                FLASH_ProgramHalfWord(address + i * 2, scoreData[i]);
            }
            FLASH_Lock();
            gameIsDone = 0;
        }
        //Checks if the the current score is grater than the high score
        if (score > scoreData[0]) {
            drawHighscoreLabel(x2); //Notifies the player if it is a new high score
        }
    }
}

