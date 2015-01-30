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
	score_count = 0;
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
	left_game_area_margin = (screen_width - area_width) / 2;
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
	bricksBMP = al_load_bitmap("bricks.png");
	optionsBMP = al_load_bitmap("options.bmp");
	buttonsBMP = al_load_bitmap("buttons.png");
	scoreBMP = al_load_bitmap("score.bmp");
	shadowBMP = al_create_bitmap(screen_width, screen_height);
	backgroundBMP = al_load_bitmap("background.bmp");
	al_set_target_bitmap(shadowBMP);
	al_clear_to_color(BLACK);
	al_set_target_bitmap(al_get_backbuffer(display));
	loadFromFile();
} 

void cGame::updateScore()//updates on_screen score
{
	int score_delay = 0;
	if (score - on_screen_score < 60) score_delay = 4;
	if (score - on_screen_score > 60) score_delay = 3;
	if (score - on_screen_score > 500) score_delay = 2;
	if (score - on_screen_score > 1000) score_delay = 1;


	if (++score_count >= score_delay)
	{
		if (score_delay == 4)	on_screen_score += 1;
		if (score_delay == 3)	on_screen_score += 3;
		if (score_delay == 2)	on_screen_score += 11;
		if (score_delay == 1)	on_screen_score += 111;
		if (on_screen_score > score) on_screen_score = score;
		score_count = 0;
	}
}
void cGame::drawScore()//draws score to the screen
{
	al_draw_textf(arial24, WHITE, left_button_margin + (SCORE_BUTTON*LEFT_MARGIN) + (BUTTON_WIDTH*SCORE_BUTTON) + 240, 5, ALLEGRO_ALIGN_RIGHT, " %i", on_screen_score);
}




void cGame::checkButtons(int mouseX,int mouseY)
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
		//	if (button[END_GAME_SAVE_SCORE_BUTTON].getFlags() && !saved_scores) { player_name = edited_text; saveScores(); }
		}

	}

	if (mouseButton == 2) //if right mouse button pressed
	{
		if (game_state == PLAY_GAME) //if playing game check these clicks
		{
			if (button[NEW_GAME_BUTTON].getFlags())		{ game_state = CHEAT; }
			if (button[GAME_AREA_BUTTON].getFlags())		
			{
				changeTile((mouseX - left_game_area_margin ) / brick_size, (mouseY - TOP_MARGIN) / brick_size, 3);
			} //debug		
		}
		if (game_state == OPTIONS)		{ game_state = PLAY_GAME; }
		if (game_state == HIGH_SCORE)	{ game_state = PLAY_GAME; }
		
	}

}
void cGame::updatePositions()
{
		bricks_on_screen = bricks_x * bricks_y;
		area_width = brick_size * bricks_x;
		area_height = brick_size * bricks_y;
		screen_width = LEFT_MARGIN + area_width + RIGHT_MARGIN;
		if (screen_width < MIN_SCREEN_X) screen_width = MIN_SCREEN_X;

		left_game_area_margin = (screen_width - area_width) / 2;

		screen_height = TOP_MARGIN + area_height + DOWN_MARGIN;
		if (screen_height <= MIN_SCREEN_Y) screen_height = MIN_SCREEN_Y;
		left_button_margin = (screen_width - (4 * BUTTON_WIDTH + 3 * LEFT_MARGIN)) / 2;
		for (int i = 0; i <= SCORE_BUTTON; i++)
		{
			button[i].changeButtonSize(left_button_margin + (i*LEFT_MARGIN) + (BUTTON_WIDTH*i), 3, BUTTON_WIDTH, BUTTON_HEIGHT);
		}
		int optionsX = left_button_margin + (2 * LEFT_MARGIN) + (BUTTON_WIDTH * 2);
		button[OPTIONS_SMALL_BUTTON].changeButtonSize(optionsX + 5, OPTIONS_SMALL_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
		button[OPTIONS_LARGE_BUTTON].changeButtonSize(optionsX + 5, OPTIONS_LARGE_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
		button[OPTIONS_CAMPAIGN_BUTTON].changeButtonSize(optionsX + 5, OPTIONS_CAMPAIGN_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
		
		for (int i = OPTIONS_24_BUTTON; i <= OPTIONS_48_BUTTON; i++)
		{
			button[i].changeButtonSize(optionsX + 16 + ((i - OPTIONS_24_BUTTON) * SMALL_BUTTON_WIDTH + (i - OPTIONS_24_BUTTON) * LEFT_MARGIN), OPTIONS_SIZE_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
		}


		button[GAME_AREA_BUTTON].changeButtonSize(left_game_area_margin, TOP_MARGIN, area_width, area_height);

		button[HIGH_SCORES_CLOSE_BUTTON].changeButtonSize(screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2) + END_GAME_NEW_GAME_X, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + END_GAME_NEW_GAME_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
		button[HIGH_SCORES_RESET_BUTTON].changeButtonSize(screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2) + END_GAME_SAVE_SCORE_X, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + END_GAME_SAVE_SCORE_Y, BUTTON_WIDTH, BUTTON_HEIGHT);

		button[END_GAME_NEW_GAME_BUTTON].changeButtonSize(screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2) + END_GAME_NEW_GAME_X, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + END_GAME_NEW_GAME_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
		button[END_GAME_SAVE_SCORE_BUTTON].changeButtonSize(screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2) + END_GAME_SAVE_SCORE_X, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + END_GAME_SAVE_SCORE_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	if (update_position)
	{
		al_resize_display(display, screen_width, screen_height);
		update_position = false;
	}
}
void cGame::loadButton() //using predefined const int as placeholder
{
	
	// positions of buttons on main screen //NEEDS EVALUATION (needs to be scalable)
	for (int i = 0; i <= SCORE_BUTTON; i++)
	{
		button[i].changeButtonSize(LEFT_MARGIN + (i*LEFT_MARGIN) + (BUTTON_WIDTH*i), 3, BUTTON_WIDTH, BUTTON_HEIGHT);
	}

	button[NEW_GAME_BUTTON].text = "NEW GAME";
	button[NEW_GAME_BUTTON].type = LARGE_BUTTON;

	button[HIGH_SCORES_BUTTON].text = "HIGH SCORES";
	button[HIGH_SCORES_BUTTON].type = LARGE_BUTTON;

	button[OPTIONS_BUTTON].text = "OPTIONS";
	button[OPTIONS_BUTTON].type = LARGE_BUTTON;

	button[SCORE_BUTTON].text = "SCORE";
	button[SCORE_BUTTON].type = SHOW_ONLY_BUTTON;

	button[GAME_AREA_BUTTON].changeButtonSize(left_game_area_margin, TOP_MARGIN, area_width, area_height);
	button[GAME_AREA_BUTTON].text = "";
	button[GAME_AREA_BUTTON].type = FAKE_BUTTON;

	button[OPTIONS_SMALL_BUTTON].changeButtonSize(0, OPTIONS_SMALL_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[OPTIONS_SMALL_BUTTON].text = "SMALL";
	button[OPTIONS_SMALL_BUTTON].type = LARGE_BUTTON;

	button[OPTIONS_LARGE_BUTTON].changeButtonSize(0, OPTIONS_LARGE_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[OPTIONS_LARGE_BUTTON].text = "LARGE";
	button[OPTIONS_LARGE_BUTTON].type = LARGE_BUTTON;

	button[OPTIONS_CAMPAIGN_BUTTON].changeButtonSize(0, OPTIONS_CAMPAIGN_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	button[OPTIONS_CAMPAIGN_BUTTON].text = "CAMPAIGN";
	button[OPTIONS_CAMPAIGN_BUTTON].type = LARGE_BUTTON;

	button[OPTIONS_24_BUTTON].changeButtonSize(0, 0, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
	button[OPTIONS_24_BUTTON].text = "24";
	button[OPTIONS_24_BUTTON].type = SMALL_BUTTON;

	button[OPTIONS_36_BUTTON].changeButtonSize(0, 0, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
	button[OPTIONS_36_BUTTON].text = "36";
	button[OPTIONS_36_BUTTON].type = SMALL_BUTTON;

	button[OPTIONS_48_BUTTON].changeButtonSize(0, 0, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
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
bool cGame::checkGameState(int state)//returns true if state=game_state
{
	if (state == game_state)	{ return true; }
	else { return false; }
}
void cGame::resetHighScores() //resets all saved highscores TODO
{
	int i = 0;
	for (i = 0 - 1; i < MAX_HIGH_SCORE; i++)
	{

		high_score[i] = 0;
		high_score_name[i] = al_ustr_new(".....");
	}
	saveToFile();
	
}
void cGame::changeBricksXY(int _x, int _y)//changes map size
{
	bricks_x =_x;
	bricks_y = _y;
	update_position = true;	
}
void cGame::changeBrickSize(int x)//changes brick size
{
	brick_size = x;
	update_position = true;
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
	
	left_game_area_margin = (screen_width - area_width) / 2;
	al_draw_tinted_bitmap_region(shadowBMP, TINT3, 0, 0, button[GAME_AREA_BUTTON].getWidth(), button[GAME_AREA_BUTTON].getHeight(), left_game_area_margin, TOP_MARGIN, NULL);
	al_draw_rectangle(left_game_area_margin, TOP_MARGIN, button[GAME_AREA_BUTTON].getWidth() + left_game_area_margin + 1, button[GAME_AREA_BUTTON].getHeight() + TOP_MARGIN + 1, BLACK, 3);
	int i = 0;
	int t = 0;

	for (i = 0; i<bricks_y; i++)
		for (t = 0; t < bricks_x; t++)
		{
			
			if (bricks[t][i].getState() !=EMPTY)
			{
				if (brick_size == BRICKS_LARGE)		al_draw_bitmap_region(bricksBMP, 1 * ((bricks[t][i].getColor() )*brick_size), 0, brick_size, brick_size, t*brick_size + left_game_area_margin, i*brick_size + TOP_MARGIN, NULL);
				if (brick_size == BRICKS_MEDIUM)	al_draw_bitmap_region(bricksBMP, 1 * ((bricks[t][i].getColor() )*brick_size), 48, brick_size, brick_size, t*brick_size + left_game_area_margin, i*brick_size + TOP_MARGIN, NULL);
				if (brick_size == BRICKS_SMALL)		al_draw_bitmap_region(bricksBMP, 1 * ((bricks[t][i].getColor() )*brick_size), 96, brick_size, brick_size, t*brick_size + left_game_area_margin, i*brick_size + TOP_MARGIN, NULL);
			}
			
			if (bricks[t][i].getState() == SELECTED)
			{
				al_draw_tinted_bitmap_region(bricksBMP, TINT2, 0, 144, brick_size, brick_size, t*brick_size + left_game_area_margin, i*brick_size + TOP_MARGIN, NULL); //needs fixing
			}
			
		}
	button[NEW_GAME_BUTTON].drawButton();
	button[HIGH_SCORES_BUTTON].drawButton();
	button[OPTIONS_BUTTON].drawButton();
	button[SCORE_BUTTON].drawButton();
	drawScore();
}
void cGame::newGame(bool debug) // restart game
{
	int i = 0;
	int t = 0;
	for (i = 0; i<bricks_y; i++)
		for (t = 0; t < bricks_x; t++)
		{
			bricks[t][i].changeColor( rand() % BRICK_COLORS);  
			if (debug) bricks[t][i].changeColor(1);//DEBUG
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
//	screen_height = TOP_MARGIN + area_height + DOWN_MARGIN;//helps with bug that makes screen_height 0 after clicking reset score button 
	al_draw_tinted_bitmap(shadowBMP, TINT, 0, 0, 0);
	al_draw_bitmap(scoreBMP, screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2), (screen_height / 2) - (al_get_bitmap_height(scoreBMP) / 2), 0);

	button[HIGH_SCORES_RESET_BUTTON].drawButton();
	button[HIGH_SCORES_CLOSE_BUTTON].drawButton();
	al_draw_textf(arial24, RED, 0, 0, 0, "XX: %d  YY: %d", xx, yy);
	al_draw_textf(arial24, RED, 0, 30, 0, "bitmap height: %d screen height: %d", al_get_bitmap_height(scoreBMP), screen_height);
	for (int i = 0; i < MAX_HIGH_SCORE; i++)
	{
		al_draw_ustr(arial24, WHITE, screen_width / 2 - 300, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + (25 * i) + 6, ALLEGRO_ALIGN_LEFT, high_score_name[i]);


		al_draw_textf(arial24, WHITE, screen_width / 2 + 250, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + (25 * i) + 6, ALLEGRO_ALIGN_CENTRE, "%i ", high_score[i]);
	}
}

void cGame::options()// open options screen;
{
	al_draw_tinted_bitmap(shadowBMP, TINT, 0, 0, 0);
	al_draw_bitmap(optionsBMP, left_button_margin + (OPTIONS_BUTTON*LEFT_MARGIN) + (BUTTON_WIDTH*OPTIONS_BUTTON), TOP_MARGIN, 0);
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

	al_draw_textf(arial24, WHITE, screen_width / 2, screen_height / 2 + 75, ALLEGRO_ALIGN_CENTRE, "you scored %i points !", on_screen_score);

}
void cGame::selectBrick(int _mouse_x, int _mouse_y) // takes mouse input and selects all same color bricks that are neighboruing to  bricks[x][y]
{
	int x = (_mouse_x - left_game_area_margin) / brick_size;
	int y = (_mouse_y - TOP_MARGIN) / brick_size;
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
	return 0;
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
		score+= i*2; //NEEDS EVALUATION
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
		int empty = 0;
		for (int y = 0; y < bricks_y; y++)
		{
			if (bricks[x][y].getState() == EMPTY)
			{
				empty++;
				//TODO
			}
			if (empty == bricks_y)
			{
					for (int xx = x; xx < bricks_x; xx++)
					for (int yy = 0; yy < bricks_y; yy++)
					{
						if (xx + 1 < bricks_x) //avoids going outside of vector
						{
							bricks[xx][yy].changeColor(bricks[xx + 1][yy].getColor());
							bricks[xx][yy].changeState(bricks[xx + 1][yy].getState());
							bricks[xx + 1][yy].changeColor(0);
							bricks[xx + 1][yy].changeState(EMPTY);
						}
					}
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