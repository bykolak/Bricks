#ifndef DEFINE_H
#define DEFINE_H

#include <vector>
//using namespace std;

//#define RED al_map_rgb(255,0,0)
//#define BLUE al_map_rgb(0,0,255)
#define WHITE al_map_rgb(255,255,255)
#define YELLOW al_map_rgb(255,255,0)
#define RED al_map_rgb(255,0,0)
#define GREEN al_map_rgb(0,255,0)
#define BLACK al_map_rgb(0,0,0)
#define TINT al_map_rgba_f(1, 1, 1, 0.8)
//#define TINT2 al_map_rgba_f(0.7, 0.7, 0.7, 0.7)
#define TINT3 al_map_rgba_f(0.3, 0.3, 0.3, 0.3)
#define MASK_COLOR al_map_rgb(255,0,255)

//DEBUGS AND OVERLAYS FLAGS
const bool BUTTON_OVERLAY = true;

const int EXPLOSION_DELAY = 1;
extern int BRICK_COLORS;
const int SELECTION_SPEED = 100; //how many bricks selected per second
const int SCORE_DELAY = 4;
//high scores 
const int MAX_HIGH_SCORE = 10;//maximum high score entries
const int MAX_USERNAME_LENGTH = 21;//defines maximum username length 
//minimum size of screen
const float SCREEN_X = 1920.0;//minimum X size of window
const float SCREEN_Y = 1080.0; //minimum Y size of window
const float AWAY_FROM_CENTER = 20.0; 
const float BUTTON_SIZE = 277.0;
const float OPACITY_INCREASE = 0.032;
// predefined sizes of game
const int BRICKS_MAP_X = 48;// number of bricks in X axis
const int BRICKS_MAP_Y = 23;// number of bricks in Y axis

//sizes of bricks
const int BRICK_SIZE = 40;//brick size in pixels
const int AREA_WIDTH = BRICK_SIZE * BRICKS_MAP_X;
const int AREA_HEIGHT = BRICK_SIZE * BRICKS_MAP_Y;

 //margins
const int TOP_MARGIN = 64;

const int DOWN_MARGIN = 100;

const int MUSIC_MUTE_X = 295;
const int MUSIC_MUTE_Y = 312;
const int SOUND_MUTE_X = 295;
const int SOUND_MUTE_Y = 349;
enum BUTTONS { PLAY_BUTTON, OPTIONS_BUTTON, HIGHSCORES_BUTTON, EXIT_BUTTON, NEW_STORY_BUTTON, LOAD_GAME_BUTTON, NEW_RANDOM_BUTTON, MUSIC_MUTE_BUTTON, SOUND_MUTE_BUTTON, RESET_PROFILE_BUTTON, GAME_AREA_BUTTON,OPTIONS_POPUP,HIGHSCORES_POPUP, MAX_BUTTONS };
enum BUTTON_TYPE{RHOMB,TRIANGLE, RECTANGLE,GAME_AREA,MAX_TYPE};
enum TILE_STATE{ EMPTY, FULL, SELECTED, EXPLODING };
enum GAME_STATE {PLAY_GAME, REFRESH_GAME, HIGH_SCORE, OPTIONS, END_GAME, SAVING_SCORE, CHEAT,MAIN_MENU};
enum KEYS { UP, DOWN, LEFT, RIGHT };
enum EXIT_CODES{DELAYED,ANIMATING};
#endif