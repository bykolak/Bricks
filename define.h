#ifndef DEFINE_H
#define DEFINE_H

#include <vector>
using namespace std;

#define RED al_map_rgb(255,0,0)
#define BLUE al_map_rgb(0,0,255)
#define WHITE al_map_rgb(255,255,255)
#define YELLOW al_map_rgb(255,255,0)
#define BLACK al_map_rgb(0,0,0)
#define TINT al_map_rgba_f(1, 1, 1, 0.8)
#define TINT2 al_map_rgba_f(0.7, 0.7, 0.7, 0.7)
#define TINT3 al_map_rgba_f(0.3, 0.3, 0.3, 0.3)
#define MASK_COLOR al_map_rgb(255,0,255)

extern int BRICK_COLORS;

const int SCORE_DELAY = 4;
//high scores 
const int MAX_HIGH_SCORE = 10;
const int MAX_USERNAME_LENGTH = 21;
//minimum size of screen
const int MIN_SCREEN_X = 1030;
const int MIN_SCREEN_Y = 400;
// predefined sizes of game
const int BRICKS_SMALL_X = 22;
const int BRICKS_SMALL_Y = 13;

const int BRICKS_LARGE_X = 35;
const int BRICKS_LARGE_Y = 18;
//sizes of bricks
const int BRICKS_LARGE = 40;
const int BRICKS_MEDIUM = 36;
const int BRICKS_SMALL = 24;
//margins
const int TOP_MARGIN = 42;
const int LEFT_MARGIN = 9;
const int RIGHT_MARGIN = 9;
const int DOWN_MARGIN = 9;

// size of buttons //NEEDS EVALUATION (needs to be scalable)
const int BUTTON_WIDTH = 250;
const int BUTTON_HEIGHT = 31;
const int MAX_CHARS = 25;//length of text on button
const int SMALL_BUTTON_WIDTH = 70;
const int SMALL_BUTTON_HEIGHT = 31;

//position of options buttons on screen 
const int OPTIONS_SMALL_Y = TOP_MARGIN + 32;
const int OPTIONS_LARGE_Y = TOP_MARGIN + 69;
const int OPTIONS_CAMPAIGN_Y = TOP_MARGIN + 106;
const int OPTIONS_SIZE_Y = TOP_MARGIN + 170;


const int END_GAME_NEW_GAME_X = 3;
const int END_GAME_NEW_GAME_Y = 333;
const int END_GAME_SAVE_SCORE_X = 390;
const int END_GAME_SAVE_SCORE_Y = 333;
//ENUMS
enum BUTTON_XY{
	NEW_GAME_BUTTON, HIGH_SCORES_BUTTON, OPTIONS_BUTTON,SCORE_BUTTON,GAME_AREA_BUTTON,//main screen buttons
	OPTIONS_SMALL_BUTTON, OPTIONS_LARGE_BUTTON, OPTIONS_CAMPAIGN_BUTTON, OPTIONS_24_BUTTON, OPTIONS_36_BUTTON, OPTIONS_48_BUTTON,//options buttons
	HIGH_SCORES_RESET_BUTTON, HIGH_SCORES_CLOSE_BUTTON, //high scores buttons
	END_GAME_NEW_GAME_BUTTON, END_GAME_SAVE_SCORE_BUTTON, //end game buttons
	NUMBER_OF_BUTTONS //array index of game.button[NUMBER_OF_BUTTONS]
};
enum TILE_STATE{ EMPTY, FULL, SELECTED, EXPLODING };
enum GAME_STATE {PLAY_GAME, REFRESH_GAME, HIGH_SCORE, OPTIONS, END_GAME, SAVING_SCORE, CHEAT};
enum KEYS { UP, DOWN, LEFT, RIGHT };
enum BUTTON_TYPE{ LARGE_BUTTON, SMALL_BUTTON, SHOW_ONLY_BUTTON,FAKE_BUTTON };
#endif