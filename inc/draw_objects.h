//
//  draw_objects.h
//  Programmeringsprojekt
//
//  Created by Alexander Ohrt on 08/06/2018.
//  Copyright © 2018 Alexander Ohrt. All rights reserved.
//

#ifndef draw_objects_h
#define draw_objects_h

#define FIX14_right(x) (x >> 14)
#define FIX14_left(x) (x << 14)

void window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char * title, uint8_t style, uint8_t reset);

typedef struct {
    int32_t x, y; //(x,y) is the position of the ball
    int32_t vX, vY; //(vX,vY) is the velocity-vector of the ball (how much it's going to move up/down next iteration)
} ball_t;
void drawBall(ball_t ball);
void updateBallPos(ball_t * ball_p, uint8_t k);
void deleteBall(ball_t ball);

typedef struct {
    int8_t x, y, length, color; //(x, y) is start position and length is the length of striker.
} striker_t;
void drawStriker (striker_t striker);
void updateStrikerPos(striker_t * striker_p, uint8_t joyStickState);
void deleteStriker(striker_t striker);

typedef struct {
    int32_t x, y;
    uint8_t xSize, ySize, powerUp;
    int8_t lives;
} box_t;
void drawBox(box_t box);

typedef struct {
    int32_t x, y;
    uint8_t type;
} powerUp_t;
void drawPowerUp(powerUp_t powerUp);

void drawScoreLabel(uint16_t score);
void drawLevelLabel(uint8_t level);
void drawPlayerLivesLabel(uint8_t playerLives);

void printBossKey(uint16_t score, uint8_t playerLives);
void gameOver(int32_t x1, int32_t x2, int32_t y1, int32_t y2);

void drawScoreboardLabel(uint8_t scoreboardX, uint8_t scoreboardY, uint8_t color, uint8_t backButton);
void drawStartLabel(uint8_t startX, uint8_t startY, uint8_t color);
void drawHelpLabel(uint8_t helpX, uint8_t helpY, uint8_t color, uint8_t backButton);
void deleteMenuLabels(uint8_t scoreboardX, uint8_t scoreboardY, uint8_t startX, uint8_t startY, uint8_t helpX, uint8_t helpY);

#endif /* draw_objects_h */
