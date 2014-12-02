
#include <stdlib.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include "classes.h"
#include "define.h"



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
	int FPS = 60;
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
	display = al_create_display(width, height);
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
	backgroundBMP = al_load_bitmap("background.bmp");
	bricksBMP = al_load_bitmap("bricks.bmp");

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
			if (button(ev.mouse.x, ev.mouse.y, NEW_GAME_X, NEW_GAME_Y, NEW_GAME_WIDTH, NEW_GAME_HEIGHT)) //checks if mouse on new game button
			{
				over_new_game = true;
			}
			else over_new_game = false;

			if (button(ev.mouse.x, ev.mouse.y, HIGH_SCORES_X, HIGH_SCORES_Y, HIGH_SCORES_WIDTH, HIGH_SCORES_HEIGHT)) //checks if mouse on high scores button
			{
				over_high_scores = true;
			}
			else over_high_scores = false;

			if (button(ev.mouse.x, ev.mouse.y, OPTIONS_X, OPTIONS_Y, OPTIONS_WIDTH, OPTIONS_HEIGHT)) //checks if mouse on high scores button
			{
				over_options = true;
			}
			else over_options = false;

			if (button(ev.mouse.x, ev.mouse.y, GAME_AREA_X, GAME_AREA_Y, GAME_AREA_WIDTH, GAME_AREA_HEIGHT)) //checks if mouse inside of game area
			{
				over_game_area = true;
			}
			else over_game_area = false;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) //checks if mouse button pressed
		{
			if (ev.mouse.button & 1)
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
				al_draw_rounded_rectangle(NEW_GAME_X, NEW_GAME_Y, NEW_GAME_X + NEW_GAME_WIDTH, NEW_GAME_HEIGHT,15,15, WHITE, 3);
			}
			if (over_high_scores)
			{
				al_draw_rounded_rectangle(HIGH_SCORES_X, HIGH_SCORES_Y, HIGH_SCORES_X + HIGH_SCORES_WIDTH, HIGH_SCORES_HEIGHT, 15, 15, WHITE, 3);
			}
			if (over_options)
			{
				al_draw_rounded_rectangle(OPTIONS_X, OPTIONS_Y, OPTIONS_X + OPTIONS_WIDTH, OPTIONS_HEIGHT, 15, 15, WHITE, 3);
			}
			
			al_draw_textf(arial24, WHITE, SCORE_X, SCORE_Y, ALLEGRO_ALIGN_LEFT , " %i", game.getScore());
			al_draw_textf(arial24, WHITE, SCORE_X-100, SCORE_Y, ALLEGRO_ALIGN_LEFT, " %i", game.getNumberOfSelected());
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
			render = false;
		}


		
		
	}
//	
//  al_draw_text(arial24, al_map_rgb(255, 0, 0), 100, 100, 0, "Hello World");
//  al_draw_filled_rectangle(10, 10, 100, 100, RED);
	al_destroy_bitmap(backgroundBMP);
	al_destroy_bitmap(bricksBMP);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}