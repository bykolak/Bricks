#ifndef DEFINE_H
#define DEFINE_H
//tutaj wszystkie define

#define RED al_map_rgb(255,0,0)
#define BLUE al_map_rgb(0,0,255)
#define WHITE al_map_rgb(255,255,255)






//dimensions of single Brick
const int BRICK_HEIGHT = 48;
const int BRICK_WIDTH = 48;
const int BRICK_COLORS = 6;
// dimensions of game area
const int BRICKS_X = 35;  //number of Bricks in X axis
const int BRICKS_Y = 15;  //number of Bricks in Y axis;
const int BRICKS_ON_SCREEN = BRICKS_X*BRICKS_Y;
const int TOP_MARGIN = 42;
const int LEFT_MARGIN = 9;
const int RIGHT_MARGIN = 9;
const int DOWN_MARGIN = 9;
//const int GAME_AREA_X = 9;
//const int GAME_AREA_Y = 42;
const int GAME_AREA_WIDTH = BRICK_WIDTH * BRICKS_X;
const int GAME_AREA_HEIGHT = BRICK_HEIGHT * BRICKS_Y;
//screen dimensions;
const int WIDTH = LEFT_MARGIN + GAME_AREA_WIDTH + RIGHT_MARGIN;
const int HEIGHT = TOP_MARGIN + GAME_AREA_HEIGHT + DOWN_MARGIN;
// size of buttons
const int NEW_GAME_X = 5;
const int NEW_GAME_Y = 3;
const int NEW_GAME_WIDTH = 250;
const int NEW_GAME_HEIGHT = 34;

const int HIGH_SCORES_X = 260;
const int HIGH_SCORES_Y = 3;
const int HIGH_SCORES_WIDTH = 250;
const int HIGH_SCORES_HEIGHT = 34;

const int OPTIONS_X = 515;
const int OPTIONS_Y = 3;
const int OPTIONS_WIDTH = 250;
const int OPTIONS_HEIGHT = 34;
const int SCORE_X = 850;
const int SCORE_Y = 5;
//ENUMS
enum TILE_STATE{ EMPTY, FULL, SELECTED };
enum GAME_STATE {PLAY_GAME, REFRESH_GAME, HIGH_SCORE, OPTIONS, END_GAME};
enum KEYS { UP, DOWN, LEFT, RIGHT };

#endif