#include <stdlib.h>
#include <vector>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "classes.h"

//======cTile methods
cTile::cTile()//default constructor
{
	color = rand() % BRICK_COLORS; //needs EVALUATION
	state = FULL;
	
}
cTile::cTile(int _color, int _state) //constructor with parameters
{
	color = _color;
	state = _state;
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
	:bricks(BRICKS_LARGE_X, vector<cTile>(BRICKS_LARGE_Y))
{
	game_state = REFRESH_GAME;
	score = 0;
	selection = false;
	brick_size = 48;
	bricks_x=BRICKS_LARGE_X;
	bricks_y=BRICKS_LARGE_Y;
	bricks_on_screen = bricks_x * bricks_y;
	area_width = brick_size * bricks_x;
	area_height = brick_size * bricks_y;
	screen_width = LEFT_MARGIN + area_width + RIGHT_MARGIN;
	screen_height = TOP_MARGIN + area_height + DOWN_MARGIN;
	number_of_selected = 0;
	
	int i = 0; 
	for (i = 0; i < NUMBER_OF_FLAGS; i++)
	{
		flags[i] = false;
	}
} 
bool cGame::getFlags(int _flag)//return state of flag
{
	return flags[_flag];
}
void cGame::toggleFlags(int _flag)//flips value of a flag;
{
	flags[_flag] = !flags[_flag];
}
void cGame::changeFlags(int _flag, bool _value)//change flag to specific value
{
	flags[_flag] = _value; //use enum BUTTON_FLAGS as index number of array
}
int cGame::getGameState() //returns gamestate;
{
	return game_state;
}
int cGame::getScore() // returns score;
{
	return score;
}
int cGame::getNumberOfSelected()  //returns how many bricks are selected
{
	return number_of_selected;
}
int cGame::getBrickSize()//returns size of brick in pixels
{
	return brick_size;
}
void cGame::resetHighScores()
{
	//resets all saved highscores TODO
}
void cGame::changeBricksXY(int _x, int _y)//changes map size
{
	bricks_x =_x;
	bricks_y = _y;
	bricks_on_screen = bricks_x * bricks_y;
	area_width = brick_size * bricks_x;
	area_height = brick_size * bricks_y;
	screen_width = LEFT_MARGIN + area_width + RIGHT_MARGIN;
	screen_height = TOP_MARGIN + area_height + DOWN_MARGIN;
	
}
void cGame::changeBrickSize(int x)//changes brick size
{
	brick_size = x;
	area_width = brick_size * bricks_x;
	area_height = brick_size * bricks_y;
	screen_width = LEFT_MARGIN + area_width + RIGHT_MARGIN;
	screen_height = TOP_MARGIN + area_height + DOWN_MARGIN;
}
void cGame::changeScore(int _score) //passes int that changes actual score
{
	score += _score;
}
void cGame::changeGameState(int _game_state) // changes game_state;
{
	game_state = _game_state;
}
void cGame::updateNumberOfSelected() //checks and update number of selected bricks
{
	number_of_selected = 0;
	int i = 0;
	int t = 0;
	for (i = 0; i<bricks_y; i++)
		for (t = 0; t < bricks_x; t++)
		{
			if (bricks[t][i].getState() == SELECTED)
			{
				number_of_selected++;
			}

		}
}
void cGame::changeTile(int x, int y, int color) //changes color of x,y tile
{
	bricks[x][y].changeColor(color);
}
void cGame::drawGameArea(ALLEGRO_BITMAP *_bricksBMP) // draw all bricks on screen;
{
	int i = 0;
	int t = 0;

	for (i = 0; i<bricks_y; i++)
		for (t = 0; t < bricks_x; t++)
		{
			
			if (bricks[t][i].getState() !=EMPTY)
			{
				if (brick_size == BRICKS_48)	al_draw_bitmap_region(_bricksBMP, 1 * ((bricks[t][i].getColor() + 1)*getBrickSize()), 0, getBrickSize(), getBrickSize(), t*getBrickSize() + LEFT_MARGIN, i*getBrickSize() + TOP_MARGIN, NULL);
				if (brick_size == BRICKS_36)	al_draw_bitmap_region(_bricksBMP, 1 * ((bricks[t][i].getColor() + 1)*getBrickSize()), 48, getBrickSize(), getBrickSize(), t*getBrickSize() + LEFT_MARGIN, i*getBrickSize() + TOP_MARGIN, NULL);
				if (brick_size == BRICKS_24)	al_draw_bitmap_region(_bricksBMP, 1 * ((bricks[t][i].getColor() + 1)*getBrickSize()), 84, getBrickSize(), getBrickSize(), t*getBrickSize() + LEFT_MARGIN, i*getBrickSize() + TOP_MARGIN, NULL);
			}
			
			if (bricks[t][i].getState() == SELECTED)
			{
				al_draw_tinted_bitmap_region(_bricksBMP, TINT2, 0, 0, getBrickSize(), getBrickSize(), t*getBrickSize() + LEFT_MARGIN, i*getBrickSize() + TOP_MARGIN, NULL);
			}
			
		}
}
void cGame::newGame() // restart game
{
	int i = 0;
	int t = 0;
	for (i = 0; i<bricks_y; i++)
		for (t = 0; t < bricks_x; t++)
		{
			bricks[t][i].changeColor( rand() % BRICK_COLORS);
			bricks[t][i].changeState(FULL);
			
		}
	
	for (i = 0; i < NUMBER_OF_FLAGS; i++)
	{
		flags[i] = false;
	}
	game_state = PLAY_GAME;
	score = 0;
	number_of_selected = 0;
}
void cGame::endGame() //checks if game ended (no more bricks to destroy)
{
	//TODO
}

void cGame::highScores() // open high scores;
{
	game_state = HIGH_SCORE; //NEEDS EVALUATION COZ SAME AS changeState(HIGH_SCORE)
}

void cGame::options()// open options screen;
{
	game_state = OPTIONS; //NEEDS EVALUATION COZ SAME AS changeState(OPTIONS)
}
void cGame::selectBrick(int _mouse_x, int _mouse_y) // takes mouse input and selects all same color bricks that are neighboruing to  bricks[x][y]
{
	int x = (_mouse_x - LEFT_MARGIN) / getBrickSize();
	int y = (_mouse_y - TOP_MARGIN) / getBrickSize();
	int xx = 0;
	int yy = 0;
	int refresh = 0; 
	if (!selection && bricks[x][y].getState() == FULL) //if not selected and full
	{
		bricks[x][y].changeState(SELECTED);
		for (refresh = bricks_on_screen; refresh > 0; refresh--) // do this BRICKS_ON_SCREEN times to select all neighbouring bricks
		{

			for (xx = bricks_x - 1; xx >= 0; xx--)//scans neighbouring bricks
			for (yy = bricks_y - 1; yy >= 0; yy--)
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

		if (x > 0)//avoids going outside of vector
		{
			if (bricks[x][y].getColor() == bricks[x - 1][y].getColor() && bricks[x - 1][y].getState() != EMPTY)
			{
				bricks[x - 1][y].changeState(SELECTED);
				selected++;
			}
		}
		if (x+1 < bricks_x) //avoids going outside of vector
		{
			if (bricks[x][y].getColor() == bricks[x + 1][y].getColor() && bricks[x + 1][y].getState() != EMPTY)
			{
				bricks[x + 1][y].changeState(SELECTED);
				selected++;
			}
		}
		if (y > 0) //avoids going outside of vector
		{
			if (bricks[x][y].getColor() == bricks[x][y - 1].getColor() && bricks[x][y - 1].getState() != EMPTY )
			{
				bricks[x][y - 1].changeState(SELECTED);
				selected++;
			}
		}
		if (y+1 < bricks_y) //avoids going outside of vector
		{
			if (bricks[x][y].getColor() == bricks[x][y + 1].getColor() && bricks[x][y + 1].getState() != EMPTY )
			{
				bricks[x][y + 1].changeState(SELECTED);
				selected++;
			}
		}
		return selected;
	}
}
void cGame::deselectBrick() // clears selection of bricks 
{
	int i = 0;
	int t = 0;
	for (i = 0; i<bricks_y; i++)
		for (t = 0; t < bricks_x; t++)
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
	for (i = 0; i < bricks_y; i++)
		for (t = 0; t < bricks_x; t++)
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
	for (i = number_of_selected; i>=0; i--)
	{
		changeScore(i*2); //NEEDS EVALUATION
	}
	
}
void cGame::dropBrick() //after destroying bricks fill holes by dropping them (checks from bottom)
{
	int xx = 0;
	int yy = 0;
	int refresh = 0;
	bool check_column = true;
	for (refresh = bricks_y -1; refresh > 0; refresh--) // do this BRICKS_Y times to drop bricks if there are bigger gaps 
	{
		for (xx = bricks_x-1; xx >= 0; xx--)//scans for empty bricks and drops them by one
		for (yy = bricks_y-1; yy >0; yy--)
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
	//TODO
}
