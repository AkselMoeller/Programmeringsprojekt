#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "hardware_control.h"
#include "draw_objects.h"
#include "vectors.h"
#include "game_control.h"

int main(void) {
    //Variables
    uint8_t x1 = 1, y1 = 1, x2 = 100, y2 = 30; //Window size
    x2 = (((x2 - x1 - 1) / 10) * 10) + x1 + 1; //Makes the width divisible by 10 (number of boxes in game

    //initialization
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
    drawBall(&ball);

    //Drawing boxes
    box_t boxMatrix[10][5];
    for (uint8_t i = 0; i < sizeof(boxMatrix) / sizeof(boxMatrix[0]); i++) {
        for (uint8_t j = 0; j < sizeof(boxMatrix[0]) / sizeof(boxMatrix[0][0]); j++) {
            boxMatrix[i][j].xSize = (x2 - x1)/10;
            boxMatrix[i][j].ySize = (y2 - y1)/20;
            boxMatrix[i][j].x = (x1 + 1) +  boxMatrix[i][j].xSize * i;
            boxMatrix[i][j].y = (y1 + 1) + boxMatrix[i][j].ySize * j;
            boxMatrix[i][j].powerUp = 0;
            drawBox(&boxMatrix[i][j], 15);
        }
    }

    while(1) {
        if (flag) {
            //Everything here is executed once every 1/20 second

            flag = 0;
        }
    }
}
