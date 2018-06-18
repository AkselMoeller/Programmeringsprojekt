#ifndef build_game_h
#define build_game_h

#define MAX_COLUMNS 10
#define MAX_ROWS 10

void center(uint8_t * centerPressed_p, uint8_t * bossKey_p, uint8_t * menuOpen_p, uint8_t * inGameStart_p, uint8_t * scoreboardSelected_p,
            uint8_t scoreboardX, uint8_t scoreboardY, uint8_t startX, uint8_t startY, uint8_t helpX, uint8_t helpY,
            uint8_t * startSelected_p, uint8_t * helpSelected_p, uint8_t score, uint8_t level, int32_t x1, int32_t x2, int32_t y1, int32_t y2,
            uint8_t playerLives, box_t boxMatrix[MAX_COLUMNS][MAX_ROWS], ball_t * ball_p, striker_t * striker_p, uint8_t * gameIsDone_p, ball_t * ball2_p);

void ballWallsCollision(ball_t * ball_p, striker_t * striker_p,
                         uint8_t * k_p, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

void playerDead(ball_t * ball_p, striker_t * striker_p,
                        uint8_t * playerLives_p, uint8_t * inGameStart_p, uint8_t * menuOpen_p, uint8_t * k_p, uint8_t * gameIsDone_p,
                        int32_t x1, int32_t y1, int32_t x2, int32_t y2);

void strikerCollision(ball_t * ball_p, striker_t striker, box_t boxMatrix[MAX_COLUMNS][MAX_ROWS], uint16_t * score_p, int32_t x2);
void ballBoxesCollision(ball_t * ball_p, box_t boxMatrix[MAX_COLUMNS][MAX_ROWS], uint16_t * score_p, uint8_t * boxesAlive_p, int32_t x2, int32_t y2);

void makeLevel(box_t boxMatrix[MAX_COLUMNS][MAX_ROWS], ball_t * ball_p, striker_t * striker_p, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t level);
void initStriker(striker_t * striker_p, int32_t x1, int32_t x2, int32_t y2);
void initBall(ball_t * ball_p, striker_t striker);
void swapScores(uint16_t * lastVal_p, uint16_t * currentVal_p);

#endif //build_game_h
