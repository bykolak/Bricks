#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include "classes.h"
#include "define.h"

//GLOBAL VARIABLES
int BRICK_COLORS = 6;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_FONT *font18 = NULL;
ALLEGRO_FONT *font24 = NULL;
ALLEGRO_FONT *font36 = NULL;

int main(int argc, char **argv)
{
	//main loop bools
	bool done = false;
	bool render = true;
	//int cur = 0; //debug
	int FPS = 60;
		srand(time(NULL));

	//======INIT
	cGame game;
	
	font24 = al_load_font("luculent.ttf", 24, 0);
	font18 = al_load_font("luculent.ttf", 12, 0);
	font36 = al_load_font("luculent.ttf", 36, 0);
	
	game.loadButton();
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
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
			game.checkButtons(ev.mouse.x,ev.mouse.y);//update if mouse over button
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) //checks if mouse button pressed
		{
			if (ev.mouse.button & 1)	{ game.clickButtons(1, ev.mouse.x, ev.mouse.y); }//if left mouse button pressed			
			if (ev.mouse.button & 2)	{ game.clickButtons(2, ev.mouse.x, ev.mouse.y); }//if right mouse button pressed
		}		
		else if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
		{
			if (game.checkGameState(SAVING_SCORE)) { game.enterPlayerName(ev.keyboard.keycode, ev.keyboard.unichar); }
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			// 60 times per second
			game.updateScore();
			game.updatePositions();
			if (game.destroy_brick) 
			{ 
			
				game.destroyBrick();
			}
			if (game.checkGameState(REFRESH_GAME))	{ game.newGame(false); } //if "new game" or "map size" button pressed
			if (game.checkGameState(CHEAT))			{ game.newGame(true); }
			if (game.checkGameState(PLAY_GAME) )	{ game.checkEndGame(); }
			game.updateNumberOfSelected();
			render = true;
		}
		//=========RENDERER
		if (render && al_is_event_queue_empty(event_queue))
		{
			game.drawGameArea();
			if (game.checkGameState(OPTIONS))											{ game.options(); }
			if (game.checkGameState(HIGH_SCORE))										{ game.highScores(); }
			if (game.checkGameState(END_GAME) || game.checkGameState(SAVING_SCORE))	{ game.endGame(); }
			if (game.checkGameState(SAVING_SCORE))
			{
				al_draw_textf(font18, WHITE, game.screen_width / 2-100, game.screen_height / 2 + 100, ALLEGRO_ALIGN_CENTRE, "Enter your name:");
				al_draw_text(font18, WHITE, game.screen_width / 2, game.screen_height / 2 + 100, NULL,"_____________________");
				al_draw_ustr(font18, YELLOW, game.screen_width / 2, game.screen_height / 2 + 100, NULL, game.edited_text);
			}
			al_flip_display();
			render = false;
		}
	}
	game.saveGame();
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}