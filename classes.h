#ifndef CLASSES_H
#define CLASSES_H

#include "define.h"

extern ALLEGRO_FONT *arial24;
extern ALLEGRO_DISPLAY *display;
//extern ALLEGRO_BITMAP *buttonsBMP;
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
	int x;
	int y;
	int width;
	int height;
	
	bool flags;
	
	ALLEGRO_BITMAP *buttonBMP;
	ALLEGRO_BITMAP *buttonPressedBMP;
public:
	int type;
	const char* text;
	cButton();//constructor
	bool getFlags();//return state of flag
	void toggleFlags();//flips value of a flag;
	void changeFlags( bool _value);//change flag to specific value
	bool overButton(int _mouse_x, int _mouse_y); //if inside button then change flags to true else make it false
	void changeButtonSize(int _x, int _y, int _width, int _height); //sets all button parameters
	void createButton(ALLEGRO_BITMAP *temp);//creates bitmap for button
	void drawButton();//draw button on screen
};
class cGame
{
private:
	
	int score; // players score
	int game_state; // using enum GAME_STATE to tell program in what state game is
	int number_of_selected; //how many selected 
	int brick_size; //size of bricks
	bool selection; //if something selectd
	//bool flags[NUMBER_OF_FLAGS]; //button flags
	
public:
	cButton button[NUMBER_OF_BUTTONS];
	int bricks_x;//bricks in x axis
	int bricks_y;//bricks in y axis
	int bricks_on_screen; //total number of bricks on screen
	int area_width;//number of bricks in x axis * brick_size
	int area_height;//number of bricks in y axis * brick_size
	int screen_width;//screen size x in pixels
	int screen_height;//screen size y in pixels
	vector< vector<cTile> > bricks;
	cGame(); //default constructor
	//cGame(cTile _bricks[BRICKS_X][BRICKS_Y]); //constructor for predefined maps
	
	void loadButton(); //loads button locations from file
	int getGameState(); //returns gamestate
	int getScore(); // returns score
	int getNumberOfSelected();//returns how many bricks are selected
	int getBrickSize();//returns size of brick in pixels
	void resetHighScores();
	void changeBricksXY(int _x, int _y); //changes map size
	void changeBrickSize(int x);//changes brick size
	void changeTile(int x, int y, int color); //changes color of x,y tile
	void changeScore(int _score); //passes int that changes actual score
	void changeGameState(int _game_state); // changes game_state
	void updateNumberOfSelected(); //checks and update number of selected brcisk
	void drawGameArea(ALLEGRO_BITMAP *_bricksBMP); // draw all bricks on screen
	void newGame(); // restart game
	int checkEndGame(); //checks if game ended (no more bricks to destroy)
	void highScores(); // open high scores;
	void options(); // open options screen;
	void selectBrick(int _mouse_x, int _mouse_y); // takes mouse input and selects all same color bricks that are connected to  Brick[x][y]
	int checkNeighbourBrick(int x, int y);  //selects neighbouring bricks
	void deselectBrick(); // clears selection of bricks 
	void destroyBrick(); // after clicking selected bricks destroys them
	void calculateScore(); //calculates score for destroyed bricks
	void dropBrick(); //after destroying bricks fill holes by dropping them (checks from bottom)
	void saveScores();//checks highscores & if your score is > than lowest highscore then prompts for username and saves it to file
};


#endif