#ifndef CLASSES_H
#define CLASSES_H

#include "define.h"
#include <fstream>
#include <string>
extern ALLEGRO_FONT *arial24;
extern ALLEGRO_DISPLAY *display;

class cTile
{
private:
	int color;
	int state;
	
public:
	cTile(); //default constructor
	cTile(int _color, int _state);
void	changeColor(int _color); 
void	changeState(int _state);
int getColor();
int getState();

	
};

class cButton
{
private:
	
	int width;
	int height;
	bool flags;
	ALLEGRO_BITMAP *buttonBMP;
	ALLEGRO_BITMAP *buttonPressedBMP;
public:
	int x;
	int y;
	int type;
	int state;
	const char* text;
	cButton();//constructor
	bool getFlags();//return state of flag
	void toggleFlags();//flips value of a flag;
	void changeFlags( bool _value);//change flag to specific value
	bool overButton(int _mouse_x, int _mouse_y); //if inside button then change flags to true else make it false
	void changeButtonSize(int _x, int _y, int _width, int _height); //sets all button parameters
	void createButton(ALLEGRO_BITMAP *temp);//creates bitmap for button
	void drawButton();//draw button on screen
	int getWidth();
	int getHeight();
	};
class cGame
{
private:
	//int xx, yy;
	int score; // players score
	int on_screen_score;//shown score
	int score_count;
	
	int number_of_selected; //how many selected 
	
	bool selection; //if something selected
	bool saved_scores; //if high scores already saved
	bool update_position;
	
	int bricks_x;//bricks in x axis
	int bricks_y;//bricks in y axis
	int bricks_on_screen; //total number of bricks on screen
	int area_width;//number of bricks in x axis * brick_size
	int area_height;//number of bricks in y axis * brick_size
	int left_button_margin;//where to start draw top buttons
	int left_game_area_margin;// where to start draw game area
	int screen_shake;// move everything on screen (in pixels)
	int frameCount;
	const int frameDelay = 0;
	int curFrame;
	const int maxFrame=40;
public:
	int brick_size; //size of bricks
	int game_state; // using enum GAME_STATE to tell program in what state game is
	bool destroy_brick;
	int screen_width;//screen size x in pixels
	int screen_height;//screen size y in pixels	
	int high_score[MAX_HIGH_SCORE]; //array that holds all high scores
	ALLEGRO_USTR* high_score_name[MAX_HIGH_SCORE];
	cButton button[NUMBER_OF_BUTTONS];
	ALLEGRO_USTR* player_name = al_ustr_new("");
	ALLEGRO_USTR* edited_text = al_ustr_new("");
	ALLEGRO_BITMAP *scoreBMP = NULL;
	ALLEGRO_BITMAP *bricksBMP = NULL;
	ALLEGRO_BITMAP *optionsBMP = NULL;
	ALLEGRO_BITMAP *shadowBMP = NULL;
	ALLEGRO_BITMAP *buttonsBMP = NULL;
	ALLEGRO_BITMAP *backgroundBMP = NULL;
	ALLEGRO_BITMAP *explosionBMP = NULL;
	vector< vector<cTile> > bricks;
	cGame(); //default constructor
	void updateScore();//updates on_screen score
	void drawScore();//draws score to the screen
	void checkButtons(int mouseX, int mouseY);//checks if mouse over button
	void clickButtons(int mouseButton, int mouseX, int mouseY);
	void updatePositions();
	void loadButton(); //loads button locations from file
	bool checkGameState(int state);//returns true if state=game_state
	void resetHighScores();//resets and saves new high score file
	void changeBricksXY(int _x, int _y); //changes map size
	void changeBrickSize(int x);//changes brick size
	void changeTile(int x, int y, int color); //changes color of x,y tile
	void updateNumberOfSelected(); //checks and update number of selected brcisk
	void drawGameArea(); // draw all bricks on screen
	void newGame(bool debug); // restart game
	void checkEndGame(); //checks if game ended (no more bricks to destroy)
	void highScores(); // draw high scores;
	void options(); // draw options screen;
	void endGame(); //draw end game screen
	void selectBrick(int _mouse_x, int _mouse_y); // takes mouse input and selects all same color bricks that are connected to  Brick[x][y]
	int checkNeighbourBrick(int x, int y);  //selects neighbouring bricks
	void deselectBrick(); // clears selection of bricks 
	void destroyBrick(); // after clicking selected bricks destroys them
	void calculateScore(); //calculates score for destroyed bricks
	void dropBrick(); //after destroying bricks fill holes by dropping them (checks from bottom)
	void moveBrickLeft();//if empty vertical line then move rest of screen to left
	bool checkSaveScores();//checks highscores & if your score is > than lowest highscore then return true
	void saveScores();//
	void enterPlayerName(int keycode, int unichar); 
	void saveToFile();
	void loadFromFile();
	void saveGame();
	void loadGame();
};


#endif