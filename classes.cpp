#include <stdlib.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "classes.h"
//======cTile methods

cTile::cTile()
{
	color = rand() % 8;
}
cTile::cTile(int _color, int _state)
{
	color = _color;
	state = _state;
	// public constructor
}
void cTile::changeColor(int _color)
{
	color = _color;
}
void cTile::changeState(int _state)
{
	state = _state;
	
}

int cTile::getColor()
{
	return color;
}
int cTile::getState()
{
	return state;
}
//=====cGame methods


cGame::cGame() //default constructor
{
	game_state = REFRESH_GAME;
	score = 0;
} 
cGame::cGame(cTile _bricks[BRICKS_X][BRICKS_Y]) //public constructor
{

}

int cGame::getGameState() //returns gamestate;
{
	return game_state;
}

int cGame::getScore() // returns score;
{
	return score;
}

void cGame::changeScore(int _score) //passes int that changes actual score
{
	score += _score;
}
void cGame::changeGameState(int _game_state) // changes game_state;
{
	game_state = _game_state;
}

void cGame::drawGameArea(ALLEGRO_BITMAP *_bricksBMP) // draw all bricks on screen;
{
	int i = 0;
	int t = 0;
	for (i = 0; i<BRICKS_Y; i++)
		for (t = 0; t < BRICKS_X; t++)
		{
			al_draw_bitmap_region(_bricksBMP, 1 * (bricks[t][i].getColor()*BRICK_WIDTH), 0, BRICK_WIDTH, BRICK_HEIGHT, t*BRICK_WIDTH + GAME_AREA_X, i*BRICK_WIDTH + GAME_AREA_Y, NULL);
		}
}

void cGame::newGame() // restart game
{

}

void cGame::endGame() // cleans and exit program
{

}

void cGame::highScores() // open high scores;
{

}

void cGame::options()// open options screen;
{

}
void cGame::selectTile(int _x, int y) // takes mouse input and selects all same color bricks that are connected to  tile[x][y]
{

}

void cGame::deselectTile() // clears selection of bricks 
{

}
void cGame::destroyTile() // after clicking selected bricks destroys them
{

}
void cGame::dropTile() //after destroying bricks fill holes by dropping them (checks from bottom)
{

}
void cGame::saveScores()//checks highscores & if your score is > than lowest highscore then prompts for username and saves it to file
{

}
