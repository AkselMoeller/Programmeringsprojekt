#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "hardware_control.h"
#include "draw_objects.h"
#include "build_game.h"

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
