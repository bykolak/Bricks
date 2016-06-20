#include <vector>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include "classes.h"

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
	if (type>SMALL_BUTTON)al_draw_textf(font18, WHITE, 10, 5, ALLEGRO_ALIGN_LEFT, text);
	else
	{
		int f =1;
		al_draw_textf(font18, BLACK, width / 2+f,5, ALLEGRO_ALIGN_CENTRE, text);
		al_draw_textf(font18, BLACK, width / 2 - f, 5, ALLEGRO_ALIGN_CENTRE, text);
		al_draw_textf(font18, WHITE, width / 2,5, ALLEGRO_ALIGN_CENTRE, text);
		
	}
	al_convert_mask_to_alpha(buttonBMP, MASK_COLOR);
	buttonPressedBMP = al_create_bitmap(width, height);
	al_set_target_bitmap(buttonPressedBMP);
	
	if (type<FAKE_BUTTON) al_draw_bitmap_region(temp, 0, type * 31, width, height, 0, 0, 0);
	if (type>SMALL_BUTTON)al_draw_textf(font18, YELLOW, 10, 2, ALLEGRO_ALIGN_LEFT, text);
	else al_draw_textf(font18, YELLOW, width / 2, 2, ALLEGRO_ALIGN_CENTRE, text);
	al_convert_mask_to_alpha(buttonPressedBMP, MASK_COLOR);
	al_set_target_bitmap(al_get_backbuffer(display));
}
void cButton::drawButton()//draw button on screen
{
		if (flags)
		{
			al_draw_bitmap(buttonPressedBMP, x, y, NULL);
			if (type < SHOW_ONLY_BUTTON) al_draw_rounded_rectangle(x, y, x + width, y + height, 15, 15, WHITE, 3); //only for type 0,1 not 2
		}
		else al_draw_bitmap(buttonBMP, x, y, NULL);
}
//=====cGame methods
cGame::cGame() //default constructor
	:bricks(BRICKS_MAP_X, std::vector<cTile>(BRICKS_MAP_Y))
{
	currently_selected = 0;
	score_count = 0;
	game_state = REFRESH_GAME;
	score = 0;
	selection = false;
	destroy_brick = false;
	brick_size = BRICK_SIZE;
	bricks_on_screen = BRICKS_MAP_X * BRICKS_MAP_Y;
	area_width = brick_size * BRICKS_MAP_X;
	area_height = brick_size * BRICKS_MAP_Y;
	screen_width = LEFT_MARGIN + area_width + RIGHT_MARGIN;
	screen_height = TOP_MARGIN + area_height + DOWN_MARGIN;
	left_game_area_margin = (screen_width - area_width) / 2;
	curFrame = 0;
	frameCount = 0;
	number_of_selected = 0;
	player_name = NULL;
	al_init();
	display = al_create_display(screen_width, screen_height);
	al_set_window_position(display, 20, 20);
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	// LOADING GRAPHICS
	al_init_image_addon();
	bricksBMP = al_load_bitmap("bricks.png");
	optionsBMP = al_load_bitmap("options.png");
	buttonsBMP = al_load_bitmap("buttons.png");
	scoreBMP = al_load_bitmap("score.bmp");
	explosionBMP = al_load_bitmap("explosion.png");
	shadowBMP = al_create_bitmap(screen_width, screen_height);
	backgroundBMP = al_load_bitmap("background.png");
	al_set_target_bitmap(shadowBMP);
	al_clear_to_color(BLACK);
	al_set_target_bitmap(al_get_backbuffer(display));
	loadHighScore();
	loadGame();
	game_state = PLAY_GAME;
	//LOADING SOUND
	al_install_audio();
	al_init_acodec_addon();

	al_reserve_samples(10 );
	explosionOGG = al_load_sample("explosion.ogg");
	clickWAV = al_load_sample("button.wav");
	instance = al_create_sample_instance(explosionOGG);
	instance2 = al_create_sample_instance(explosionOGG);
	instanceClick = al_create_sample_instance(clickWAV);
	instanceClick2 = al_create_sample_instance(clickWAV);
	al_set_sample_instance_speed(instanceClick2, 1.5);
	al_set_sample_instance_gain(instance, 0.5);
	al_attach_sample_instance_to_mixer(instance, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance2, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instanceClick, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instanceClick2, al_get_default_mixer());
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
	int f = 1;
	al_draw_textf(font18, BLACK, left_button_margin + (SCORE_BUTTON*LEFT_MARGIN) + (BUTTON_WIDTH*SCORE_BUTTON) + 240-f, 8, ALLEGRO_ALIGN_RIGHT, " %i", on_screen_score);
	al_draw_textf(font18, BLACK, left_button_margin + (SCORE_BUTTON*LEFT_MARGIN) + (BUTTON_WIDTH*SCORE_BUTTON) + 240+f, 8, ALLEGRO_ALIGN_RIGHT, " %i", on_screen_score);
	al_draw_textf(font18, WHITE, left_button_margin + (SCORE_BUTTON*LEFT_MARGIN) + (BUTTON_WIDTH*SCORE_BUTTON) + 240, 8, ALLEGRO_ALIGN_RIGHT, " %i", on_screen_score);
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

			if (button[NEW_GAME_BUTTON].flags)		{ al_play_sample_instance(instanceClick); game_state = REFRESH_GAME; }
			if (button[HIGH_SCORES_BUTTON].flags)		{ al_play_sample_instance(instanceClick); game_state = HIGH_SCORE; }
			if (button[OPTIONS_BUTTON].flags)			{ al_play_sample_instance(instanceClick); game_state = OPTIONS; }
			if (button[GAME_AREA_BUTTON].flags && !destroy_brick)		{ al_play_sample_instance(instanceClick2); selectBrick(mouseX, mouseY); }
		}
		if (game_state == OPTIONS) //if in options check these events
		{
		//	if (button[OPTIONS_SMALL_BUTTON].flags)		{ al_play_sample_instance(instanceClick); changeBricksXY(BRICKS_SMALL_X, BRICKS_SMALL_Y); game_state = REFRESH_GAME; }
		//	if (button[OPTIONS_LARGE_BUTTON].flags)		{ al_play_sample_instance(instanceClick); changeBricksXY(BRICKS_LARGE_X, BRICKS_LARGE_Y); game_state = REFRESH_GAME; }
			if (button[OPTIONS_CAMPAIGN_BUTTON].flags)	{ al_play_sample_instance(instanceClick); }
//			if (button[OPTIONS_24_BUTTON].flags)			{ al_play_sample_instance(instanceClick); changeBrickSize(BRICKS_SMALL); }
//			if (button[OPTIONS_36_BUTTON].flags)			{ al_play_sample_instance(instanceClick); changeBrickSize(BRICKS_MEDIUM); }
//			if (button[OPTIONS_48_BUTTON].flags)			{ al_play_sample_instance(instanceClick); changeBrickSize(BRICKS_LARGE); }
		}
		if (game_state == HIGH_SCORE) //if in high scores check these events
		{
			if (button[HIGH_SCORES_RESET_BUTTON].flags)	{ al_play_sample_instance(instanceClick); resetHighScores(); }
			if (button[HIGH_SCORES_CLOSE_BUTTON].flags)	{ al_play_sample_instance(instanceClick); game_state = PLAY_GAME; 	button[HIGH_SCORES_CLOSE_BUTTON].flags = false; }
		}
		if (game_state == END_GAME) //if game ended
		{
			if (button[END_GAME_NEW_GAME_BUTTON].flags) { al_play_sample_instance(instanceClick); game_state = REFRESH_GAME; }
			if (button[END_GAME_SAVE_SCORE_BUTTON].flags && checkSaveScores()) { al_play_sample_instance(instanceClick); game_state = SAVING_SCORE; }
		}
		if (game_state == SAVING_SCORE) //if game ended
		{
			if (button[END_GAME_NEW_GAME_BUTTON].flags) { al_play_sample_instance(instanceClick); game_state = REFRESH_GAME; }
		//	if (button[END_GAME_SAVE_SCORE_BUTTON].flags && !saved_scores) {  saveScores(); }
		}
	}

	if (mouseButton == 2) //if right mouse button pressed
	{
		if (game_state == PLAY_GAME) //if playing game check these clicks
		{
			if (button[NEW_GAME_BUTTON].flags)		{ game_state = CHEAT; }
			if (button[GAME_AREA_BUTTON].flags)		
			{
				changeTile((mouseX - left_game_area_margin ) / brick_size, (mouseY - TOP_MARGIN) / brick_size, 0);
			} //debug		
		}
		if (game_state == OPTIONS)		{ game_state = PLAY_GAME; }
		if (game_state == HIGH_SCORE)	{ game_state = PLAY_GAME; }
	}
}
void cGame::updatePositions()
{
		bricks_on_screen = BRICKS_MAP_X * BRICKS_MAP_Y;
		area_width = brick_size * BRICKS_MAP_X;
		area_height = brick_size * BRICKS_MAP_Y;
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
	for (int i = 0; i < MAX_HIGH_SCORE; i++)
	{

		high_score[i] = 0;
		al_ustr_free(high_score_name[i]);
		high_score_name[i] = al_ustr_new(".....");
	}
	saveHighScore();
}
//void cGame::changeBricksXY(int _x, int _y)//changes map size
//{
//	BRICKS_LARGE_X =_x;
//	BRICKS_LARGE_Y = _y;
//	update_position = true;	
//}
void cGame::changeBrickSize(int x)//changes brick size
{
	brick_size = x;
	update_position = true;
}
void cGame::updateNumberOfSelected() //checks and update number of selected bricks
{
	/*number_of_selected = 0;
	for (int i = 0; i<BRICKS_MAP_Y; i++)
		for (int t = 0; t < BRICKS_MAP_X; t++)
		{
			if (bricks[t][i].state == SELECTED || bricks[t][i].state == EXPLODING)
			{
				number_of_selected++;
			}
		}*/
	if (currently_selected < selectionList.size())
	{
		int x = selectionList[currently_selected].x;
		int y = selectionList[currently_selected].y;
		bricks[x][y].state = SELECTED;
		currently_selected++;
	}

}
void cGame::changeTile(int x, int y, int color) //changes color of x,y tile
{
	bricks[x][y].color = color;
}
void cGame::drawGameArea() // draw all bricks on screen;
{
	int  x_times = screen_width / al_get_bitmap_width(backgroundBMP) + 1;
	int  y_times = screen_height / al_get_bitmap_height(backgroundBMP) + 1;
	for (int i = 0; i < x_times; i++)
		for (int t = 0; t < y_times; t++)	al_draw_bitmap(backgroundBMP, i * 128, t * 128, NULL);

	left_game_area_margin = (screen_width - area_width) / 2;
	al_draw_tinted_bitmap_region(shadowBMP, TINT3, 0, 0, button[GAME_AREA_BUTTON].width, button[GAME_AREA_BUTTON].height, left_game_area_margin, TOP_MARGIN, NULL);
	al_draw_rectangle(left_game_area_margin, TOP_MARGIN, button[GAME_AREA_BUTTON].width + left_game_area_margin + 1, button[GAME_AREA_BUTTON].height + TOP_MARGIN + 1, BLACK, 1);
	for (int i = 0; i < BRICKS_MAP_Y; i++)
		for (int t = 0; t < BRICKS_MAP_X; t++)
		{
			//BAD CODE needs fixing
			
			if (bricks[t][i].state == FULL || bricks[t][i].state == SELECTED)
			{
				al_draw_bitmap_region(bricksBMP,bricks[t][i].color*brick_size, 0, brick_size, brick_size, t*brick_size + left_game_area_margin, i*brick_size + TOP_MARGIN + screen_shake, NULL);
			}
			
			if (bricks[t][i].state == SELECTED)
			{
				al_draw_bitmap_region(bricksBMP, 0, brick_size, brick_size, brick_size, t*brick_size + left_game_area_margin, i*brick_size + TOP_MARGIN + screen_shake, NULL); //needs fixing
			}	
		}
	for (int i = 0; i<BRICKS_MAP_Y; i++)
		for (int t = 0; t < BRICKS_MAP_X; t++)
		{

			al_draw_textf(font18, BLACK, t*brick_size + left_game_area_margin, i*brick_size + TOP_MARGIN + screen_shake, NULL, "%i,%i", t,i);
			if (bricks[t][i].state == EXPLODING)
			{
				al_draw_bitmap_region(explosionBMP, 240 * curFrame, 0, 240, 240, (t - 2)*brick_size + left_game_area_margin, (i - 2)*brick_size + TOP_MARGIN, NULL);
			}
		}
	
	if (destroy_brick)
	{
		int f = 2;
		//al_play_sample(explosionOGG, 1, 0, 1.3, ALLEGRO_PLAYMODE_ONCE, NULL);
		al_draw_textf(font36, BLACK, (last_clicked_x *brick_size) + left_game_area_margin + (brick_size / 2)-f, (last_clicked_y *brick_size) + TOP_MARGIN - (curFrame * 4)-f, NULL, "%i", last_score);
		al_draw_textf(font36, BLACK, (last_clicked_x *brick_size) + left_game_area_margin + (brick_size / 2) + f, (last_clicked_y *brick_size) + TOP_MARGIN - (curFrame * 4) + f, NULL, "%i", last_score);
		al_draw_textf(font36, WHITE, (last_clicked_x *brick_size) + left_game_area_margin + (brick_size / 2), (last_clicked_y *brick_size) + TOP_MARGIN - (curFrame * 4), NULL, "%i", last_score);
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
	for (i = 0; i<BRICKS_MAP_Y; i++)
		for (t = 0; t < BRICKS_MAP_X; t++)
		{
			bricks[t][i].color =  rand() % BRICK_COLORS;  
			if (debug) bricks[t][i].color=1;//DEBUG
			bricks[t][i].state = FULL;
		}
	
	for (i = 0; i < NUMBER_OF_BUTTONS; i++)
	{
		button[i].flags = false;
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
	
		for (x = BRICKS_MAP_X - 1; x >= 0; x--)
			for (y = BRICKS_MAP_Y - 1; y >= 0; y--)
			{
				if (bricks[x][y].state!=EMPTY)
				{

					if (x > 0)//avoids going outside of vector
					{
					if (bricks[x][y].color == bricks[x - 1][y].color && bricks[x - 1][y].state != EMPTY)		{ selected++; }
					}
					if (x + 1 < BRICKS_MAP_X) //avoids going outside of vector
					{
					if (bricks[x][y].color == bricks[x + 1][y].color && bricks[x + 1][y].state != EMPTY)		{ selected++; }
					}
					if (y > 0) //avoids going outside of vector
					{
					if (bricks[x][y].color == bricks[x][y - 1].color && bricks[x][y - 1].state != EMPTY) 	{ selected++; }
					}
					if (y + 1 < BRICKS_MAP_Y) //avoids going outside of vector
					{
					if (bricks[x][y].color == bricks[x][y + 1].color && bricks[x][y + 1].state != EMPTY)		{ selected++; }
					}
				}

			}
		if (selected == 0 && !destroy_brick) game_state = END_GAME;
		
}
void cGame::highScores() // open high scores;
{
	al_draw_tinted_bitmap(shadowBMP, TINT, 0, 0, 0);
	al_draw_bitmap(scoreBMP, screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2), (screen_height / 2) - (al_get_bitmap_height(scoreBMP) / 2), 0);

	button[HIGH_SCORES_RESET_BUTTON].drawButton();
	button[HIGH_SCORES_CLOSE_BUTTON].drawButton();
	for (int i = 0; i < MAX_HIGH_SCORE; i++)
	{
		al_draw_ustr(font24, WHITE, screen_width / 2 - 300, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + (25 * i) + 6, ALLEGRO_ALIGN_LEFT, high_score_name[i]);
		al_draw_textf(font24, WHITE, screen_width / 2 + 250, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + (25 * i) + 6, ALLEGRO_ALIGN_CENTRE, "%i ", high_score[i]);
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
		int f = 26;
		al_draw_ustr(font24, WHITE, screen_width / 2 - 300, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + f * i , ALLEGRO_ALIGN_LEFT, high_score_name[i]);
		al_draw_textf(font24, WHITE, screen_width / 2 + 250, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + f * i , ALLEGRO_ALIGN_CENTRE, "%i ", high_score[i]);
	}
	al_draw_textf(font24, WHITE, screen_width / 2, screen_height / 2 + 75, ALLEGRO_ALIGN_CENTRE, "you scored %i points !", on_screen_score);
}
void cGame::selectBrick(int _mouse_x, int _mouse_y) // takes mouse input and selects all same color bricks that are neighboruing to  bricks[x][y]
{
	int x = (_mouse_x - left_game_area_margin) / brick_size;
	int y = (_mouse_y - TOP_MARGIN) / brick_size;
	last_clicked_x = x;
	last_clicked_y = y;
	int refresh = 0; 
	if (!selection && bricks[x][y].state == FULL) //if not selected and full
	{
		selectionList.clear();
		cList currentList(x,y,false);
		selectionList.push_back(currentList);

		for (int i = 0; i < selectionList.size(); i++)
		{
			int x = selectionList[i].x;
			int y = selectionList[i].y;

			if (y - 1 >= 0)	if (bricks[x][y].color == bricks[x][y - 1].color &&	!bricks[x][y - 1].select && bricks[x][y - 1].state != EMPTY) //if north brick is same color
			{
				currentList.set(x, y - 1, false);
				bricks[x][y - 1].select = true;
				selectionList.push_back(currentList);
				bricks[x][y].select = true;
			}

			if (y + 1 < BRICKS_MAP_Y)if (bricks[x][y].color == bricks[x][y + 1].color && !bricks[x][y + 1].select && bricks[x][y + 1].state != EMPTY) //if south brick is same color
			{
				currentList.set(x, y + 1, false);
				bricks[x][y + 1].select = true;
				selectionList.push_back(currentList);
				bricks[x][y].select = true;
			}

			if (x - 1 >= 0)	if (bricks[x][y].color == bricks[x - 1][y].color && !bricks[x - 1][y].select && bricks[x - 1][y].state != EMPTY) //if west brick is same color
			{
				currentList.set(x - 1, y, false);
				bricks[x - 1][y].select = true;
				selectionList.push_back(currentList);
				bricks[x][y].select = true;
			}

			if (x + 1 < BRICKS_MAP_X)	if (bricks[x][y].color == bricks[x + 1][y].color && !bricks[x + 1][y].select && bricks[x + 1][y].state != EMPTY) //if east brick is same color
			{
				currentList.set(x + 1, y, false);
				bricks[x + 1][y].select = true;
				selectionList.push_back(currentList);
				bricks[x][y].select = true;
			}
			selectionList[i].state = true;
		}
		if (selectionList.size() >=2)		{	selection = true;	}
		else		{	selectionList.clear();		}
		
		currently_selected = 0;
	}else 
	if (selection && bricks[x][y].state == SELECTED) // else if selected
	{
		destroy_brick = true;
		selectionList.clear();
		for (int i = 0; i < BRICKS_MAP_X; i++)
			for (int t = 0; t < BRICKS_MAP_Y; t++)
			{
				if (bricks[i][t].state == SELECTED) bricks[i][t].state = EXPLODING;//change selected to explosion status
			}
		
	}else 
	if (selection && bricks[x][y].state != SELECTED)
	{
		deselectBrick();	
		selectBrick(_mouse_x, _mouse_y);
	}

}

void cGame::deselectBrick() // clears selection of bricks 
{
	int i = 0;
	int t = 0;
	for (i = 0; i<BRICKS_MAP_Y; i++)
		for (t = 0; t < BRICKS_MAP_X; t++)
		{
			if (bricks[t][i].state == SELECTED)
			{ 
				bricks[t][i].state = FULL;
				
			}
			bricks[t][i].select = false;
		}
	selection = false;
	selectionList.clear();
}
void cGame::destroyBrick() // after clicking selected bricks destroys them
{
	float explosion_volume =0.3+ 0.05 * number_of_selected; //more bricks, louder explosion
	if (explosion_volume>1.5) explosion_volume = 1.5; // up to 150% volume
	al_set_sample_instance_gain(instance, explosion_volume);
	al_play_sample_instance(instance);
	screen_shake = 0;
	bool start_destroy = false;
	last_score = calculateScore();
	if (++frameCount >= frameDelay)
	{
		if (++curFrame >= maxFrame)
		{
			start_destroy = true;
			curFrame = 0;
		}
		frameCount = 0;
		screen_shake = (rand() % 6)-3; // -3 to 3 pixels in Y direction
	}

	if (start_destroy)
	{
		for (int i = 0; i < BRICKS_MAP_Y; i++)
			for (int t = 0; t < BRICKS_MAP_X; t++)
			{
				bricks[t][i].select = false;
				if (bricks[t][i].state == EXPLODING)
				{
					bricks[t][i].state = EMPTY;
				}
			}
		score+=calculateScore();
		dropBrick();
		selection = false;
		destroy_brick = false;
		screen_shake = 0;
	}
}
int  cGame::calculateScore() //calculates score for destroyed bricks
{
	int score_earned = 0;
	for (int i = number_of_selected; i>=0; i--)
	{
		score_earned+= i*2; //NEEDS EVALUATION
	}
	return score_earned;
}
void cGame::dropBrick() //after destroying bricks fill holes by dropping them (checks from bottom)
{
	int refresh = 0;
	bool check_column = true;
	for (refresh = BRICKS_MAP_Y -1; refresh > 0; refresh--) // do this BRICKS_LARGE_Y times to drop bricks if there are bigger gaps 
	{
		for (int xx = BRICKS_MAP_X-1; xx >= 0; xx--)//scans for empty bricks and drops them by one
		for (int yy = BRICKS_MAP_Y-1; yy >0; yy--)
		{
			if (bricks[xx][yy].state == EMPTY)
			{
				bricks[xx][yy].color = bricks[xx][yy-1].color;
				bricks[xx][yy].state = bricks[xx][yy-1].state;
				bricks[xx][yy-1].state = EMPTY;
			}
		}
	}
	moveBrickLeft();
}
void cGame::moveBrickLeft()
{
	
	for (int x = 0; x < BRICKS_MAP_X; x++)
	{
		int empty = 0;
		for (int y = 0; y < BRICKS_MAP_Y; y++)
		{
			if (bricks[x][y].state == EMPTY)
			{
				empty++;
				//TODO
			}
			if (empty == BRICKS_MAP_Y)
			{
					for (int xx = x; xx < BRICKS_MAP_X; xx++)
					for (int yy = 0; yy < BRICKS_MAP_Y; yy++)
					{
						if (xx + 1 < BRICKS_MAP_X) //avoids going outside of vector
						{
							bricks[xx][yy].color =bricks[xx + 1][yy].color;
							bricks[xx][yy].state = bricks[xx + 1][yy].state;
							bricks[xx + 1][yy].color = 0;
							bricks[xx + 1][yy].state = EMPTY;
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
				high_score_name[t] = high_score_name[t - 1];
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
saveHighScore();
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
void cGame::saveHighScore()
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
void cGame::loadHighScore()
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
void cGame::saveGame()
{
	if (game_state == PLAY_GAME)
	{
		ALLEGRO_FILE* save_game = al_fopen("save.bri", "wb");
		al_fwrite(save_game, &score, sizeof(int));
		//al_fwrite(save_game, &brick_size, sizeof(int));
		//al_fwrite(save_game, &BRICKS_MAP_X, sizeof(int));
		//al_fwrite(save_game, &BRICKS_MAP_Y, sizeof(int));

		for (int i = 0; i < BRICKS_MAP_X; i++)
			for (int t = 0; t < BRICKS_MAP_Y; t++)
			{
				int color = bricks[i][t].color;
				int state = bricks[i][t].state;
				al_fwrite(save_game, &color, sizeof(int));
				al_fwrite(save_game, &state, sizeof(int));
			}
		al_fclose(save_game);
	}
	update_position = true;
}
void cGame::loadGame()
{
	ALLEGRO_FILE* save_game = al_fopen("save.bri", "rb");
	if (al_fopen("save.bri", "rb"))
	{
		int score;
		al_fread(save_game, &score, sizeof(int));
		on_screen_score = score;
		//buffer = &brick_size;
		//al_fread(save_game, &brick_size, sizeof(int));
		//al_fread(save_game, &BRICKS_MAP_X, sizeof(int));
		//al_fread(save_game, &BRICKS_MAP_Y, sizeof(int));

		for (int i = 0; i < BRICKS_MAP_X; i++)
			for (int t = 0; t < BRICKS_MAP_Y; t++)
			{
				int color;
				int state;
				al_fread(save_game, &color, sizeof(int));
				bricks[i][t].color = color;
				al_fread(save_game, &state, sizeof(int));
				bricks[i][t].state = state;
			}
		
		//on_screen_score = score;
		
	}
	else
	{
		game_state = REFRESH_GAME;
	}
	al_fclose(save_game);
	update_position = true;
}

cList::cList()
{
	x = 0;
	y = 0;
	state = false;
}

cList::cList(int _x, int _y, bool _state)
{
	x = _x;
	y = _y;
	state = _state;
}

void cList::setState(bool _state)
{
	state = _state;
}

void cList::set(int _x, int _y, bool _state)
{
	x = _x;
	y = _y;
	state = _state;
}
