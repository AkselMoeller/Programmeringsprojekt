//
//  draw_objects.h
//  Programmeringsprojekt
//
//  Created by Alexander Ohrt on 08/06/2018.
//  Copyright © 2018 Alexander Ohrt. All rights reserved.
//

#ifndef draw_objects_h
#define draw_objects_h

#define FIX14_right(x) ((x) >> 14)
#define FIX14_left(x) ((x) << 14)


void window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char * title, uint8_t style, uint8_t reset);

typedef struct {
    int32_t x, y; //(x,y) is the position of the ball
    int32_t vX, vY; //(vX,vY) is the velocity-vector of the ball (how much it's going to move up/down next iteration)
    int8_t active;
} ball_t;

void drawBall(ball_t ball);
void deleteBall(ball_t ball);
void updateBallPos(ball_t * ball_p);

typedef struct {
    uint8_t x, y, length, color; //(x, y) is start position and length is the length of striker.
} striker_t;
void drawStriker (striker_t striker);
void deleteStriker(striker_t striker);
void updateStrikerPos(striker_t * striker_p, uint8_t joyStickState);

typedef struct {
    int32_t x, y;
    uint8_t style, hit;
} powerUp_t;

typedef struct {
    int32_t x, y;
    uint8_t xSize, ySize;
    int8_t lives;
    powerUp_t powerUp;
} box_t;
void drawBox(box_t box);
void drawPowerUp(box_t * box_p, int32_t y2);

void drawScoreLabel(uint16_t score, uint8_t x2);
void drawLevelLabel(uint8_t level, uint8_t x2);
void drawPlayerLivesLabel(uint8_t playerLives, uint8_t x2);
void drawHighscoreLabel(int32_t x2);

void printBossKey(uint16_t score, uint8_t level, uint8_t playerLives);
void gameOver(int32_t x1, int32_t x2, int32_t y1, int32_t y2);

void drawBackMessage(uint8_t x, uint8_t y);
void deleteBackMessage(uint8_t x, uint8_t y);

void drawScoreboardLabel(uint8_t scoreboardX, uint8_t scoreboardY, uint8_t color);
void drawStartLabel(uint8_t startX, uint8_t startY, uint8_t color);
void drawHelpLabel(uint8_t helpX, uint8_t helpY, uint8_t color);
void deleteMenuLabels(uint8_t scoreboardX, uint8_t scoreboardY, uint8_t startX, uint8_t startY, uint8_t helpX, uint8_t helpY);

void drawHelp(uint8_t x, uint8_t y);
void deleteHelp(uint8_t x, uint8_t y);
void drawScoreboard(uint8_t x, uint8_t y, uint32_t address);
void deleteScoreboard(uint8_t x, uint8_t y);

#endif /* draw_objects_h */
