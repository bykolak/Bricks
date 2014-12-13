#include <stdlib.h>
#include <vector>
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
	:bricks(BRICKS_X, vector<cTile>(BRICKS_Y))
{
	game_state = REFRESH_GAME;
	score = 0;
	selection = false;
	number_of_selected = 0;
	tile_size = 48;
} 
//cGame::cGame(cTile _bricks[BRICKS_X][BRICKS_Y]) //public constructor
//{}

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

int cGame::getTileSize()
{
	return tile_size;
}

void cGame::resetHighScores()
{
	//resets all saved highscores
}
void cGame::changeTileSize(int x)
{
	tile_size = x;
	
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

void cGame::changeTile(int x, int y, int color)
{
	bricks[x][y].changeColor(color);
}


void cGame::drawGameArea(ALLEGRO_BITMAP *_bricksBMP) // draw all bricks on screen;
{
	int i = 0;
	int t = 0;
	//ALLEGRO_BITMAP *temp[BRICK_COLORS];
	//vector <ALLEGRO_BITMAP> temp;
//	for (i = 0; i < BRICK_COLORS; i++)
//	{
//		al_set_target_bitmap(temp[i]);
//		al_draw_bitmap_region(_bricksBMP, 1 * i, 0, BRICK_WIDTH, BRICK_HEIGHT, , 0, 0, 0);
//	}
//	al_set_target_bitmap(al_get_backbuffer(display));
	for (i = 0; i<BRICKS_Y; i++)
		for (t = 0; t < BRICKS_X; t++)
		{
			if (bricks[t][i].getState() !=EMPTY)
			{
			//	al_draw_scaled_bitmap(temp[bricks[t][i].getColor()], 0, 0, BRICK_WIDTH, BRICK_HEIGHT, t*tile_size + LEFT_MARGIN, i*tile_size + TOP_MARGIN, NULL);
				al_draw_bitmap_region(_bricksBMP, 1 * ((bricks[t][i].getColor()+1)*BRICK_WIDTH), 0, BRICK_WIDTH, BRICK_HEIGHT, t*tile_size + LEFT_MARGIN, i*tile_size + TOP_MARGIN, NULL);
			}
			
			if (bricks[t][i].getState() == SELECTED)
			{
				al_draw_tinted_bitmap_region(_bricksBMP,TINT2, 0, 0, BRICK_WIDTH, BRICK_HEIGHT, t*tile_size + LEFT_MARGIN, i*tile_size + TOP_MARGIN, NULL);
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
	game_state = HIGH_SCORE;
}

void cGame::options()// open options screen;
{
	game_state = OPTIONS;
}
void cGame::selectBrick(int _mouse_x, int _mouse_y) // takes mouse input and selects all same color bricks that are neighboruing to  bricks[x][y]
{
	int x = (_mouse_x - LEFT_MARGIN) / BRICK_WIDTH;
	int y = (_mouse_y - TOP_MARGIN) / BRICK_HEIGHT;
	int xx = 0;
	int yy = 0;
	int refresh = 0; 
	if (!selection && bricks[x][y].getState() == FULL) //if not selected and full
	{
		bricks[x][y].changeState(SELECTED);
		for (refresh = BRICKS_ON_SCREEN; refresh > 0; refresh--) // do this BRICKS_ON_SCREEN times to select all neighbouring bricks
		{

			for (xx = BRICKS_X - 1; xx >= 0; xx--)//scans neighbouring bricks
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

		if (x > 0)//avoids going outside of vector
		{
			if (bricks[x][y].getColor() == bricks[x - 1][y].getColor() && bricks[x - 1][y].getState() != EMPTY)
			{
				bricks[x - 1][y].changeState(SELECTED);
				selected++;
			}
		}
		if (x+1 < BRICKS_X) //avoids going outside of vector
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
		if (y+1 < BRICKS_Y) //avoids going outside of vector
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
	for (i = number_of_selected; i>=0; i--)
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
	for (refresh = BRICKS_Y -1; refresh > 0; refresh--) // do this BRICKS_Y times to drop bricks if there are bigger gaps 
	{
	
		for (xx = BRICKS_X-1; xx >= 0; xx--)//scans for empty bricks and drops them by one
		for (yy = BRICKS_Y-1; yy >0; yy--)
	//	for (xx= 0; xx< BRICKS_Y; xx++)
	//		for (yy = 0; yy < BRICKS_X; yy++)
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
