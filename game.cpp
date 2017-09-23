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
//	resetHighScores();//DEBUG: clears high scores from names and points
	done = false;
	game_state = REFRESH_GAME;
	destroy_brick_flag = false;
	bricks_on_screen = BRICKS_MAP_X * BRICKS_MAP_Y;
	area_width = BRICK_SIZE * BRICKS_MAP_X;
	area_height = BRICK_SIZE * BRICKS_MAP_Y;
	screen_width = SCREEN_X;
	screen_height = SCREEN_Y;
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
	//bitmap.load();
	bitmap = new cBitmaps();
	score.loadHighScore();
	loadGame();
	game_state = PLAY_GAME;
	//LOADING SOUND
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(10 );
	explosionOGG = al_load_sample("explosion.wav");
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
	button[PLAY_BUTTON].create(screen_width / 2 - BUTTON_SIZE/2, screen_height / 2 - BUTTON_SIZE-AWAY_FROM_CENTER, 36, MENU_BUTTON, al_ustr_new("PLAY"));
	button[OPTIONS_BUTTON].create(screen_width / 2 - BUTTON_SIZE - AWAY_FROM_CENTER, screen_height / 2 - BUTTON_SIZE/2, 36, MENU_BUTTON, al_ustr_new("OPTIONS"));
	button[SCORES_BUTTON].create(screen_width / 2 + AWAY_FROM_CENTER, screen_height / 2 - BUTTON_SIZE / 2, 36, MENU_BUTTON, al_ustr_new("SCORES"));
	button[EXIT_BUTTON].create(screen_width / 2 - BUTTON_SIZE / 2, screen_height / 2 + AWAY_FROM_CENTER, 36, MENU_BUTTON, al_ustr_new("EXIT"));
	button[LOAD_GAME_BUTTON].create(screen_width / 2 - BUTTON_SIZE, screen_height / 2 - BUTTON_SIZE , 36, TEXT_BUTTON, al_ustr_new("LOAD"));
	button[NEW_RANDOM_BUTTON].create(screen_width / 2 + BUTTON_SIZE- al_get_text_width(font36, "NEW "), screen_height / 2 - BUTTON_SIZE , 36, TEXT_BUTTON, al_ustr_new("NEW"));
	button[GAME_AREA_BUTTON].create(0, TOP_MARGIN, 36, GAME_AREA, al_ustr_new(" "));
	options_menu.createOptions();
	scores_menu.createScores();
} 
void cGame::clickButtons(int mouseButton)
{
	if (mouseButton == 1) //if left mouse button pressed
	{
		al_play_sample_instance(instanceClick2);
		if (game_state == MAIN_MENU)
		{
			int click = -1;
			if (button[PLAY_BUTTON].isMouseOver()) { click = PLAY_BUTTON; }
			if (button[OPTIONS_BUTTON].isMouseOver()) { click = OPTIONS_BUTTON; }
			if (button[SCORES_BUTTON].isMouseOver()) { click = SCORES_BUTTON; }
			if (button[EXIT_BUTTON].isMouseOver()) { click = EXIT_BUTTON; }
			if (click != -1) //if button was pressed
			{
				for (int i = 0; i < MAX_BUTTONS; i++)
				{
					button[i].changeClicked(false);//unclick all buttons
				}
				button[click].changeClicked(true);//click button
			}
			if (button[PLAY_BUTTON].isClicked())
			{
				button[LOAD_GAME_BUTTON].fade(true);
				button[NEW_RANDOM_BUTTON].fade(true);
				click = -1;
				if (button[LOAD_GAME_BUTTON].isMouseOver()) { click = LOAD_GAME_BUTTON; }
				if (button[NEW_RANDOM_BUTTON].isMouseOver()) { click = NEW_RANDOM_BUTTON; }
				if (click > -1) { button[click].changeClicked(true); }
			}
			else
			{
				button[LOAD_GAME_BUTTON].fade(false);
				button[NEW_RANDOM_BUTTON].fade(false);
			}

		if (button[OPTIONS_BUTTON].isClicked())			{		options_menu.fade(true);			}
			else										{		options_menu.fade(false);			}

			if (button[SCORES_BUTTON].isClicked())		{ scores_menu.fade(true);			}
			else										{		scores_menu.fade(false);		}
		}

		if (game_state == PLAY_GAME) //if playing game check these clicks
		{
			if (button[GAME_AREA_BUTTON].isMouseOver()) { al_play_sample_instance(instanceClick2); selectBrick(); }
		}
		
	}
	if (mouseButton == 2) //if right mouse button pressed
	{
		if (game_state == MAIN_MENU)		{			score.resetHighScores();		}//debug
		if (game_state == PLAY_GAME) //if playing game check these clicks
		{
			if (button[GAME_AREA_BUTTON].isMouseOver())			
			{
				float posX = mouse.bricksX;
				float posY = mouse.bricksY;
				sPoint position { posX,posY };
				bricks[mouse.bricksX][mouse.bricksY].create(position, 1, FULL);
			} //debug	
			else game_state = MAIN_MENU;
		}	
	}
}
void cGame::update()
{
	bool render = true;
	bool dropDone = true;
	al_start_timer(timer);
	al_start_timer(timer2);
	while (!done)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);
		//==========INPUT
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)		{			done = true;		}// checks for window Xed
		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) //checks if mouse moved 
		{
			mouse.x = event.mouse.x;//position in pixels
			mouse.y = event.mouse.y;
			mouse.bricksX = mouse.x  / BRICK_SIZE; // bricks[bricksX][bricksY]
			mouse.bricksY = (mouse.y - TOP_MARGIN) / BRICK_SIZE;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) //checks if mouse button pressed
		{
			if (!destroy_brick_flag)
			{
				if (event.mouse.button & 1) { clickButtons(1); }//if left mouse button pressed			
				if (event.mouse.button & 2) { clickButtons(2); }//if right mouse button pressed
			}		
		}
		else if (event.type == ALLEGRO_EVENT_KEY_CHAR)
		{
			if (game_state == SAVING_SCORE) { if(score.enterPlayerName(event.keyboard.keycode, event.keyboard.unichar))game_state=MAIN_MENU; }//if enterPlayerName returns true(pressed enter) change game_state to MAIN_MENU
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && game_state==PLAY_GAME) { game_state=MAIN_MENU; }
		}
		else if (event.type == ALLEGRO_EVENT_TIMER)
		{
			if (event.timer.source == timer)
			{
				for (int i = 0; i < MAX_BUTTONS; i++) 
				{ 
					button[i].update(mouse); 
					
				} //updates all buttons
				scores_menu.update(mouse);
				options_menu.update(mouse);
				// 60 times per second
				if (button[NEW_RANDOM_BUTTON].isClicked()) { button[NEW_RANDOM_BUTTON].changeClicked(false); newGame(false); }
				if (button[LOAD_GAME_BUTTON].isClicked()) { button[LOAD_GAME_BUTTON].changeClicked(false); newGame(true); }
				if (button[EXIT_BUTTON].isClicked()) { done = true; }
				if (game_state == PLAY_GAME) 
				{
					score.update();
					for (int i = 0; i < BRICKS_MAP_X; i++)
					for (int t = 0; t < BRICKS_MAP_Y; t++)  {		bricks[i][t].update();				}
					if (destroy_brick_flag)					{		destroyBrick();						}
					if (!dropColumn())						{		if (!moveLeft())	checkEndGame();	}
				}
				if (game_state == END_GAME)
				{
					if (score.checkSaveScores())					{		game_state = SAVING_SCORE;			}
				}
				render = true;
			}	
		}
		//=========RENDERER
		if (render && al_is_event_queue_empty(event_queue))
		{
			render = false;
			if (game_state != MAIN_MENU) drawGameArea();
			if (game_state == END_GAME || game_state == SAVING_SCORE) { //score.drawHighScores(*this); 
			if (game_state == SAVING_SCORE) score.drawEnd(); }

			if (game_state == MAIN_MENU)		{	drawMenu(); 	}
			al_flip_display();
		}	
	}
}
void cGame::drawGameArea() // draw all bricks on screen;
{
	al_draw_bitmap(bitmap->backgroundPNG,0, 0, NULL);
	for (int i = 0; i < BRICKS_MAP_Y; i++)
		for (int t = 0; t < BRICKS_MAP_X; t++)
		{
			bricks[t][i].draw(*bitmap);
		}
	for (unsigned int i = 0; i < selectionList.size(); i++)
	{
		sPoint position = selectionList[i].returnPosition();
		bricks[position.bricksX][position.bricksY].draw(*bitmap);
	}
	if (destroy_brick_flag)
	{
	//	score.drawHighScores(bitmap);
		//al_play_sample(explosionOGG, 1, 0, 1.3, ALLEGRO_PLAYMODE_ONCE, NULL);
	
	}
	int f = 1;
	al_draw_textf(font36, WHITE, 818, 1004, ALLEGRO_ALIGN_LEFT, " %i", score.on_screen);
	al_draw_textf(font36, WHITE, 50, 8, ALLEGRO_ALIGN_LEFT, " selectionList.size:%i", selectionList.size());
	al_draw_textf(font36, WHITE, 600, 8, ALLEGRO_ALIGN_LEFT, " score:%i", score);
}
void cGame::newGame(bool debug) // restart game
{
	for (int i = 0; i<BRICKS_MAP_Y; i++)
		for (int t = 0; t < BRICKS_MAP_X; t++)
		{
			sPoint position = { static_cast<float>(t),static_cast<float>(i)};
			int color = rand() % BRICK_COLORS;
			if (debug)
			{
				if (t % 2 && i % 2)	bricks[t][i].create(position, 1, FULL);
				else if (t % 4 && i % 4)
				{
					bricks[t][i].create(position, 4, FULL);
				}else
					bricks[t][i].create(position, 3, FULL);
			}else 
			bricks[t][i].create(position,color,FULL);
		}
	
	/*for (int i = 0; i < MAX_BUTTONS; i++)
	{
		button[i].mouseOver = false;
	} 
	//resets buttons
	*/
	game_state = PLAY_GAME;
	score.reset();
	
}
void cGame::checkEndGame() //checks if game ended (no more bricks to destroy)
{
	int selected = 0;
	int x = 0;
	int y = 0;	
		for (x = BRICKS_MAP_X - 1; x >= 0; x--)
			for (y = BRICKS_MAP_Y - 1; y >= 0; y--)
			{
				if (bricks[x][y].returnState()!=EMPTY)
				{
					if (x > 0)//avoids going outside of vector
					{
					if (bricks[x][y].returnColor() == bricks[x - 1][y].returnColor() && bricks[x - 1][y].returnState() != EMPTY)		{ selected++; }
					}
					if (x + 1 < BRICKS_MAP_X) //avoids going outside of vector
					{
					if (bricks[x][y].returnColor() == bricks[x + 1][y].returnColor() && bricks[x + 1][y].returnState() != EMPTY)		{ selected++; }
					}
					if (y > 0) //avoids going outside of vector
					{
					if (bricks[x][y].returnColor() == bricks[x][y - 1].returnColor() && bricks[x][y - 1].returnState() != EMPTY) 	{ selected++; }
					}
					if (y + 1 < BRICKS_MAP_Y) //avoids going outside of vector
					{
					if (bricks[x][y].returnColor() == bricks[x][y + 1].returnColor() && bricks[x][y + 1].returnState() != EMPTY)		{ selected++; }
					}
				}
			}
		if (selected == 0 && !destroy_brick_flag) game_state = END_GAME;		
}
void cGame::selectBrick() // takes mouse input and selects all same color bricks that are neighboruing to  bricks[x][y]
{
	int xx = mouse.bricksX;
	int yy = mouse.bricksY;
	int refresh = 0; 
	if (selectionList.size()==0 && bricks[xx][yy].returnState() == FULL) //if not selected and full
	{
		selectionList.clear();
		cTile currentList;
		sPoint position = bricks[xx][yy].returnPosition();

		currentList.create(position, bricks[xx][yy].returnColor(),bricks[xx][yy].returnState());
		selectionList.push_back(currentList);
		for (unsigned int i = 0; i < selectionList.size(); i++)
		{
			sPoint position = selectionList[i].returnPosition();
			int x = position.bricksX;
			int y = position.bricksY;

			if (y - 1 >= 0)
				if (bricks[x][y].compare(bricks[x][y - 1]))
				{
					selectionList.push_back(bricks[x][y - 1]);
					bricks[x][y - 1].selected = true;
				}
			if (y + 1 < BRICKS_MAP_Y)
				if (bricks[x][y].compare(bricks[x][y + 1]))
				{
					selectionList.push_back(bricks[x][y + 1]);
					bricks[x][y + 1].selected = true;
				}
			if (x - 1 >= 0)
				if (bricks[x][y].compare(bricks[x - 1][y]))
				{
					selectionList.push_back(bricks[x - 1][y]);
					bricks[x - 1][y].selected = true;
				}
			if (x + 1 <BRICKS_MAP_X)
				if (bricks[x][y].compare(bricks[x + 1][y]))
				{
					selectionList.push_back(bricks[x + 1][y]);
					bricks[x + 1][y].selected = true;
				}
		}
		if (selectionList.size() < 2)
		{
			for (unsigned int i = 0; i < selectionList.size(); i++)
			{
				sPoint pos = selectionList[i].returnPosition();
				bricks[pos.bricksX][pos.bricksY].selected = false;
			}
			selectionList.clear();
		}
		else
		{
			for (unsigned int i = 0; i < selectionList.size(); i++)
			{
				sPoint pos = selectionList[i].returnPosition();
				int color = bricks[pos.bricksX][pos.bricksY].returnColor();
				bricks[pos.bricksX][pos.bricksY].create(pos, color,SELECTED);
				bricks[pos.bricksX][pos.bricksY].setAnimationDelay(i/8);
			}
		}
	}else 
	if (selectionList.size()>0 && bricks[xx][yy].returnState() == SELECTED)// else if selected
	{
		destroy_brick_flag = true;
		for (unsigned int i = 0; i < selectionList.size(); i++)
		{
			sPoint pos = selectionList[i].returnPosition();
			bricks[pos.bricksX][pos.bricksY].create( pos, selectionList[i].returnColor(), EXPLODING);
			bricks[pos.bricksX][pos.bricksY].setAnimationDelay(i/4);
			bricks[pos.bricksX][pos.bricksY].selected = false;
		}
	//	score += 
			score.calculateScore(selectionList.size());
		//last_score = score.calculateScore(selectionList.size());
	}else 
	if (selectionList.size()>0 && bricks[xx][yy].returnState() != SELECTED)//selcted other bricks
		{
			for (unsigned int i = 0; i < selectionList.size(); i++)
			{
				sPoint pos = selectionList[i].returnPosition();
				bricks[pos.bricksX][pos.bricksY].create(pos, selectionList[i].returnColor(), FULL);
				bricks[pos.bricksX][pos.bricksY].selected = false;
			}
			selectionList.clear();
		selectBrick();
		}
}
void cGame::destroyBrick() // after clicking selected bricks destroys them
{
	bool animationEnds = false;
	for (unsigned int i = 0; i < selectionList.size(); i++)
	{
		sPoint pos = selectionList[i].returnPosition();
		if (bricks[pos.bricksX][pos.bricksY].returnState() == EMPTY) { animationEnds = true; }
		else animationEnds = false;
	}
	if (selectionList.size() == 0 || animationEnds) { explosion_finish_flag = true; }
	float explosion_volume = 0.3 + 0.05 * selectionList.size(); //more bricks, louder explosion
	if (explosion_volume > 1.2) explosion_volume = 1.2; // up to 150% volume
	al_set_sample_instance_gain(instance, explosion_volume);
	al_play_sample_instance(instance);
	if (explosion_finish_flag)
	{
		for (unsigned int i = 0; i < BRICKS_MAP_Y; i++)
			for (unsigned int t = 0; t < BRICKS_MAP_X; t++)
			{
				sPoint position = bricks[t][i].returnPosition();
				if (bricks[t][i].returnState() == EXPLODING) {
					bricks[t][i].create(position, bricks[t][i].returnColor(), EMPTY);
				}
				destroy_brick_flag = false;
				screen_shake = 0.0;
				selectionList.clear();
				explosion_finish_flag = false;
			}
	}
}
bool cGame::dropColumn()
{
	bool dropped = false;
	dropCounter = 0;
	for (int x = 0; x < BRICKS_MAP_X; x++)
		for (int y = BRICKS_MAP_Y - 1; y >= 0; y--)
		{
			if (y - 1 >= 0)
				if (bricks[x][y].returnState() == EMPTY && bricks[x][y - 1].returnState() == FULL)
				{
					sPoint position{ static_cast<float>(x),static_cast<float>(y) };
					bricks[x][y].create(position, bricks[x][y - 1].returnColor(), bricks[x][y - 1].returnState());
					position.y--;
					bricks[x][y - 1].create(position, 0, EMPTY);
					dropCounter++;
				}
		}
	if (dropCounter > 0) return true; 
	return false;
}
bool cGame::moveLeft()
{	
	moveCounter = 0;
	for (int x = 0; x < BRICKS_MAP_X; x++)
	{
		int empty = 0;
		for (int y = 0; y < BRICKS_MAP_Y; y++)
		{
			if (bricks[x][y].returnState() == EMPTY)
			{
				empty++;
			}
			if (empty == BRICKS_MAP_Y)
			{
				for (int xx = x; xx < BRICKS_MAP_X; xx++)
				for (int yy = 0; yy < BRICKS_MAP_Y; yy++)
					{
						if (xx + 1 < BRICKS_MAP_X) //avoids going outside of vector
						{
							
							sPoint position = bricks[xx][yy].returnPosition();
							if (bricks[xx + 1][yy].returnState() == FULL)
							{
								bricks[xx][yy].create(position, bricks[xx + 1][yy].returnColor(), MOVING); //bricks[xx + 1][yy].state);
								position.x++;
								bricks[xx + 1][yy].create(position, 0, EMPTY);
								moveCounter++;
							}
						}
					}
			}
		}
	}
	if (moveCounter > 0) return true;
	return false;
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
				int color = bricks[i][t].returnColor();
				int state = bricks[i][t].returnState();
				al_fwrite(save_game, &color, sizeof(int));
				al_fwrite(save_game, &state, sizeof(int));
			}
		al_fclose(save_game);
	}
}
void cGame::loadGame()
{
	ALLEGRO_FILE* save_game = al_fopen("save.bri", "rb");
	if (al_fopen("save.bri", "rb"))
	{
		cScore score;
		al_fread(save_game, &score, sizeof(cScore));
		//on_screen = score;
		for (int i = 0; i < BRICKS_MAP_X; i++)
			for (int t = 0; t < BRICKS_MAP_Y; t++)
			{
				int color;
				int state;
				al_fread(save_game, &color, sizeof(int));
				al_fread(save_game, &state, sizeof(int));
				sPoint position{ static_cast<float>(i),static_cast<float>(t) };
				bricks[i][t].create(position, color, state);
			}
	}
	else
	{
		game_state = REFRESH_GAME;
	}
	al_fclose(save_game);
}
void cGame::drawMenu()
{
	al_draw_bitmap(bitmap->mainPNG, 0, 0, 0);
	for (int i = 0; i < MAX_BUTTONS; i++)	
	{	
		button[i].draw(BUTTON_OVERLAY);	//if flags are set to true then draw overlay showing collision box
	}
	options_menu.draw();
	scores_menu.draw();
	/*for (int i = 0; i < MAX_HIGH_SCORE; i++)
	{
		if (button[SCORES_BUTTON].clicked) menu_items[i].draw(BUTTON_OVERLAY);
	}*/
}

