#ifndef build_game_h
#define build_game_h

#define MAX_COLUMNS 10
#define MAX_ROWS 10

void ballWallsCollision(ball_t * ball_p, striker_t * striker_p,
                        uint8_t * playerLives_p, uint8_t * inGameStart_p, uint8_t * menuOpen_p, uint8_t * k_p, uint8_t * level_p, uint8_t * gameIsDone_p,
                        int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void ballStrikerCollision(ball_t * ball_p, striker_t striker);
void ballBoxesCollision(ball_t * ball_p, box_t boxMatrix[MAX_COLUMNS][MAX_ROWS], uint16_t * score_p, uint8_t * boxesAlive_p, int32_t x2);

void makeLevel(box_t boxMatrix[MAX_COLUMNS][MAX_ROWS], ball_t * ball_p, striker_t * striker_p, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t level);
void initStriker(striker_t * striker_p, int32_t x1, int32_t x2, int32_t y2);
void initBall(ball_t * ball_p, striker_t striker);
void swapScores(uint16_t * lastVal_p, uint16_t * currentVal_p);

#endif //build_game_h
