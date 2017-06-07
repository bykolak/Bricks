#include <vector>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include "game.h"

//=====cGame methods
cGame::cGame() //default constructor
	:bricks(BRICKS_MAP_X, std::vector<cTile>(BRICKS_MAP_Y))
{
	done = false;
//	opacity = 0;
	currently_selected = 0;
	score_count = 0;
	game_state = REFRESH_GAME;
	score = 0;
	selection = false;
	destroy_brick = false;
//	brick_size = BRICK_SIZE;
	bricks_on_screen = BRICKS_MAP_X * BRICKS_MAP_Y;
	area_width = BRICK_SIZE * BRICKS_MAP_X;
	area_height = BRICK_SIZE * BRICKS_MAP_Y;
	screen_width = SCREEN_X;//LEFT_MARGIN + area_width + RIGHT_MARGIN;
	screen_height = SCREEN_Y;//TOP_MARGIN + area_height + DOWN_MARGIN;
	left_game_area_margin = (screen_width - area_width) / 2;
	curFrame = 0;
	frameCount = 0;
	player_name = NULL;
	al_init();
	display = al_create_display(screen_width, screen_height);
	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	// LOADING GRAPHICS
	al_init_image_addon();
	bricksPNG = al_load_bitmap("bricks.png");
	explosionPNG = al_load_bitmap("explosion.png");
	backgroundPNG = al_load_bitmap("background.png");
	mainPNG = al_load_bitmap("main.png");
	optionsPNG = al_load_bitmap("options.png");
	highscorePNG = al_load_bitmap("highscore.png");
//	newgamePNG = al_load_bitmap("newgame.png");
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
	
	font24 = al_load_font("bebas.ttf", 24, 0);
	font18 = al_load_font("bebas.ttf", 20, 0);
	font36 = al_load_font("bebas.ttf", 36, 0);
	float FPS = 60.0;
	srand(time(NULL));
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	timer = al_create_timer(1.0 / FPS);
	timer2 = al_create_timer(1.0 / SELECTION_SPEED);
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_timer_event_source(timer2));

	button[PLAY_BUTTON].create(screen_width / 2 - BUTTON_SIZE/2, screen_height / 2 - BUTTON_SIZE-AWAY_FROM_CENTER, BUTTON_SIZE, BUTTON_SIZE, RHOMB, "PLAY");
	button[OPTIONS_BUTTON].create(screen_width / 2 - BUTTON_SIZE - AWAY_FROM_CENTER, screen_height / 2 - BUTTON_SIZE/2, BUTTON_SIZE, BUTTON_SIZE, RHOMB, "OPTIONS");
	button[HIGHSCORES_BUTTON].create(screen_width / 2 + AWAY_FROM_CENTER, screen_height / 2 - BUTTON_SIZE / 2, BUTTON_SIZE, BUTTON_SIZE, RHOMB,"HIGH SCORES");
	button[EXIT_BUTTON].create(screen_width / 2 - BUTTON_SIZE / 2, screen_height / 2 + AWAY_FROM_CENTER, BUTTON_SIZE , BUTTON_SIZE, RHOMB,"EXIT");
	button[NEW_STORY_BUTTON].create(screen_width / 2 - BUTTON_SIZE - AWAY_FROM_CENTER , screen_height / 2 - BUTTON_SIZE - AWAY_FROM_CENTER - BUTTON_SIZE / 2-20, BUTTON_SIZE, BUTTON_SIZE, RHOMB, "NEW STORY");
	button[LOAD_GAME_BUTTON].create(screen_width / 2 - BUTTON_SIZE / 2, screen_height / 2 - 2*BUTTON_SIZE - 3*AWAY_FROM_CENTER, BUTTON_SIZE, BUTTON_SIZE, TRIANGLE, "LOAD");
	button[NEW_RANDOM_BUTTON].create(screen_width / 2 + AWAY_FROM_CENTER, screen_height / 2 - BUTTON_SIZE - 2*AWAY_FROM_CENTER - BUTTON_SIZE / 2 , BUTTON_SIZE, BUTTON_SIZE, RHOMB, "NEW RANDOM");
	button[MUSIC_MUTE_BUTTON].create(screen_width / 16+MUSIC_MUTE_X, screen_height / 4+MUSIC_MUTE_Y , 24, 24, GAME_AREA, ""); //remove magic numbers 414,580
	button[SOUND_MUTE_BUTTON].create(screen_width - screen_width / 3+SOUND_MUTE_X, screen_height / 4 + SOUND_MUTE_Y, 24,24, GAME_AREA, "");//remove magic numbers 414,615
	button[RESET_PROFILE_BUTTON].create(414, 655, 24, 24, GAME_AREA, "");//remove magic numbers
	button[GAME_AREA_BUTTON].create(left_game_area_margin, TOP_MARGIN, area_width, area_height, GAME_AREA,"");
	button[OPTIONS_POPUP].create(screen_width / 16, screen_height / 4, al_get_bitmap_width(optionsPNG), al_get_bitmap_height(optionsPNG), RECTANGLE, "");//temp shit
	button[HIGHSCORES_POPUP].create(screen_width / 16, screen_height / 4, al_get_bitmap_width(optionsPNG), al_get_bitmap_height(optionsPNG), RECTANGLE, "");//temp shit
	button[NEW_STORY_BUTTON].opacity = 0.0;
	button[LOAD_GAME_BUTTON].opacity = 0.0;
	button[OPTIONS_POPUP].opacity = 0.0;
	button[HIGHSCORES_POPUP].opacity = 0.0;
	button[NEW_RANDOM_BUTTON].opacity = 0.0;
	button[NEW_STORY_BUTTON].fadeIn = false;
	button[LOAD_GAME_BUTTON].fadeIn = false;
	button[NEW_RANDOM_BUTTON].fadeIn = false;
	button[OPTIONS_POPUP].fadeIn = false;
	button[HIGHSCORES_POPUP].fadeIn = false;
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
	//al_draw_textf(font36, BLACK, left_button_margin + BUTTON_WIDTH + 240-f, 8, ALLEGRO_ALIGN_RIGHT, " %i", on_screen_score);
	//al_draw_textf(font36, BLACK, left_button_margin + BUTTON_WIDTH + 240+f, 8, ALLEGRO_ALIGN_RIGHT, " %i", on_screen_score);
	al_draw_textf(font36, WHITE, left_button_margin  + 750, 1010, ALLEGRO_ALIGN_RIGHT, " %i", on_screen_score);
	al_draw_textf(font36, WHITE, left_button_margin  + 200, 8, ALLEGRO_ALIGN_RIGHT, " selectionList.size:%i", selectionList.size());
	al_draw_textf(font36, WHITE, left_button_margin  + 600, 8, ALLEGRO_ALIGN_RIGHT, " score:%i", score);
}
void cGame::clickButtons(int mouseButton)
{
	if (mouseButton == 1) //if left mouse button pressed
	{
		al_play_sample_instance(instanceClick2);
		if (game_state == MAIN_MENU)
		{
			int click = -1;
			if (button[PLAY_BUTTON].mouseOver) { click = PLAY_BUTTON; }
			if (button[OPTIONS_BUTTON].mouseOver) { click = OPTIONS_BUTTON; }
			if (button[HIGHSCORES_BUTTON].mouseOver) { click = HIGHSCORES_BUTTON; }
			if (button[EXIT_BUTTON].mouseOver) { click = EXIT_BUTTON; }


			if (click != -1) //if button was pressed
			{
				for (int i = 0; i < MAX_BUTTONS; i++)
				{
					button[i].clicked = false;//unclick all buttons
				}
				button[click].clicked = true;//click button
			}
			if (button[PLAY_BUTTON].clicked)
			{
				button[NEW_STORY_BUTTON].fadeIn = true;
				button[LOAD_GAME_BUTTON].fadeIn = true;
				button[NEW_RANDOM_BUTTON].fadeIn = true;
				click = -1;
				if (button[NEW_STORY_BUTTON].mouseOver) { click = NEW_STORY_BUTTON; }
				if (button[LOAD_GAME_BUTTON].mouseOver) { click = LOAD_GAME_BUTTON; }
				if (button[NEW_RANDOM_BUTTON].mouseOver) { click = NEW_RANDOM_BUTTON; }
				if (click > -1) { button[click].clicked = true; }
			}
			else
			{
				button[NEW_STORY_BUTTON].fadeIn = false;
				button[LOAD_GAME_BUTTON].fadeIn = false;
				button[NEW_RANDOM_BUTTON].fadeIn = false;
			}

			if (button[OPTIONS_BUTTON].clicked)
			{
				button[OPTIONS_POPUP].fadeIn = true;
			}
			else
			{
				button[OPTIONS_POPUP].fadeIn = false;
			}

			if (button[HIGHSCORES_BUTTON].clicked)
			{
				button[HIGHSCORES_POPUP].fadeIn = true;
			}
			else
			{
				button[HIGHSCORES_POPUP].fadeIn = false;
			}

		}

		if (game_state == PLAY_GAME) //if playing game check these clicks
		{
			if (button[GAME_AREA_BUTTON].mouseOver) { al_play_sample_instance(instanceClick2); selectBrick(); }
		}
	}
	if (mouseButton == 2) //if right mouse button pressed
	{
		if (game_state == MAIN_MENU)
		{
			resetHighScores();//debug
		}
		if (game_state == PLAY_GAME) //if playing game check these clicks
		{
			//if (button[PLAY_BUTTON].mouseOver) { game_state = CHEAT; }
			if (button[GAME_AREA_BUTTON].mouseOver)			{	changeTile((mouse.x - left_game_area_margin ) / BRICK_SIZE, (mouse.y - TOP_MARGIN) / BRICK_SIZE, 0);	} //debug	
			else game_state = MAIN_MENU;
		}
		
	}
}
void cGame::update()
{
	bool render = true;

	al_start_timer(timer);
	al_start_timer(timer2);
	while (!done)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);
		//==========INPUT
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // checks for window Xed
		{
			done = true;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) //checks if mouse moved 
		{
			mouse.x = event.mouse.x;
			mouse.y = event.mouse.y;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) //checks if mouse button pressed
		{
			if (event.mouse.button & 1) { clickButtons(1); }//if left mouse button pressed			
			if (event.mouse.button & 2) { clickButtons(2); }//if right mouse button pressed
			
		}
		else if (event.type == ALLEGRO_EVENT_KEY_CHAR)
		{
			if (game_state == SAVING_SCORE) { enterPlayerName(event.keyboard.keycode, event.keyboard.unichar); }
		}
		else if (event.type == ALLEGRO_EVENT_TIMER)
		{
			if (event.timer.source == timer)
			{
				for (int i = 0; i < MAX_BUTTONS; i++) { button[i].update(mouse); } //updates all buttons
				// 60 times per second
				if (button[NEW_RANDOM_BUTTON].clicked) { button[NEW_RANDOM_BUTTON].clicked = false; newGame(false); }
				if (button[NEW_STORY_BUTTON].clicked) { button[NEW_STORY_BUTTON].clicked = false; newGame(true); }
				if (button[EXIT_BUTTON].clicked) { done = true; }
				if (destroy_brick)
				{
					destroyBrick();
				}
				updateScore();
				if (game_state == PLAY_GAME) { checkEndGame(); }
				render = true;
			}

			if (event.timer.source == timer2)	updateNumberOfSelected();
		
		}
		//=========RENDERER
		if (render && al_is_event_queue_empty(event_queue))
		{
			render = false;
			drawGameArea();
		//	if (game_state == OPTIONS) { options(); }
		//	if (game_state == HIGH_SCORE) { highScores(); }
			if (game_state == END_GAME || game_state == SAVING_SCORE) { endGame(); }
			if (game_state == SAVING_SCORE)
			{
//				al_draw_textf(font18, WHITE, screen_width / 2 - 100, screen_height / 2 + 100, ALLEGRO_ALIGN_CENTRE, "Enter your name:");
//				al_draw_text(font18, WHITE, screen_width / 2, screen_height / 2 + 100, NULL, "_____________________");
//				al_draw_ustr(font18, YELLOW, screen_width / 2, screen_height / 2 + 100, NULL, edited_text);
			}
			if (game_state == MAIN_MENU)
			{
				drawMenu();
				
				//highScores();
			}
			al_flip_display();
			
		}
		
	}
}
void cGame::resetHighScores() //resets all saved highscores TODO
{
	for (int i = 0; i < MAX_HIGH_SCORE; i++)
	{

		high_score[i] = 0;
		al_ustr_free(high_score_name[i]);
		high_score_name[i] = al_ustr_new("WWWWWWWWWWWWWWW");
		high_score[i] = 1234567;
	}
	saveHighScore();
}
void cGame::updateNumberOfSelected() //checks and update number of selected bricks
{
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
	al_draw_bitmap(backgroundPNG,0, 0, NULL);
	left_game_area_margin = (screen_width - area_width) / 2;
	for (int i = 0; i < BRICKS_MAP_Y; i++)
		for (int t = 0; t < BRICKS_MAP_X; t++)
		{
			bricks[t][i].draw(bricksPNG);
		}
	for (int i = 0; i<BRICKS_MAP_Y; i++)
		for (int t = 0; t < BRICKS_MAP_X; t++)
		{
			//al_draw_textf(font18, BLACK, t*brick_size + left_game_area_margin, i*brick_size + TOP_MARGIN + screen_shake, NULL, "%i,%i", t,i); //debug: shows brick position in bricks[t][i] array
			if (bricks[t][i].state == EXPLODING)
			{
				al_draw_bitmap_region(explosionPNG, 240 * curFrame, 0, 240, 240, (t - 2)*BRICK_SIZE + left_game_area_margin, (i - 2)*BRICK_SIZE + TOP_MARGIN, NULL);
			}
		}
	
	if (destroy_brick)
	{
		int f = 2;
		//al_play_sample(explosionOGG, 1, 0, 1.3, ALLEGRO_PLAYMODE_ONCE, NULL);
		al_draw_textf(font36, BLACK, (last_clicked_x *BRICK_SIZE) + left_game_area_margin + (BRICK_SIZE / 2)-f, (last_clicked_y *BRICK_SIZE) + TOP_MARGIN - (curFrame * 4)-f, NULL, "%i", last_score);
		al_draw_textf(font36, BLACK, (last_clicked_x *BRICK_SIZE) + left_game_area_margin + (BRICK_SIZE / 2) + f, (last_clicked_y *BRICK_SIZE) + TOP_MARGIN - (curFrame * 4) + f, NULL, "%i", last_score);
		al_draw_textf(font36, WHITE, (last_clicked_x *BRICK_SIZE) + left_game_area_margin + (BRICK_SIZE / 2), (last_clicked_y *BRICK_SIZE) + TOP_MARGIN - (curFrame * 4), NULL, "%i", last_score);
	}
	drawScore();
}
void cGame::newGame(bool debug) // restart game
{
	for (int i = 0; i<BRICKS_MAP_Y; i++)
		for (int t = 0; t < BRICKS_MAP_X; t++)
		{
			sPoint position = { t,i };
			int color = rand() % BRICK_COLORS;
			if (debug) color = 1;//DEBUG
			bricks[t][i].create(position,color);
		}
	
	for (i = 0; i < MAX_BUTTONS; i++)
	{
		button[i].mouseOver = false;
	}
	game_state = PLAY_GAME;
	score = 0;
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
void cGame::endGame()
{
	//al_draw_tinted_bitmap(shadowBMP, TINT, 0, 0, 0);
//	al_draw_bitmap(scoreBMP, screen_width / 2 - (al_get_bitmap_width(scoreBMP) / 2), screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2), 0);
//	button[END_GAME_NEW_GAME_BUTTON].draw();
//	if (checkSaveScores()) button[END_GAME_SAVE_SCORE_BUTTON].draw();
	for (int i = 0; i < MAX_HIGH_SCORE; i++)
	{
		int f = 26;
	//	al_draw_ustr(font24, WHITE, screen_width / 2 - 300, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + f * i , ALLEGRO_ALIGN_LEFT, high_score_name[i]);
	//	al_draw_textf(font24, WHITE, screen_width / 2 + 250, screen_height / 2 - (al_get_bitmap_height(scoreBMP) / 2) + f * i , ALLEGRO_ALIGN_CENTRE, "%i ", high_score[i]);
	}
	al_draw_textf(font24, WHITE, screen_width / 2, screen_height / 2 + 75, ALLEGRO_ALIGN_CENTRE, "you scored %i points !", on_screen_score);
}
void cGame::selectBrick() // takes mouse input and selects all same color bricks that are neighboruing to  bricks[x][y]
{
	int x = (mouse.x - left_game_area_margin) / BRICK_SIZE;
	int y = (mouse.y - TOP_MARGIN) / BRICK_SIZE;
	last_clicked_x = x;
	last_clicked_y = y;
	int refresh = 0; 
	if (!selection && bricks[x][y].state == FULL) //if not selected and full
	{
		selectionList.clear();
		cList currentList(x,y,false);
		selectionList.push_back(currentList);

		for (unsigned int i = 0; i < selectionList.size(); i++)
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
		if (selectionList.size() > 1)		{	selection = true;	}
		else		{	selectionList.clear();		}
		
		currently_selected = 0;
	}else 
	if (selection && bricks[x][y].state == SELECTED) // else if selected
	{
		destroy_brick = true;
		//selectionList.clear();
		for (int i = 0; i < BRICKS_MAP_X; i++)
			for (int t = 0; t < BRICKS_MAP_Y; t++)
			{
				if (bricks[i][t].state == SELECTED) bricks[i][t].state = EXPLODING;//change selected to explosion status
			}	
	}else 
	if (selection && bricks[x][y].state != SELECTED)
	{
		deselectBrick();	
		selectBrick();
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
	float explosion_volume = 0.3 + 0.05 * selectionList.size(); //more bricks, louder explosion
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
	int iterator = selectionList.size();
	for (int i = 0; i<iterator; i++)
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
void cGame::drawMenu()
{
	al_draw_bitmap(mainPNG, 0, 0, 0);
	for (int i = 0; i < MAX_BUTTONS; i++)	{	button[i].draw(!BUTTON_OVERLAY);	}//if flags set to true
	if (button[HIGHSCORES_BUTTON].clicked)
	{
		button[HIGHSCORES_POPUP].clicked = true;
	}
	if (button[HIGHSCORES_POPUP].opacity == 0) { button[HIGHSCORES_POPUP].clicked = false; }
	
	if (button[OPTIONS_BUTTON].clicked)
	{
		button[OPTIONS_POPUP].clicked = true;
	}
	if (button[OPTIONS_POPUP].opacity == 0) { button[OPTIONS_POPUP].clicked = false; }

//draw highscores
		float highX = al_get_bitmap_width(highscorePNG);
		float highY = al_get_bitmap_height(highscorePNG);
		 high_scoresTMP= al_create_bitmap(highX,highY); //creates empty bitmap of highscoresPNG size
		al_set_target_bitmap(high_scoresTMP);
		al_draw_bitmap(highscorePNG, 0, 0, NULL);
		for (int i = 0; i < MAX_HIGH_SCORE; i++)
		{			
			al_draw_textf(font18, WHITE, 70 ,(28 * i) + 80, ALLEGRO_ALIGN_CENTRE, "1.");
			al_draw_ustr(font18, WHITE, 80, (28 * i) + 80, ALLEGRO_ALIGN_LEFT, high_score_name[i]);
			al_draw_textf(font18, WHITE, 360,(28 * i) + 80, ALLEGRO_ALIGN_CENTRE, "Points: %i", high_score[i]);
		}
		al_set_target_bitmap(al_get_backbuffer(display));
		al_draw_tinted_scaled_bitmap(high_scoresTMP, al_map_rgba_f(button[HIGHSCORES_POPUP].opacity, button[HIGHSCORES_POPUP].opacity, button[HIGHSCORES_POPUP].opacity, button[HIGHSCORES_POPUP].opacity),
			0, 0, highX, highY, screen_width - screen_width / 3, screen_height / 4, highX, highY, 0);
		al_destroy_bitmap(high_scoresTMP);//always destroy temp bitmaps
	
//draw options	
		al_draw_tinted_scaled_bitmap(optionsPNG, al_map_rgba_f(button[OPTIONS_POPUP].opacity, button[OPTIONS_POPUP].opacity, button[OPTIONS_POPUP].opacity, button[OPTIONS_POPUP].opacity),
			0, 0, al_get_bitmap_width(optionsPNG), al_get_bitmap_height(optionsPNG), screen_width / 16, screen_height / 4, al_get_bitmap_width(optionsPNG), al_get_bitmap_height(optionsPNG), 0);
	
}
//=====cList methods
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

void cTile::create(sPoint position, int _color)
{
	x = position.x*BRICK_SIZE + (SCREEN_X - AREA_WIDTH) / 2;
	y = position.y*BRICK_SIZE + (SCREEN_Y - AREA_HEIGHT) / 2;
	state = FULL;
	color = _color;
}

void cTile::update()
{
	//t*BRICK_SIZE + left_game_area_margin, i*BRICK_SIZE + TOP_MARGIN + screen_shake            (x,y)
}

void cTile::draw(ALLEGRO_BITMAP * bricksPNG)
{
	if (state == FULL || state == SELECTED)
	{
		al_draw_bitmap_region(bricksPNG, color*BRICK_SIZE, 0, BRICK_SIZE, BRICK_SIZE, x,y, NULL);
	}
	if (state == SELECTED)
	{
		al_draw_bitmap_region(bricksPNG, 0, BRICK_SIZE, BRICK_SIZE, BRICK_SIZE, x,y, NULL);
	}
}
