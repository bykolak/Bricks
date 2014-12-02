#include <stdlib.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "classes.h"
//======cTile methods

cTile::cTile()
{
	color = rand() % BRICK_COLORS;
	state = FULL;
	
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
	selection = false;
	number_of_selected = 0;
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
int cGame::getNumberOfSelected()
{
	return number_of_selected;
}

void cGame::changeScore(int _score) //passes int that changes actual score
{
	score += _score;
}
void cGame::changeGameState(int _game_state) // changes game_state;
{
	game_state = _game_state;
}

void cGame::updateNumberOfSelected() // changes game_state;
{
	number_of_selected = 0;
	int i = 0;
	int t = 0;
	for (i = 0; i<BRICKS_Y; i++)
		for (t = 0; t < BRICKS_X; t++)
		{
			if (bricks[t][i].getState() == SELECTED)
			{
				number_of_selected++;
			}

		}
//	selection = false;
}

void cGame::drawGameArea(ALLEGRO_BITMAP *_bricksBMP) // draw all bricks on screen;
{
	int i = 0;
	int t = 0;
	for (i = 0; i<BRICKS_Y; i++)
		for (t = 0; t < BRICKS_X; t++)
		{
			if (bricks[t][i].getState() == FULL)
			{
				al_draw_bitmap_region(_bricksBMP, 1 * ((bricks[t][i].getColor()+1)*BRICK_WIDTH), 0, BRICK_WIDTH, BRICK_HEIGHT, t*BRICK_WIDTH + GAME_AREA_X, i*BRICK_WIDTH + GAME_AREA_Y, NULL);
			}
			else if (bricks[t][i].getState() == SELECTED)
			{
				al_draw_bitmap_region(_bricksBMP, 0, 0, BRICK_WIDTH, BRICK_HEIGHT, t*BRICK_WIDTH + GAME_AREA_X, i*BRICK_WIDTH + GAME_AREA_Y, NULL);
			}
			
		}
}

void cGame::newGame() // restart game
{
	int i = 0;
	int t = 0;
	for (i = 0; i<BRICKS_Y; i++)
		for (t = 0; t < BRICKS_X; t++)
		{
			bricks[t][i].changeColor( rand() % BRICK_COLORS);
			bricks[t][i].changeState(FULL);
			
		}
	game_state = PLAY_GAME;
	score = 0;
	number_of_selected = 0;
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
void cGame::selectBrick(int _mouse_x, int _mouse_y) // takes mouse input and selects all same color bricks that are neighboruing to  bricks[x][y]
{
	int x = (_mouse_x - GAME_AREA_X) / BRICK_WIDTH;
	int y = (_mouse_y - GAME_AREA_Y) / BRICK_HEIGHT;
	int xx = 0;
	int yy = 0;
	int refresh = 0; 
	if (!selection && bricks[x][y].getState() == FULL) //if not selected and full
	{
		bricks[x][y].changeState(SELECTED);
		for (refresh = BRICKS_ON_SCREEN; refresh > 0; refresh--) // do this BRICKS_ON_SCREEN times to select all neighbouring bricks
		{

			for (xx = BRICKS_X - 1; xx >= 0; xx--)//scans for empty bricks and drops them by one
			for (yy = BRICKS_Y - 1; yy >= 0; yy--)
				{
					checkNeighbourBrick(xx, yy);
				}
			
		}
		
		if (checkNeighbourBrick(x,y)<1) bricks[x][y].changeState(FULL); //if only one block selected then unselect it
		selection = true;
	}else 
	if (selection && bricks[x][y].getState() == SELECTED) // else if selected
	{
		destroyBrick();
	}else 
	if (selection && bricks[x][y].getState() != SELECTED)
	{
		deselectBrick();	
	}

}

int cGame::checkNeighbourBrick(int x, int y)  //if x,y brick is already selected then all neighbouring/same color bricks are selected
{
	int selected = 0;
	if (bricks[x][y].getState() == SELECTED)
	{
	
		if (bricks[x][y].getColor() == bricks[x - 1][y].getColor() && bricks[x - 1][y].getState() != EMPTY && x>0)
		{
			bricks[x - 1][y].changeState(SELECTED);
			selected++;
		}

		if (bricks[x][y].getColor() == bricks[x + 1][y].getColor() && bricks[x + 1][y].getState() != EMPTY && x < BRICKS_X)
		{
			bricks[x + 1][y].changeState(SELECTED);
			selected++;
		}

		if (bricks[x][y].getColor() == bricks[x][y - 1].getColor() && bricks[x][y - 1].getState() != EMPTY && y > 0)
		{
			bricks[x][y - 1].changeState(SELECTED);
			selected++;
		}

		if (bricks[x][y].getColor() == bricks[x][y + 1].getColor() && bricks[x][y + 1].getState() != EMPTY && y < BRICKS_Y)
		{
			bricks[x][y + 1].changeState(SELECTED);
			selected++;
		}
		return selected;
	}
}
void cGame::deselectBrick() // clears selection of bricks 
{
	int i = 0;
	int t = 0;
	for (i = 0; i<BRICKS_Y; i++)
		for (t = 0; t < BRICKS_X; t++)
		{
			if (bricks[t][i].getState() == SELECTED)
			{ 
				bricks[t][i].changeState(FULL);
			}
			
		}
	selection = false;
}
void cGame::destroyBrick() // after clicking selected bricks destroys them
{
	int i = 0;
	int t = 0;
	for (i = 0; i < BRICKS_Y; i++)
		for (t = 0; t < BRICKS_X; t++)
		{
			if (bricks[t][i].getState() == SELECTED)
			{
				bricks[t][i].changeState(EMPTY);
			}
		}
	calculateScore();
	dropBrick();
	selection = false;
}
void cGame::calculateScore() //calculates score for destroyed bricks
{
	int i = 0;
	for (i = number_of_selected; i=0; i--)
	{
		changeScore(i*2);
	}
	

}
void cGame::dropBrick() //after destroying bricks fill holes by dropping them (checks from bottom)
{

	int xx = 0;
	int yy = 0;
	int refresh = 0;
	bool check_column = true;
	for (refresh = BRICKS_Y - 1; refresh > 0; refresh--) // do this BRICKS_Y times to drop bricks if there are bigger gaps 
	{
	
		for (xx = BRICKS_X-1; xx > 0; xx--)//scans for empty bricks and drops them by one
		for (yy = BRICKS_Y-1; yy > 0; yy--)
		{
			if (bricks[xx][yy].getState() == EMPTY)
			{
				bricks[xx][yy].changeColor(bricks[xx][yy-1].getColor());
				bricks[xx][yy].changeState(bricks[xx][yy-1].getState());
				bricks[xx][yy-1].changeState(EMPTY);
				
			}
		}
	}


}
void cGame::saveScores()//checks highscores & if your score is > than lowest highscore then prompts for username and saves it to file
{

}
