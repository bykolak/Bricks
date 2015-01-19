
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
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_FONT *arial24 = NULL;
ALLEGRO_FONT *arial14 = NULL;

int main(int argc, char **argv)
{
	//main loop bools
	bool done = false;
	bool render = true;

	int FPS = 60;
	int mx = 0; //debug
	int my = 0;//debug

	srand(time(NULL));

	//======INIT
	cGame game;
	game.newGame();
	game.loadButton();
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	arial24 = al_load_font("arial.ttf", 24, 0);
	arial14 = al_load_font("arial.ttf", 14, 0);
	//game.changeScore(100000);
	int i = 0;
	for (i = 0; i < NUMBER_OF_BUTTONS; i++)
	{

		game.button[i].createButton(game.buttonsBMP);
	}
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

				mx = (ev.mouse.x - LEFT_MARGIN) / game.getBrickSize();//debug
				my = (ev.mouse.y - TOP_MARGIN) / game.getBrickSize();//debug

				//if inside of button then change button[].flag to true value else false
				game.button[NEW_GAME_BUTTON].overButton(ev.mouse.x, ev.mouse.y);
				game.button[HIGH_SCORES_BUTTON].overButton(ev.mouse.x, ev.mouse.y);
				game.button[OPTIONS_BUTTON].overButton(ev.mouse.x, ev.mouse.y);
				game.button[GAME_AREA_BUTTON].overButton(ev.mouse.x, ev.mouse.y);

			}
			if (game.getGameState() == OPTIONS) //if in options check these events
			{

				game.button[OPTIONS_SMALL_BUTTON].overButton(ev.mouse.x, ev.mouse.y);
				game.button[OPTIONS_LARGE_BUTTON].overButton(ev.mouse.x, ev.mouse.y);
				game.button[OPTIONS_CAMPAIGN_BUTTON].overButton(ev.mouse.x, ev.mouse.y);
				game.button[OPTIONS_24_BUTTON].overButton(ev.mouse.x, ev.mouse.y);
				game.button[OPTIONS_36_BUTTON].overButton(ev.mouse.x, ev.mouse.y);
				game.button[OPTIONS_48_BUTTON].overButton(ev.mouse.x, ev.mouse.y);

			}
			if (game.getGameState() == HIGH_SCORE) //if in high scores check these events
			{
				game.button[HIGH_SCORES_RESET_BUTTON].overButton(ev.mouse.x, ev.mouse.y);
				game.button[HIGH_SCORES_CLOSE_BUTTON].overButton(ev.mouse.x, ev.mouse.y);

			}
			if (game.getGameState() == END_GAME) //if game ended check these events
			{
				game.button[END_GAME_NEW_GAME_BUTTON].overButton(ev.mouse.x, ev.mouse.y);
				game.button[END_GAME_SAVE_SCORE_BUTTON].overButton(ev.mouse.x, ev.mouse.y);


			}
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) //checks if mouse button pressed
		{


			if (ev.mouse.button & 1) //if left mouse button pressed
			{

				if (game.getGameState() == PLAY_GAME) //if playing game check these clicks
				{

					if (game.button[NEW_GAME_BUTTON].getFlags())		{ game.changeGameState(REFRESH_GAME); }
					if (game.button[HIGH_SCORES_BUTTON].getFlags())		{ game.highScores(); }
					if (game.button[OPTIONS_BUTTON].getFlags())			{ game.options(); }
					if (game.button[GAME_AREA_BUTTON].getFlags())		{ game.selectBrick(ev.mouse.x, ev.mouse.y); }
				}
				if (game.getGameState() == OPTIONS) //if in options check these events
				{
					if (game.button[OPTIONS_SMALL_BUTTON].getFlags())		{ game.changeBricksXY(BRICKS_SMALL_X, BRICKS_SMALL_Y); game.changeGameState(REFRESH_GAME); }
					if (game.button[OPTIONS_LARGE_BUTTON].getFlags())		{ game.changeBricksXY(BRICKS_LARGE_X, BRICKS_LARGE_Y); game.changeGameState(REFRESH_GAME); }
					if (game.button[OPTIONS_CAMPAIGN_BUTTON].getFlags())	{}
					if (game.button[OPTIONS_24_BUTTON].getFlags())			{ game.changeBrickSize(BRICKS_SMALL); }
					if (game.button[OPTIONS_36_BUTTON].getFlags())			{ game.changeBrickSize(BRICKS_MEDIUM); }
					if (game.button[OPTIONS_48_BUTTON].getFlags())			{ game.changeBrickSize(BRICKS_LARGE); }
				}
				if (game.getGameState() == HIGH_SCORE) //if in high scores check these events
				{
					if (game.button[HIGH_SCORES_RESET_BUTTON].getFlags())	{ game.resetHighScores(); }
					if (game.button[HIGH_SCORES_CLOSE_BUTTON].getFlags())	{ game.changeGameState(PLAY_GAME); 	game.button[HIGH_SCORES_CLOSE_BUTTON].changeFlags(false); }
				}
				if (game.getGameState() == END_GAME) //if game ended
				{
					if (game.button[END_GAME_NEW_GAME_BUTTON].getFlags()) { game.changeGameState(REFRESH_GAME); }
					if (game.button[END_GAME_SAVE_SCORE_BUTTON].getFlags() && game.checkSaveScores()) { game.changeGameState(SAVING_SCORE); }
				}
				if (game.getGameState() == SAVING_SCORE) //if game ended
				{
					if (game.button[END_GAME_NEW_GAME_BUTTON].getFlags()) { game.changeGameState(REFRESH_GAME); }
					if (game.button[END_GAME_SAVE_SCORE_BUTTON].getFlags()) { game.changeGameState(SAVING_SCORE); }
				}

			}

			if (ev.mouse.button & 2) //if right mouse button pressed
			{
				if (game.getGameState() == PLAY_GAME) //if playing game check these clicks
				{
					if (game.button[GAME_AREA_BUTTON].getFlags())		{ game.changeTile(mx, my, 3); } //debug		
				}
				if (game.getGameState() == OPTIONS) //if in options check these clicks
				{
					game.changeGameState(PLAY_GAME);
				}
				if (game.getGameState() == HIGH_SCORE) //if in high scores check these clicks
				{
					game.changeGameState(PLAY_GAME);
				}
				if (game.getGameState() == END_GAME) //if game ended check these clicks
				{
					//	game.changeGameState(REFRESH_GAME);
				}
				if (game.getGameState() == SAVING_SCORE) //if game ended check these clicks
				{


					//	game.changeGameState(REFRESH_GAME);
				}
			}

		}
		
		else if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
		{
			if (game.getGameState() == SAVING_SCORE)
			{
				game.enterPlayerName(ev.keyboard.keycode,ev.keyboard.unichar);
				
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			// 60 times per second
			if (game.getGameState() == REFRESH_GAME)	{ game.newGame(); } //if "new game" or "map size" button pressed
			if (game.getGameState() == PLAY_GAME)	{ game.checkEndGame(); }
			game.updateNumberOfSelected();
			render = true;
		}

		//=========RENDERER
		if (render && al_is_event_queue_empty(event_queue))
		{
			game.drawGameArea();
			game.button[NEW_GAME_BUTTON].drawButton();
			game.button[HIGH_SCORES_BUTTON].drawButton();
			game.button[OPTIONS_BUTTON].drawButton();
			game.button[SCORE_BUTTON].drawButton();
			


			al_draw_textf(arial24, WHITE, DEBUG_X + 240, DEBUG_Y + 2, ALLEGRO_ALIGN_RIGHT, " %i", game.getScore());

			//al_draw_textf(arial14, WHITE, DEBUG_X + 650, DEBUG_Y, ALLEGRO_ALIGN_LEFT, "gamestate %i", game.getGameState());//debug
			//if (game.button[GAME_AREA_BUTTON].getFlags())//debug
			//{
			//	al_draw_textf(arial14, WHITE, DEBUG_X + 250, DEBUG_Y, ALLEGRO_ALIGN_LEFT, "mouseX %i", mx);//debug
			//	al_draw_textf(arial14, WHITE, DEBUG_X + 350, DEBUG_Y, ALLEGRO_ALIGN_LEFT, "mouseY %i", my);//debug
			//	al_draw_textf(arial14, WHITE, DEBUG_X + 450, DEBUG_Y, ALLEGRO_ALIGN_LEFT, " Selected:%i", game.getNumberOfSelected());//debug
			//	al_draw_textf(arial14, WHITE, DEBUG_X + 550, DEBUG_Y, ALLEGRO_ALIGN_LEFT, "brick_size %i", game.getBrickSize());//debug

			//
			//}
			if (game.getGameState() == OPTIONS)
			{
				al_draw_tinted_bitmap(game.shadowBMP, TINT, 0, 0, 0);
				al_draw_bitmap(game.optionsBMP, OPTIONS_X, TOP_MARGIN, 0);
				game.button[OPTIONS_SMALL_BUTTON].drawButton();
				game.button[OPTIONS_LARGE_BUTTON].drawButton();
				game.button[OPTIONS_CAMPAIGN_BUTTON].drawButton();
				game.button[OPTIONS_24_BUTTON].drawButton();
				game.button[OPTIONS_36_BUTTON].drawButton();
				game.button[OPTIONS_48_BUTTON].drawButton();
			}
			if (game.getGameState() == HIGH_SCORE)
			{
				al_draw_tinted_bitmap(game.shadowBMP, TINT, 0, 0, 0);
				//al_draw_bitmap(game.high_scoresBMP, HIGH_SCORES_X, TOP_MARGIN, 0);
				al_draw_bitmap(game.endBMP, game.screen_width / 2 - (al_get_bitmap_width(game.endBMP) / 2), game.screen_height / 2 - (al_get_bitmap_height(game.endBMP) / 2), 0);
				game.button[HIGH_SCORES_RESET_BUTTON].drawButton();
				game.button[HIGH_SCORES_CLOSE_BUTTON].drawButton();
				for (i = 0; i < MAX_HIGH_SCORE; i++)
				{
					al_draw_ustr(arial24, WHITE, game.screen_width / 2 - 300, game.screen_height / 2 - (al_get_bitmap_height(game.endBMP) / 2) + 25 * i + 6, ALLEGRO_ALIGN_LEFT, game.high_score_name[i]);


					al_draw_textf(arial24, WHITE, game.screen_width / 2 + 250, game.screen_height / 2 - (al_get_bitmap_height(game.endBMP) / 2) + 25 * i + 6, ALLEGRO_ALIGN_CENTRE, "%i ", game.high_score[i]);
				}

			}
			if (game.getGameState() == END_GAME|| game.getGameState()==SAVING_SCORE)
			{
				al_draw_tinted_bitmap(game.shadowBMP, TINT, 0, 0, 0);
				al_draw_bitmap(game.endBMP, game.screen_width / 2 - (al_get_bitmap_width(game.endBMP) / 2), game.screen_height / 2 - (al_get_bitmap_height(game.endBMP) / 2), 0);
				game.button[END_GAME_NEW_GAME_BUTTON].drawButton();
				if (game.checkSaveScores()) game.button[END_GAME_SAVE_SCORE_BUTTON].drawButton();
				for (i = 0; i < MAX_HIGH_SCORE; i++)
				{
				al_draw_ustr(arial24, WHITE, game.screen_width / 2 - 300, game.screen_height / 2 - (al_get_bitmap_height(game.endBMP) / 2) + 25 * i + 6, ALLEGRO_ALIGN_LEFT, game.high_score_name[i]);
				
				
					al_draw_textf(arial24, WHITE, game.screen_width / 2+250, game.screen_height / 2 - (al_get_bitmap_height(game.endBMP) / 2) + 25 * i+6, ALLEGRO_ALIGN_CENTRE, "%i ", game.high_score[i]);
				}
				
				al_draw_textf(arial24, WHITE,game.screen_width / 2, game.screen_height / 2 +75 , ALLEGRO_ALIGN_CENTRE, "you scored %i points !", game.getScore());
			}
			if (game.getGameState() == SAVING_SCORE)
			{
				al_draw_textf(arial24, WHITE, game.screen_width / 2-100, game.screen_height / 2 + 100, ALLEGRO_ALIGN_CENTRE, "Enter your name:");
				al_draw_ustr(arial24, RED, game.screen_width / 2, game.screen_height / 2 + 100, NULL, game.edited_text);
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(115, 115, 115));
			render = false;
		}


		
		
	}
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}