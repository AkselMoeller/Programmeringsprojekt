#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "hardware_control.h"
#include "draw_objects.h"
#include "build_game.h"



void center (uint8_t * centerPressed_p, uint8_t * bossKey_p, uint8_t * menuOpen_p, uint8_t * inGameStart_p, uint8_t * scoreboardSelected_p, uint8_t scoreboardX,
             uint8_t scoreboardY, uint8_t startX, uint8_t startY, uint8_t helpX, uint8_t helpY, uint8_t * startSelected_p, uint8_t * helpSelected_p,
             uint8_t score, uint8_t level, int32_t x1, int32_t x2, int32_t y1, int32_t y2, uint8_t playerLives, uint8_t colums, uint8_t rows,
             box_t boxMatrix[colums][rows], ball_t * ball_p, striker_t * striker_p) {
     if (!(*centerPressed_p)) {
        if (!(*bossKey_p) && ((*menuOpen_p) == 1 || (*inGameStart_p))) {
            if ((*scoreboardSelected_p)) { //Show scoreboard
                deleteMenuLabels(scoreboardX, scoreboardY, startX, startY, helpX, helpY);
                (*menuOpen_p) = 2;
            } else if ((*startSelected_p) || (*inGameStart_p)) { //Start game
                deleteMenuLabels(scoreboardX, scoreboardY, helpX, helpY, startX, startY);
                drawScoreLabel(score, x2);
                drawLevelLabel(level, x2);
                drawPlayerLivesLabel(playerLives, x2);
                (*inGameStart_p) = 0;
                (*menuOpen_p) = 0;
                TIM2->CR1 = 0x0001;
            } else if (*helpSelected_p) { //Show help
                deleteMenuLabels(scoreboardX, scoreboardY, startX, startY, helpX, helpY);
                (*menuOpen_p) = 3;
            }
        } else if ((*bossKey_p) && !(*menuOpen_p)) { //Resume game
            window(x1, y1, x2, y2, "Breakout", 1, 1);
            for (uint8_t i = 0; i < MAX_COLUMNS; i++) {
                for (uint8_t j = 0; j < MAX_ROWS; j++) {
                    drawBox(boxMatrix[i][j]);
                }
            }
            drawScoreLabel(score, x2);
            drawLevelLabel(level, x2);
            drawPlayerLivesLabel(playerLives, x2);
            drawStriker((*striker_p));
            drawBall(*ball_p);
            TIM2->CR1 = 0x0001;
            (*bossKey_p) = 0;
        } else if ((*bossKey_p) && (*menuOpen_p)) { //When the menu-page should be opened
            window(x1, y1, x2, y2, "Breakout", 1, 1);
            for (uint8_t i = 0; i < MAX_COLUMNS; i++) {
                for (uint8_t j = 0; j < MAX_ROWS; j++) {
                    drawBox(boxMatrix[i][j]);
                }
            }
            drawStriker(*striker_p);
            drawScoreboardLabel(scoreboardX, scoreboardY, 0);
            drawStartLabel(startX, startY, 0);
            drawHelpLabel(helpX, helpY, 0);
            (*scoreboardSelected_p) = 0;
            (*startSelected_p) = 0;
            (*helpSelected_p) = 0;
            (*bossKey_p) = 0;
            (*menuOpen_p) = 1;
        } else if ((*menuOpen_p) == 2 || (*menuOpen_p) == 3) { //Return to home-page from scoreboard or help-page
            deleteHelp((x2 - x1)/4, 28);
            deleteScoreboard((x2 - x1)/4, 28);
            deleteBackMessage((x2 - x1)/2, 25);
            drawScoreboardLabel(scoreboardX, scoreboardY, 0);
            drawStartLabel(startX, startY, 0);
            drawHelpLabel(helpX, helpY, 0);
            (*scoreboardSelected_p) = 0;
            (*startSelected_p) = 0;
            (*helpSelected_p) = 0;
            (*menuOpen_p) = 1;
        }
        (*centerPressed_p) = 1;
    }
}



void ballWallsCollision(ball_t * ball_p, striker_t * striker_p,
                        uint8_t * playerLives_p, uint8_t * inGameStart_p, uint8_t * menuOpen_p, uint8_t * k_p, uint8_t * level_p, uint8_t * gameIsDone_p,
                        int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    if ((*ball_p).x <= FIX14_left(x1 + 1) || (*ball_p).x >= FIX14_left(x2 - 1)) {
        (*ball_p).vX = -(*ball_p).vX;
    }
    if ((*ball_p).y <= FIX14_left(y1 + 1)) {
        (*ball_p).vY = -(*ball_p).vY;
    }
    if ((*ball_p).y >= FIX14_left(y2 - 1) && (*k_p)) {
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
            (*gameIsDone_p) = 1;
            (*k_p) = 0;
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
                }
            }
        }
    }
}

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
                        if (i == 4){
                            boxMatrix[i][j].powerUp = 1; // power up for extra ball
                            boxMatrix[i][j].lives = 2;
                        }
                    } else {
                        boxMatrix[i][j].lives = 0;
                    }
                    break;
                case 3 :
                    if (j < 4 && ((j%2 && (i+1)%2) || ((j+1)%2 && i%2))) {
                        boxMatrix[i][j].lives = 1;
                    } else if (j == 6) {
                        boxMatrix[i][j].lives = 2;
                    } else {
                        boxMatrix[i][j].lives = 0;
                    }
            }
            drawBox(boxMatrix[i][j]); //draw all boxes
        }
    }

    (*striker_p).x = (x1 + x2)/2 - (*striker_p).length/2;
    drawStriker(*striker_p);

    initBall(ball_p, *striker_p);

    //making the ball faster for every level
    switch(level){
        case 2 : (*ball_p).vY -= 0x1000; // (+0,25) - very fast for testing
        case 3 : (*ball_p).vY -= 0x2000; // (+0,5)
    }
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

//Writes a value to data set, while swapping it with the next
void swapScores(uint16_t * lastVal_p, uint16_t * currentVal_p) {
    uint16_t temp = *lastVal_p;
    *lastVal_p = *currentVal_p;
    *currentVal_p = temp;
}
