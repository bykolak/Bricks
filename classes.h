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

	
};

class cGame
{
private:
	cTile bricks[BRICKS_X][BRICKS_Y];
	int score;
	int game_state;

	

public:
	cGame(); //default constructor
	cGame(cTile _bricks[BRICKS_X][BRICKS_Y]); //public constructor
	int getGameState(); //returns gamestate;
	int getScore(); // returns score;
	void changeScore(int _score); //passes int that changes actual score
	void changeGameState(int _game_state); // changes game_state;
	void drawGameArea(); // draw all bricks on screen;
	void newGame(); // restart game
	void endGame(); // cleans and exit program
	void highScores(); // open high scores;
	void options(); // open options screen;
	void selectTile(int _x, int y); // takes mouse input and selects all same color bricks that are connected to  tile[x][y]
	void deselectTile(); // clears selection of bricks 
	void destroyTile(); // after clicking selected bricks destroys them
	void dropTile(); //after destroying bricks fill holes by dropping them (checks from bottom)
	void saveScores();//checks highscores & if your score is > than lowest highscore then prompts for username and saves it to file
};


#endif