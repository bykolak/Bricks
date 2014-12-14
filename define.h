#ifndef DEFINE_H
#define DEFINE_H

#include <vector>
using namespace std;

#define RED al_map_rgb(255,0,0)
#define BLUE al_map_rgb(0,0,255)
#define WHITE al_map_rgb(255,255,255)
#define BLACK al_map_rgb(0,0,0)
#define TINT al_map_rgba_f(1, 1, 1, 0.8)
#define TINT2 al_map_rgba_f(0.5, 0.5, 0.5, 0.5)


//dimensions of single Brick
const int BRICK_HEIGHT = 48;
const int BRICK_WIDTH = 48;
extern int BRICK_COLORS;
// dimensions of game area
extern int BRICKS_X;  //number of Bricks in X axis
extern int BRICKS_Y;  //number of Bricks in Y axis;
const int BRICKS_SMALL_X = 20;
const int BRICKS_SMALL_Y = 20;
//const int BRICKS_MEDIUM_X = 30;
//const int BRICKS_MEDIUM_Y = 20;
const int BRICKS_LARGE_X = 35;
const int BRICKS_LARGE_Y = 20;
const int BRICKS_ON_SCREEN = BRICKS_X*BRICKS_Y;
const int TOP_MARGIN = 42;
const int LEFT_MARGIN = 9;
const int RIGHT_MARGIN = 9;
const int DOWN_MARGIN = 9;
const int GAME_AREA_WIDTH = BRICK_WIDTH * BRICKS_X;
const int GAME_AREA_HEIGHT = BRICK_HEIGHT * BRICKS_Y;
//screen dimensions;
const int WIDTH = LEFT_MARGIN + GAME_AREA_WIDTH + RIGHT_MARGIN;
const int HEIGHT = TOP_MARGIN + GAME_AREA_HEIGHT + DOWN_MARGIN;
// size of buttons
const int BUTTON_WIDTH = 250;
const int BUTTON_HEIGHT = 34;
const int SMALL_BUTTON_WIDTH = 70;
const int SMALL_BUTTON_HEIGHT = 34;
// positions of buttons on main screen
const int NEW_GAME_X = 5;
const int NEW_GAME_Y = 3;

const int HIGH_SCORES_X = 260;
const int HIGH_SCORES_Y = 3;

const int OPTIONS_X = 515;
const int OPTIONS_Y = 3;

const int SCORE_X = 850;
const int SCORE_Y = 5;

//position of high_score buttons on screen 6,404  6, 441
const int HIGH_SCORE_RESET_X = HIGH_SCORES_X + 6;
const int HIGH_SCORE_RESET_Y = TOP_MARGIN + 404;
const int HIGH_SCORE_CLOSE_X = HIGH_SCORES_X + 6;
const int HIGH_SCORE_CLOSE_Y = TOP_MARGIN + 441;

//position of options buttons on screen
const int OPTIONS_SMALL_X = OPTIONS_X + 5; // +5 is position on bitmap
const int OPTIONS_SMALL_Y = TOP_MARGIN + 32;
const int OPTIONS_LARGE_X = OPTIONS_X + 5;
const int OPTIONS_LARGE_Y = TOP_MARGIN + 69;
const int OPTIONS_CAMPAIGN_X = OPTIONS_X + 5;
const int OPTIONS_CAMPAIGN_Y = TOP_MARGIN + 106;

const int OPTIONS_24_X = OPTIONS_X + 14;
const int OPTIONS_24_Y = TOP_MARGIN + 170;
const int OPTIONS_36_X = OPTIONS_X + 96;
const int OPTIONS_36_Y = TOP_MARGIN + 170;
const int OPTIONS_48_X = OPTIONS_X + 176;
const int OPTIONS_48_Y = TOP_MARGIN + 170;



//ENUMS
enum BUTTON_FLAGS {
	OVER_NEW_GAME, OVER_HIGH_SCORES, OVER_OPTIONS, OVER_GAME_AREA,  //main screen flags
	OVER_OPTIONS_SMALL, OVER_OPTIONS_LARGE, OVER_OPTIONS_CAMPAIGN, OVER_OPTIONS_24, OVER_OPTIONS_36, OVER_OPTIONS_48, //inside of options flags
	OVER_HIGH_SCORE_RESET, OVER_HIGH_SCORE_CLOSE, //inside of high scores flags
	NUMBER_OF_FLAGS};//number for array index of game.flags[]
enum TILE_STATE{ EMPTY, FULL, SELECTED };
enum GAME_STATE {PLAY_GAME, REFRESH_GAME, HIGH_SCORE, OPTIONS, END_GAME};
enum KEYS { UP, DOWN, LEFT, RIGHT };

#endif