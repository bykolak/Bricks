#ifndef CLASSES_H
#define CLASSES_H

#include "define.h"
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

class cGame
{
private:
	cTile bricks[BRICKS_X][BRICKS_Y];
	int score;
	int game_state;
	int number_of_selected;
	bool selection;
	

public:
	
	cGame(); //default constructor
	cGame(cTile _bricks[BRICKS_X][BRICKS_Y]); //constructor for predefined maps
	int getGameState(); //returns gamestate;
	int getScore(); // returns score;
	int getNumberOfSelected();
	void changeScore(int _score); //passes int that changes actual score
	void changeGameState(int _game_state); // changes game_state;
	void updateNumberOfSelected();
	void drawGameArea(ALLEGRO_BITMAP *_bricksBMP); // draw all bricks on screen;
	void newGame(); // restart game
	void endGame(); // cleans and exit program
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