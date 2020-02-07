#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include "game.h"
#include "define.h"


//GLOBAL VARIABLES
int BRICK_COLORS = 6;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_FONT *font18 = NULL;
ALLEGRO_FONT *font24 = NULL;
ALLEGRO_FONT *font36 = NULL;

int main(int argc, char **argv)
{
	//======INIT
	cGame *game = new cGame();
	game->game_state = MAIN_MENU; //debug
	game->update();//main loop
	game->saveGame();//save game if main loop is broken
	//TODO game.destructor //destroying game object
	al_destroy_display(display); // closing window
	delete game;
	return 0;
}