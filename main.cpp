
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include "classes.h"
#include "define.h"

//GLOBAL VARIABLES
int BRICK_COLORS = 6;
// dimensions of game area
int BRICKS_X = BRICKS_LARGE_X;  //number of Bricks in X axis
int BRICKS_Y = BRICKS_LARGE_Y;  //number of Bricks in Y axis;


bool button(int _mouse_x, int _mouse_y, int _x, int _y, int _width, int _height) //jesli w zakresie to zwroc true
{
	if (_mouse_x >= _x && _mouse_x <= _x + _width && _mouse_y >= _y && _mouse_y <= _y + _height) return true;
	else return false;
}


	

int main(int argc, char **argv)
{
	bool done = false;
	bool render = true;

	//flags for mouse
	bool over_new_game = false;
	bool over_high_scores = false;
	bool over_options = false;
	bool over_game_area = false;
	//flags for options 
	bool over_small = false;
	bool over_large = false;
	bool over_campaign = false;
	bool over_24 = false;
	bool over_36 = false;
	bool over_48 = false;
	bool over_high_score_reset = false;
	bool over_high_score_close = false;
	int FPS = 60;
	int mx = 0; //debug
	int my = 0;//debug
	srand(time(NULL));
	cGame game;
	game.newGame();
		
	//======INIT
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	

	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL, "Error Initializing Allegro", NULL, NULL);
		return -1;
	}
	display = al_create_display(WIDTH, HEIGHT);
	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL, "Error Initializing display", NULL, NULL);
		return -1;
	}
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	// LOADING GRAPHICS
	al_init_image_addon();
	ALLEGRO_BITMAP *backgroundBMP = NULL;
	ALLEGRO_BITMAP *bricksBMP = NULL;
	ALLEGRO_BITMAP *optionsBMP = NULL;
	ALLEGRO_BITMAP *shadowBMP = NULL;
	ALLEGRO_BITMAP *high_scoresBMP = NULL;
	shadowBMP = al_create_bitmap(WIDTH, HEIGHT);
	al_set_target_bitmap(shadowBMP);
	al_clear_to_color(BLACK);
	al_set_target_bitmap(al_get_backbuffer(display));
	backgroundBMP = al_load_bitmap("background.bmp");
	bricksBMP = al_load_bitmap("bricks.bmp");
	optionsBMP = al_load_bitmap("options.bmp");
	high_scoresBMP = al_load_bitmap("high_scores.bmp");

	ALLEGRO_FONT *arial24 = al_load_font("arial.ttf", 24, 0);
	//game.changeScore(1000000);
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	timer = al_create_timer(1.0 / FPS);
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	//===========MAIN LOOP
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//==========INPUT
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // checks for window Xed
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) //checks if mouse moved 
		{
			if (game.getGameState() == PLAY_GAME) //if playing game check these events
			{
				mx = (ev.mouse.x - LEFT_MARGIN) / BRICK_WIDTH;//debug
				my = (ev.mouse.y - TOP_MARGIN) / BRICK_HEIGHT;//debug
				if (button(ev.mouse.x, ev.mouse.y, NEW_GAME_X, NEW_GAME_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) //checks if mouse on new game button
				{
					over_new_game = true;
				}
				else over_new_game = false;

				if (button(ev.mouse.x, ev.mouse.y, HIGH_SCORES_X, HIGH_SCORES_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) //checks if mouse on high scores button
				{
					over_high_scores = true;
				}
				else over_high_scores = false;

				if (button(ev.mouse.x, ev.mouse.y, OPTIONS_X, OPTIONS_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) //checks if mouse on options button
				{
					over_options = true;
				}
				else over_options = false;

				if (button(ev.mouse.x, ev.mouse.y, LEFT_MARGIN, TOP_MARGIN, GAME_AREA_WIDTH, GAME_AREA_HEIGHT)) //checks if mouse inside of game area
				{
					over_game_area = true;
				}
				else over_game_area = false;
			}
			if (game.getGameState() == OPTIONS) //if in options check these events
			{
				if (button(ev.mouse.x, ev.mouse.y, OPTIONS_SMALL_X, OPTIONS_SMALL_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) //checks if mouse on small option button
				{
					over_small = true;
				}
				else over_small = false;

				if (button(ev.mouse.x, ev.mouse.y, OPTIONS_LARGE_X, OPTIONS_LARGE_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) //checks if mouse on large option button
				{
					over_large = true;
				}
				else over_large = false;

				if (button(ev.mouse.x, ev.mouse.y, OPTIONS_CAMPAIGN_X, OPTIONS_CAMPAIGN_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) //checks if mouse on campaign option button
				{
					over_campaign = true;
				}
				else over_campaign = false;

				if (button(ev.mouse.x, ev.mouse.y, OPTIONS_24_X, OPTIONS_24_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT)) //checks if mouse on large option button
				{
					over_24 = true;
				}
				else over_24 = false;

				if (button(ev.mouse.x, ev.mouse.y, OPTIONS_36_X, OPTIONS_36_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT)) //checks if mouse on large option button
				{
					over_36 = true;
				}
				else over_36 = false;

				if (button(ev.mouse.x, ev.mouse.y, OPTIONS_48_X, OPTIONS_48_Y, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT)) //checks if mouse on large option button
				{
					over_48 = true;
				}
				else over_48 = false;


			}
			if (game.getGameState() == HIGH_SCORE) //if in high scores check these events
			{
				if (button(ev.mouse.x, ev.mouse.y, HIGH_SCORE_RESET_X, HIGH_SCORE_RESET_Y, BUTTON_WIDTH, BUTTON_HEIGHT))
				{
					over_high_score_reset = true;
				}
				else over_high_score_reset = false;


				if (button(ev.mouse.x, ev.mouse.y, HIGH_SCORE_CLOSE_X, HIGH_SCORE_CLOSE_Y, BUTTON_WIDTH, BUTTON_HEIGHT))
				{
					over_high_score_close = true;
				}
				else over_high_score_close = false;
								
			}
			if (game.getGameState() == END_GAME)
			{

			}
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) //checks if mouse button pressed
		{
			
				if (game.getGameState() == PLAY_GAME) //if playing game check these events
				{
					if (ev.mouse.button & 1) //if first mouse button pressed
					{
						if (over_new_game)
						{
							game.changeGameState(REFRESH_GAME);
						}

						if (over_high_scores)
						{
							game.highScores();
						}
						if (over_options)
						{
							game.options();
						}
						if (over_game_area)
						{
							game.selectBrick(ev.mouse.x, ev.mouse.y);
						}

					}

					if (ev.mouse.button & 2) //if right mouse button pressed
					{
						if (over_game_area)
						{
							game.changeTile(mx, my, 3); //debug
						}
					}
			
				}

				if (game.getGameState() == OPTIONS) //if in options check these events
				{
					if (ev.mouse.button & 1) //if left mouse button pressed
					{
						if (over_small)
						{
							
						}
						if (over_large)
						{

						}
						if (over_campaign)
						{

						}
						if (over_24)
						{
							game.changeTileSize(24);
						//	al_resize_display(display,)
						}
						if (over_36)
						{
							game.changeTileSize(36);
						}
						if (over_48)
						{
							game.changeTileSize(48);
						}
					}
					if (ev.mouse.button & 2) //if second mouse button pressed
					{

						game.changeGameState(PLAY_GAME);
					}

				}

				if (game.getGameState() == HIGH_SCORE) //if in high scores check these events
				{
					if (ev.mouse.button & 1) //if first mouse button pressed
					{
						if (over_high_score_reset)
						{
							game.resetHighScores();
						}
						if (over_high_score_close)
						{
							
							game.changeGameState(PLAY_GAME);
							over_high_score_close = false;
						}
					}
					if (ev.mouse.button & 2) //if second mouse button pressed
					{
						
						game.changeGameState(PLAY_GAME);
					}
				}
				if (game.getGameState() == END_GAME) //if game ended
				{

				}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			// 60 times per second
			if (game.getGameState() == REFRESH_GAME) //if "new game" button pressed
			{
				game.newGame();
			}
			game.updateNumberOfSelected();
			render = true;
		}

	
		//=========RENDERER
		if (render && al_is_event_queue_empty(event_queue))
		{
			
			al_draw_bitmap(backgroundBMP, 0, 0, 0);
			game.drawGameArea(bricksBMP);
			if (over_new_game)
			{
				al_draw_rounded_rectangle(NEW_GAME_X, NEW_GAME_Y, NEW_GAME_X + BUTTON_WIDTH, BUTTON_HEIGHT,15,15, WHITE, 3);
			}
			if (over_high_scores)
			{
				al_draw_rounded_rectangle(HIGH_SCORES_X, HIGH_SCORES_Y, HIGH_SCORES_X + BUTTON_WIDTH, BUTTON_HEIGHT, 15, 15, WHITE, 3);
			}
			if (over_options)
			{
				al_draw_rounded_rectangle(OPTIONS_X, OPTIONS_Y, OPTIONS_X + BUTTON_WIDTH, BUTTON_HEIGHT, 15, 15, WHITE, 3);
			}
			
			al_draw_textf(arial24, WHITE, SCORE_X, SCORE_Y, ALLEGRO_ALIGN_LEFT , " %i", game.getScore());
			al_draw_textf(arial24, WHITE, SCORE_X-250, SCORE_Y+60, ALLEGRO_ALIGN_LEFT, " Selected:%i", game.getNumberOfSelected());
			
			if (over_game_area)//debug
			{
				al_draw_textf(arial24, WHITE, SCORE_X - 250, SCORE_Y, ALLEGRO_ALIGN_LEFT, "mouseX %i", mx);//debug
				al_draw_textf(arial24, WHITE, SCORE_X - 250, SCORE_Y + 30, ALLEGRO_ALIGN_LEFT, "mouseY %i", my);//debug
				al_draw_textf(arial24, WHITE, SCORE_X - 250, SCORE_Y + 90, ALLEGRO_ALIGN_LEFT, "tile_size %i", game.getTileSize());//debug
			}
			if (game.getGameState() == OPTIONS)
			{
				al_draw_tinted_bitmap(shadowBMP, TINT, 0, 0, 0);
				al_draw_bitmap(optionsBMP, OPTIONS_X, TOP_MARGIN, 0);
				if (over_small)
				{
					
					al_draw_rounded_rectangle(OPTIONS_SMALL_X, OPTIONS_SMALL_Y, OPTIONS_SMALL_X + BUTTON_WIDTH, OPTIONS_SMALL_Y + BUTTON_HEIGHT, 15, 15, WHITE, 3);
				}
				if (over_large)
				{

					al_draw_rounded_rectangle(OPTIONS_LARGE_X, OPTIONS_LARGE_Y, OPTIONS_LARGE_X + BUTTON_WIDTH, OPTIONS_LARGE_Y + BUTTON_HEIGHT, 15, 15, WHITE, 3);
				}
				if (over_campaign)
				{

					al_draw_rounded_rectangle(OPTIONS_CAMPAIGN_X, OPTIONS_CAMPAIGN_Y, OPTIONS_CAMPAIGN_X + BUTTON_WIDTH, OPTIONS_CAMPAIGN_Y + BUTTON_HEIGHT, 15, 15, WHITE, 3);
				}
				if (over_24)
				{

					al_draw_rounded_rectangle(OPTIONS_24_X, OPTIONS_24_Y, OPTIONS_24_X + SMALL_BUTTON_WIDTH, OPTIONS_24_Y + SMALL_BUTTON_HEIGHT, 15, 15, WHITE, 3);
				}

				if (over_36)
				{

					al_draw_rounded_rectangle(OPTIONS_36_X, OPTIONS_36_Y, OPTIONS_36_X + SMALL_BUTTON_WIDTH, OPTIONS_36_Y + SMALL_BUTTON_HEIGHT, 15, 15, WHITE, 3);
				}
				if (over_48)
				{

					al_draw_rounded_rectangle(OPTIONS_48_X, OPTIONS_48_Y, OPTIONS_48_X + SMALL_BUTTON_WIDTH, OPTIONS_48_Y + SMALL_BUTTON_HEIGHT, 15, 15, WHITE, 3);
				}
			}
			if (game.getGameState() == HIGH_SCORE)
			{
				al_draw_tinted_bitmap(shadowBMP, TINT, 0, 0, 0);
				al_draw_bitmap(high_scoresBMP, HIGH_SCORES_X, TOP_MARGIN, 0);
				if (over_high_score_reset)
				{
					al_draw_rounded_rectangle(HIGH_SCORE_RESET_X, HIGH_SCORE_RESET_Y, HIGH_SCORE_RESET_X + BUTTON_WIDTH, HIGH_SCORE_RESET_Y + BUTTON_HEIGHT, 15, 15, WHITE, 3);
				}
				if (over_high_score_close)
				{
					al_draw_rounded_rectangle(HIGH_SCORE_CLOSE_X, HIGH_SCORE_CLOSE_Y, HIGH_SCORE_CLOSE_X + BUTTON_WIDTH, HIGH_SCORE_CLOSE_Y + BUTTON_HEIGHT, 15, 15, WHITE, 3);
				}
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(115, 115, 115));
			render = false;
		}


		
		
	}
//	
//  al_draw_text(arial24, al_map_rgb(255, 0, 0), 100, 100, 0, "Hello World");
//  al_draw_filled_rectangle(10, 10, 100, 100, RED);
	al_destroy_bitmap(backgroundBMP);
	al_destroy_bitmap(bricksBMP);
	al_destroy_bitmap(optionsBMP);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}