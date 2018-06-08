//
//  draw_objects.h
//  Programmeringsprojekt
//
//  Created by Alexander Ohrt on 08/06/2018.
//  Copyright © 2018 Alexander Ohrt. All rights reserved.
//

#ifndef draw_objects_h
#define draw_objects_h

void window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char * title, uint8_t style);

typedef struct {
    uint8_t x, y; //(x,y) is the position of the ball
    int8_t vX, vY; //(vX,vY) is the velocity-vector of the ball (how much it's going to move up/down next iteration)
} ball_t;
void drawBall(ball_t * ball_p);
void updateBallPos(ball_t * ball_p, uint8_t k);
void deleteBall(ball_t * ball_p);

typedef struct {
    int8_t x, y, length; //(x, y) is start position and length is the length of striker.
} striker_t;
void drawStriker (striker_t * striker_p);
void updateStrikerPos(striker_t * striker_p, uint8_t joyStickState);
void deleteStriker(striker_t * striker_p);

typedef struct {
    uint8_t x, y, xSize, ySize, powerUp;
} box_t;
void drawBox(box_t * box_p, uint8_t boxColor);

#endif /* draw_objects_h */
