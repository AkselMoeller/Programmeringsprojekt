#ifndef build_game_h
#define build_game_h

#define MAX_COLUMNS 10
#define MAX_ROWS 10

void makeLevel(box_t boxMatrix[MAX_COLUMNS][MAX_ROWS], ball_t * ball_p, striker_t * striker_p, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t level);
void initStriker(striker_t * striker_p, int32_t x1, int32_t x2, int32_t y2);
void initBall(ball_t * ball_p, striker_t striker);
int swapWithReturnVal(uint16_t data[10],uint16_t lastVal, uint8_t i);

#endif //build_game_h
