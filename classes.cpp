#include <stdlib.h>
#include <vector>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>
#include "classes.h"
#include <cstdlib>


//======cTile methods
cTile::cTile()//default constructor
{
	color = 0; 
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
//=====cButton methods
cButton::cButton()//constructor
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	type = 0;
	text = "default";
	buttonBMP = NULL;
	buttonPressedBMP = NULL;
	
}
bool cButton::getFlags()//return state of flag
{
	return flags;
}
void cButton::toggleFlags()//flips value of a flag;
{
	flags = !flags;
}
void cButton::changeFlags( bool _value)//change flag to specific value
{
	flags = _value; 
}
bool cButton::overButton(int _mouse_x, int _mouse_y) //if inside button then change flags to true else make it false
{
	if (_mouse_x >= x && _mouse_x <= x + width && _mouse_y >= y && _mouse_y <= y + height)
	{
		flags = true;
		return true;
	}
	else
	{
		flags = false;
		return false;
	}
}
void cButton::changeButtonSize(int _x, int _y, int _width, int _height) //sets all button parameters
{
	x = _x; y = _y; width = _width; height = _height;
}
void cButton::createButton(ALLEGRO_BITMAP *temp)
{

	buttonBMP = al_create_bitmap(width, height);
	al_set_target_bitmap(buttonBMP);
	if (type<FAKE_BUTTON) al_draw_bitmap_region(temp, 0, type*31, width, height, 0, 0, 0);
	if (type>SMALL_BUTTON)al_draw_textf(arial24, WHITE, 10, 0, ALLEGRO_ALIGN_LEFT, text);
	else al_draw_textf(arial24, WHITE, width /2, 0, ALLEGRO_ALIGN_CENTRE, text);
	al_convert_mask_to_alpha(buttonBMP, MASK_COLOR);
	buttonPressedBMP = al_create_bitmap(width, height);
	al_set_target_bitmap(buttonPressedBMP);
	
	if (type<FAKE_BUTTON) al_draw_bitmap_region(temp, 0, type * 31, width, height, 0, 0, 0);
	if (type>SMALL_BUTTON)al_draw_textf(arial24, RED, 10, 0, ALLEGRO_ALIGN_LEFT, text);
	else al_draw_textf(arial24, RED, width / 2, 0, ALLEGRO_ALIGN_CENTRE, text);
	al_convert_mask_to_alpha(buttonPressedBMP, MASK_COLOR);
	al_set_target_bitmap(al_get_backbuffer(display));
}

void cButton::drawButton()//draw button on screen
{
	//if (state == _gamestate)
//	{		
		if (flags)
		{
			al_draw_bitmap(buttonPressedBMP, x, y, NULL);
			if (type < SHOW_ONLY_BUTTON) al_draw_rounded_rectangle(x, y, x + width, y + height, 15, 15, WHITE, 3); //only for type 0,1 not 2
		}
		else al_draw_bitmap(buttonBMP, x, y, NULL);
//	}
}

int cButton::getWidth()
{
	return width;
}
int cButton::getHeight()
{
	return height;
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
	player_name = NULL;
	al_init();
	display = al_create_display(screen_width, screen_height);
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	// LOADING GRAPHICS
	al_init_image_addon();
	bricksBMP = al_load_bitmap("bricks.bmp");
	optionsBMP = al_load_bitmap("options.bmp");
	buttonsBMP = al_load_bitmap("buttons.bmp");
	scoreBMP = al_load_bitmap("score.bmp");
	shadowBMP = al_create_bitmap(screen_width, screen_height);
	backgroundBMP = al_load_bitmap("background.bmp");
	al_set_target_bitmap(shadowBMP);
	al_clear_to_color(BLACK);
	al_set_target_bitmap(al_get_backbuffer(display));
	loadFromFile();
} 
void cGame::updateButtons(int mouseX,int mouseY)
{
	if (game_state == PLAY_GAME) //if playing game check these events
	{
		//if inside of button then change button[].flag to true value else false
		button[NEW_GAME_BUTTON].overButton(mouseX, mouseY);
		button[HIGH_SCORES_BUTTON].overButton(mouseX, mouseY);
		button[OPTIONS_BUTTON].overButton(mouseX, mouseY);
		button[GAME_AREA_BUTTON].overButton(mouseX, mouseY);
	}
	if (game_state == OPTIONS) //if in options check these events
	{
		button[OPTIONS_SMALL_BUTTON].overButton(mouseX, mouseY);
		button[OPTIONS_LARGE_BUTTON].overButton(mouseX, mouseY);
		button[OPTIONS_CAMPAIGN_BUTTON].overButton(mouseX, mouseY);
		button[OPTIONS_24_BUTTON].overButton(mouseX, mouseY);
		button[OPTIONS_36_BUTTON].overButton(mouseX, mouseY);
		button[OPTIONS_48_BUTTON].overButton(mouseX, mouseY);
	}
	if (game_state == HIGH_SCORE) //if in high scores check these events
	{
		button[HIGH_SCORES_RESET_BUTTON].overButton(mouseX, mouseY);
		button[HIGH_SCORES_CLOSE_BUTTON].overButton(mouseX, mouseY);
	}
	if (game_state == END_GAME) //if game ended check these events
	{
		button[END_GAME_NEW_GAME_BUTTON].overButton(mouseX, mouseY);
		button[END_GAME_SAVE_SCORE_BUTTON].overButton(mouseX, mouseY);
	}
	if (game_state == SAVING_SCORE) //if saving score check these events
	{
		button[END_GAME_NEW_GAME_BUTTON].overButton(mouseX, mouseY);
		button[END_GAME_SAVE_SCORE_BUTTON].overButton(mouseX, mouseY);
	}

}
void cGame::clickButtons(int mouseButton, int mouseX, int mouseY)
{
	if (mouseButton==1) //if left mouse button pressed
	{

		if (game_state == PLAY_GAME) //if playing game check these clicks
		{

			if (button[NEW_GAME_BUTTON].getFlags())		{ game_state = REFRESH_GAME; }
			if (button[HIGH_SCORES_BUTTON].getFlags())		{ game_state = HIGH_SCORE; }
			if (button[OPTIONS_BUTTON].getFlags())			{ game_state = OPTIONS; }
			if (button[GAME_AREA_BUTTON].getFlags())		{ selectBrick(mouseX, mouseY); }
		}
		if (game_state == OPTIONS) //if in options check these events
		{
			if (button[OPTIONS_SMALL_BUTTON].getFlags())		{ changeBricksXY(BRICKS_SMALL_X, BRICKS_SMALL_Y); game_state = REFRESH_GAME; }
			if (button[OPTIONS_LARGE_BUTTON].getFlags())		{ changeBricksXY(BRICKS_LARGE_X, BRICKS_LARGE_Y); game_state = REFRESH_GAME; }
			if (button[OPTIONS_CAMPAIGN_BUTTON].getFlags())	{}
			if (button[OPTIONS_24_BUTTON].getFlags())			{ changeBrickSize(BRICKS_SMALL); }
			if (button[OPTIONS_36_BUTTON].getFlags())			{ changeBrickSize(BRICKS_MEDIUM); }
			if (button[OPTIONS_48_BUTTON].getFlags())			{ changeBrickSize(BRICKS_LARGE); }
		}
		if (game_state == HIGH_SCORE) //if in high scores check these events
		{
			if (button[HIGH_SCORES_RESET_BUTTON].getFlags())	{ resetHighScores(); }
			if (button[HIGH_SCORES_CLOSE_BUTTON].getFlags())	{ game_state = PLAY_GAME; 	button[HIGH_SCORES_CLOSE_BUTTON].changeFlags(false); }
		}
		if (game_state == END_GAME) //if game ended
		{
			if (button[END_GAME_NEW_GAME_BUTTON].getFlags()) { game_state = REFRESH_GAME; }
			if (button[END_GAME_SAVE_SCORE_BUTTON].getFlags() && checkSaveScores()) { game_state = SAVING_SCORE; }
		}
		if (game_state == SAVING_SCORE) //if game ended
		{
			if (button[END_GAME_NEW_GAME_BUTTON].getFlags()) { game_state = REFRESH_GAME; }
			if (button[END_GAME_SAVE_SCORE_BUTTON].getFlags()) { game_state = SAVING_SCORE; }
		}

	}

	if (mouseButton == 2) //if right mouse button pressed
	{
		if (game_state == PLAY_GAME) //if playing game check these clicks
		{
			if (button[GAME_AREA_BUTTON].getFlags())		
			{ changeTile((mouseX - LEFT_MARGIN) / getBrickSize(), (mouseY - TOP_MARGIN) / getBrickSize(), 3); } //debug		
		}
		if (game_state == OPTIONS)		{ game_state = PLAY_GAME; }
		if (game_state == HIGH_SCORE)	{ game_state = PLAY_GAME; }

	}

}
void cGame::loadButton() //using predefined const int as placeholder
{
	
	// positions of buttons on main screen //NEEDS EVALUATION (needs to be scalable)
	const int NEW_GAME_X = 5;
	const int NEW_GAME_Y = 3;
	const int SCORE_X = 770;
	const int SCORE_Y = 3;
	//everything should be loaded from file. its temporary solution
	button[NEW_GAME_BUTTON].changeButtonSize(NEW_GAME_X, NEW_GAME_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[NEW_GAME_BUTTON].text = "NEW GAME";
	button[NEW_GAME_BUTTON].type = LARGE_BUTTON;

	button[HIGH_SCORES_BUTTON].changeButtonSize(HIGH_SCORES_X, HIGH_SCORES_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[HIGH_SCORES_BUTTON].text = "HIGH SCORES";
	button[HIGH_SCORES_BUTTON].type = LARGE_BUTTON;

	button[OPTIONS_BUTTON].changeButtonSize(OPTIONS_X, OPTIONS_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[OPTIONS_BUTTON].text = "OPTIONS";
	button[OPTIONS_BUTTON].type = LARGE_BUTTON;

	button[SCORE_BUTTON].changeButtonSize(SCORE_X, SCORE_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[SCORE_BUTTON].text = "SCORE";
	button[SCORE_BUTTON].type = SHOW_ONLY_BUTTON;

	button[GAME_AREA_BUTTON].changeButtonSize(LEFT_MARGIN, TOP_MARGIN, area_width, area_height);
	button[GAME_AREA_BUTTON].text = "";
	button[GAME_AREA_BUTTON].type = FAKE_BUTTON;

	button[OPTIONS_SMALL_BUTTON].changeButtonSize(OPTIONS_SMALL_X, OPTIONS_SMALL_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[OPTIONS_SMALL_BUTTON].text = "SMALL";
	button[OPTIONS_SMALL_BUTTON].type = LARGE_BUTTON;

	button[OPTIONS_LARGE_BUTTON].changeButtonSize(OPTIONS_LARGE_X, OPTIONS_LARGE_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[OPTIONS_LARGE_BUTTON].text = "LARGE";
	button[OPTIONS_LARGE_BUTTON].type = LARGE_BUTTON;

	button[OPTIONS_CAMPAIGN_BUTTON].changeButtonSize(OPTIONS_CAMPAIGN_X, OPTIONS_CAMPAIGN_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[OPTIONS_CAMPAIGN_BUTTON].text = "CAMPAIGN";
	button[OPTIONS_CAMPAIGN_BUTTON].type = LARGE_BUTTON;

	button[OPTIONS_24_BUTTON].changeButtonSize(OPTIONS_24_X, OPTIONS_24_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
	button[OPTIONS_24_BUTTON].text = "24";
	button[OPTIONS_24_BUTTON].type = SMALL_BUTTON;

	button[OPTIONS_36_BUTTON].changeButtonSize(OPTIONS_36_X, OPTIONS_36_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
	button[OPTIONS_36_BUTTON].text = "36";
	button[OPTIONS_36_BUTTON].type = SMALL_BUTTON;

	button[OPTIONS_48_BUTTON].changeButtonSize(OPTIONS_48_X, OPTIONS_48_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
	button[OPTIONS_48_BUTTON].text = "48";
	button[OPTIONS_48_BUTTON].type = SMALL_BUTTON;

	button[HIGH_SCORES_RESET_BUTTON].changeButtonSize(screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2) + END_GAME_SAVE_SCORE_X, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + END_GAME_SAVE_SCORE_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[HIGH_SCORES_RESET_BUTTON].text = "RESET SCORE";
	button[HIGH_SCORES_RESET_BUTTON].type = LARGE_BUTTON;

	button[HIGH_SCORES_CLOSE_BUTTON].changeButtonSize(screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2) + END_GAME_NEW_GAME_X, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + END_GAME_NEW_GAME_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[HIGH_SCORES_CLOSE_BUTTON].text = "CLOSE";
	button[HIGH_SCORES_CLOSE_BUTTON].type = LARGE_BUTTON;
	
	button[END_GAME_NEW_GAME_BUTTON].changeButtonSize(screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2) + END_GAME_NEW_GAME_X, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + END_GAME_NEW_GAME_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[END_GAME_NEW_GAME_BUTTON].text = "PLAY AGAIN?";
	button[END_GAME_NEW_GAME_BUTTON].type = LARGE_BUTTON;
	
	button[END_GAME_SAVE_SCORE_BUTTON].changeButtonSize(screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2) + END_GAME_SAVE_SCORE_X, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + END_GAME_SAVE_SCORE_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[END_GAME_SAVE_SCORE_BUTTON].text = "SAVE SCORES";
	button[END_GAME_SAVE_SCORE_BUTTON].type = LARGE_BUTTON;

	int i = 0;
	for (i = 0; i < NUMBER_OF_BUTTONS; i++)
	{

		button[i].createButton(buttonsBMP);
	}

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
void cGame::resetHighScores() //resets all saved highscores TODO
{
	int i = 0;
	for (i = 0 - 1; i < MAX_HIGH_SCORE; i++)
	{

		high_score[i] = 0;
		high_score_name[i] = al_ustr_new("  ");
	}
	saveToFile();
	
}
void cGame::changeBricksXY(int _x, int _y)//changes map size
{
	bricks_x =_x;
	bricks_y = _y;
	bricks_on_screen = bricks_x * bricks_y;
	area_width = brick_size * bricks_x;
	area_height = brick_size * bricks_y;
	screen_width = LEFT_MARGIN + area_width + RIGHT_MARGIN;
	if (screen_width < MIN_SCREEN_X) screen_width = MIN_SCREEN_X;
	screen_height = TOP_MARGIN + area_height + DOWN_MARGIN;
	if (screen_height < MIN_SCREEN_Y) screen_height = MIN_SCREEN_Y;
	button[GAME_AREA_BUTTON].changeButtonSize(LEFT_MARGIN, TOP_MARGIN, area_width, area_height);
	al_resize_display(display, screen_width, screen_height);

	button[HIGH_SCORES_CLOSE_BUTTON].changeButtonSize(screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2) + END_GAME_NEW_GAME_X, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + END_GAME_NEW_GAME_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[HIGH_SCORES_RESET_BUTTON].changeButtonSize(screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2) + END_GAME_SAVE_SCORE_X, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + END_GAME_SAVE_SCORE_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[END_GAME_NEW_GAME_BUTTON].changeButtonSize(screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2) + END_GAME_NEW_GAME_X, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + END_GAME_NEW_GAME_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[END_GAME_SAVE_SCORE_BUTTON].changeButtonSize(screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2) + END_GAME_SAVE_SCORE_X, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + END_GAME_SAVE_SCORE_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	
	
}
void cGame::changeBrickSize(int x)//changes brick size
{
	brick_size = x;
	area_width = brick_size * bricks_x;
	area_height = brick_size * bricks_y;
	screen_width = LEFT_MARGIN + area_width + RIGHT_MARGIN;
	if (screen_width < MIN_SCREEN_X) screen_width = MIN_SCREEN_X;
	screen_height = TOP_MARGIN + area_height + DOWN_MARGIN;
	if (screen_height < MIN_SCREEN_Y) screen_height = MIN_SCREEN_Y;
	button[GAME_AREA_BUTTON].changeButtonSize(LEFT_MARGIN, TOP_MARGIN, area_width, area_height);
	al_resize_display(display, screen_width, screen_height); // casuses crash
	//left_margin = (screen_width - area_width) / 2;
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
void cGame::drawGameArea() // draw all bricks on screen;
{
	int  x_times = screen_width / al_get_bitmap_width(backgroundBMP) + 1;
	int  y_times = screen_height / al_get_bitmap_height(backgroundBMP) + 1;
	for (int i = 0; i < x_times; i++)
		for (int t = 0; t < y_times; t++)	al_draw_bitmap(backgroundBMP, i * 128, t * 128, NULL);
	
	al_draw_tinted_bitmap_region(shadowBMP, TINT3,0, 0, button[GAME_AREA_BUTTON].getWidth(), button[GAME_AREA_BUTTON].getHeight(), LEFT_MARGIN, TOP_MARGIN, NULL);
	al_draw_rectangle(LEFT_MARGIN, TOP_MARGIN, button[GAME_AREA_BUTTON].getWidth() + LEFT_MARGIN+1, button[GAME_AREA_BUTTON].getHeight() + TOP_MARGIN+1, BLACK, 3);
	int i = 0;
	int t = 0;

	for (i = 0; i<bricks_y; i++)
		for (t = 0; t < bricks_x; t++)
		{
			
			if (bricks[t][i].getState() !=EMPTY)
			{
				if (brick_size == BRICKS_LARGE)	al_draw_bitmap_region(bricksBMP, 1 * ((bricks[t][i].getColor() + 1)*getBrickSize()), 0, getBrickSize(), getBrickSize(), t*getBrickSize() + LEFT_MARGIN, i*getBrickSize() + TOP_MARGIN, NULL);
				if (brick_size == BRICKS_MEDIUM)	al_draw_bitmap_region(bricksBMP, 1 * ((bricks[t][i].getColor() + 1)*getBrickSize()), 48, getBrickSize(), getBrickSize(), t*getBrickSize() + LEFT_MARGIN, i*getBrickSize() + TOP_MARGIN, NULL);
				if (brick_size == BRICKS_SMALL)	al_draw_bitmap_region(bricksBMP, 1 * ((bricks[t][i].getColor() + 1)*getBrickSize()), 84, getBrickSize(), getBrickSize(), t*getBrickSize() + LEFT_MARGIN, i*getBrickSize() + TOP_MARGIN, NULL);
			}
			
			if (bricks[t][i].getState() == SELECTED)
			{
				al_draw_tinted_bitmap_region(bricksBMP, TINT2, 0, 0, getBrickSize(), getBrickSize(), t*getBrickSize() + LEFT_MARGIN, i*getBrickSize() + TOP_MARGIN, NULL);
			}
			
		}
	button[NEW_GAME_BUTTON].drawButton();
	button[HIGH_SCORES_BUTTON].drawButton();
	button[OPTIONS_BUTTON].drawButton();
	button[SCORE_BUTTON].drawButton();
	al_draw_textf(arial24, WHITE, DEBUG_X + 240, DEBUG_Y + 2, ALLEGRO_ALIGN_RIGHT, " %i", score);
}
void cGame::newGame() // restart game
{
	int i = 0;
	int t = 0;
	for (i = 0; i<bricks_y; i++)
		for (t = 0; t < bricks_x; t++)
		{
			bricks[t][i].changeColor( rand() % BRICK_COLORS);  
		//	bricks[t][i].changeColor(1);//DEBUG
			/////////////////////////////////////////////////////////////////
			bricks[t][i].changeState(FULL);
		}
	
	for (i = 0; i < NUMBER_OF_BUTTONS; i++)
	{
		button[i].changeFlags(false);
	}
	game_state = PLAY_GAME;
	score = 0;
	number_of_selected = 0;
	saved_scores = false;
}
void cGame::checkEndGame() //checks if game ended (no more bricks to destroy)
{
	int selected = 0;
	int x = 0;
	int y = 0;
	
		for (x = bricks_x - 1; x >= 0; x--)
			for (y = bricks_y - 1; y >= 0; y--)
			{
				if (bricks[x][y].getState()!=EMPTY)
				{

					if (x > 0)//avoids going outside of vector
					{
					if (bricks[x][y].getColor() == bricks[x - 1][y].getColor() && bricks[x - 1][y].getState() != EMPTY)		{ selected++; }
					}
					if (x + 1 < bricks_x) //avoids going outside of vector
					{
					if (bricks[x][y].getColor() == bricks[x + 1][y].getColor() && bricks[x + 1][y].getState() != EMPTY)		{ selected++; }
					}
					if (y > 0) //avoids going outside of vector
					{
					if (bricks[x][y].getColor() == bricks[x][y - 1].getColor() && bricks[x][y - 1].getState() != EMPTY) 	{ selected++; }
					}
					if (y + 1 < bricks_y) //avoids going outside of vector
					{
					if (bricks[x][y].getColor() == bricks[x][y + 1].getColor() && bricks[x][y + 1].getState() != EMPTY)		{ selected++; }
					}
				}

			}
		if (selected == 0) game_state = END_GAME;
		
	}

	
	


void cGame::highScores() // open high scores;
{
	al_draw_tinted_bitmap(shadowBMP, TINT, 0, 0, 0);
	al_draw_bitmap(scoreBMP, screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2), screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2), 0);
	button[HIGH_SCORES_RESET_BUTTON].drawButton();
	button[HIGH_SCORES_CLOSE_BUTTON].drawButton();
	for (int i = 0; i < MAX_HIGH_SCORE; i++)
	{
		al_draw_ustr(arial24, WHITE, screen_width / 2 - 300, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + 25 * i + 6, ALLEGRO_ALIGN_LEFT, high_score_name[i]);


		al_draw_textf(arial24, WHITE, screen_width / 2 + 250, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + 25 * i + 6, ALLEGRO_ALIGN_CENTRE, "%i ", high_score[i]);
	}
}

void cGame::options()// open options screen;
{
	al_draw_tinted_bitmap(shadowBMP, TINT, 0, 0, 0);
	al_draw_bitmap(optionsBMP, OPTIONS_X, TOP_MARGIN, 0);
	button[OPTIONS_SMALL_BUTTON].drawButton();
	button[OPTIONS_LARGE_BUTTON].drawButton();
	button[OPTIONS_CAMPAIGN_BUTTON].drawButton();
	button[OPTIONS_24_BUTTON].drawButton();
	button[OPTIONS_36_BUTTON].drawButton();
	button[OPTIONS_48_BUTTON].drawButton();
}
void cGame::endGame()
{
	al_draw_tinted_bitmap(shadowBMP, TINT, 0, 0, 0);
	al_draw_bitmap(scoreBMP, screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2), screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2), 0);
	button[END_GAME_NEW_GAME_BUTTON].drawButton();
	if (checkSaveScores()) button[END_GAME_SAVE_SCORE_BUTTON].drawButton();
	for (int i = 0; i < MAX_HIGH_SCORE; i++)
	{
		al_draw_ustr(arial24, WHITE, screen_width / 2 - 300, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + 25 * i + 6, ALLEGRO_ALIGN_LEFT, high_score_name[i]);
		al_draw_textf(arial24, WHITE, screen_width / 2 + 250, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + 25 * i + 6, ALLEGRO_ALIGN_CENTRE, "%i ", high_score[i]);
	}

	al_draw_textf(arial24, WHITE, screen_width / 2, screen_height / 2 + 75, ALLEGRO_ALIGN_CENTRE, "you scored %i points !", score);

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
	moveBrickLeft();
}
void cGame::moveBrickLeft()
{
	for (int x = 0; x < bricks_x; x++)
	{
		for (int y = 0; y < bricks_y; y++)
		{
			if (bricks[x][y].getState() == EMPTY)
			{
				//TODO
			}

		}
	}
}
bool cGame::checkSaveScores()//checks highscores & if your score is > than lowest highscore then prompts for username and saves it to file
{
	if (!saved_scores)
	{
		int i;
		int t = 0;
		for (i = 0; i<MAX_HIGH_SCORE; i++)
		{
			if (score>high_score[i])	{ t++; }
		}
		if (t > 0) return true;
		else return false;
	}
	return false;
}
void cGame::saveScores()
{
int i = 0;
int t = 0;

if (!saved_scores)
{
	for (i = 0; i<MAX_HIGH_SCORE; i++)
	{
		if (score>high_score[i])
		{
			for (t = MAX_HIGH_SCORE - 1; t > i; t--)
			{

				high_score[t] = high_score[t - 1];
			}
			high_score[i] = score;
			high_score_name[i] = player_name;
			player_name = al_ustr_new("");
			edited_text = al_ustr_new("");
			game_state = END_GAME;
			saved_scores = true;
			i = MAX_HIGH_SCORE + 1;
		}
	}
}
saveToFile();


}
void cGame::enterPlayerName(int keycode, int unichar)
{
	if (keycode <= ALLEGRO_KEY_9 || keycode == ALLEGRO_KEY_SPACE) //if A-Z and 0-9 pressed or spacebar
		if (al_ustr_length(edited_text) < MAX_USERNAME_LENGTH) al_ustr_append_chr(edited_text, unichar);

	if (keycode == ALLEGRO_KEY_BACKSPACE)
	{
		if (al_ustr_length(edited_text) > 0)	{ al_ustr_truncate(edited_text, al_ustr_length(edited_text) - 1); }
	}
	if (keycode == ALLEGRO_KEY_ENTER|| keycode== ALLEGRO_KEY_PAD_ENTER)
	{
		player_name = edited_text;
		saveScores();
	}
}

void cGame::saveToFile()
{
	ALLEGRO_FILE* high_score_file = al_fopen("high_score.bri", "wb");
	int i = 0;
	for (i = 0; i < MAX_HIGH_SCORE; i++)
	{
		int buffer = al_ustr_size(high_score_name[i]);
		al_fwrite32le(high_score_file, buffer);
		al_fwrite(high_score_file, al_cstr(high_score_name[i]), buffer);
		al_fwrite(high_score_file, &high_score[i], sizeof(high_score[i]));

	}
	al_fclose(high_score_file);
}
void cGame::loadFromFile()
{
	ALLEGRO_FILE* high_score_file = al_fopen("high_score.bri", "rb");
	int i = 0;
	for (i = 0; i < MAX_HIGH_SCORE; i++)
	{
		int size = al_fread32le(high_score_file);

		if (size > 0)
		{
			char *buffer = (char *)al_malloc(size);
			al_fread(high_score_file, buffer, size);
			high_score_name[i] = al_ustr_new_from_buffer(buffer, size);

			int *buffer2 = &high_score[i];
			al_fread(high_score_file, buffer2, sizeof(high_score[i]));

			al_free(buffer);
		}
	}

	al_fclose(high_score_file);
}