#include "define.h"
#include "button.h"
#include "game.h"

//=====cScore methods
void cScore::resetHighScores() //resets all saved highscores TODO
{
	for (int i = 0; i < MAX_HIGH_SCORE; i++)
	{
		high_score[i] = 0;
		al_ustr_free(high_score_name[i]);
		high_score_name[i] = al_ustr_new(" ");
		high_score[i] = 0;
	}
	saveHighScore();
}
void cScore::drawEnd()
{
	/*needs rework*/
	//al_draw_bitmap(highscorepng, screen_width / 2 - (al_get_bitmap_width(highscorepng) / 2), screen_height / 2 - (al_get_bitmap_height(highscorepng) / 2), 0);
	//for (int i = 0; i < max_high_score; i++)
	//{
	//	al_draw_textf(font18, white, screen_width / 2 - (al_get_bitmap_width(highscorepng) / 2)+70, (28 * i) + 80+screen_height / 2 - (al_get_bitmap_height(highscorepng) / 2), allegro_align_centre, "1.");
	//	al_draw_ustr(font18, white, screen_width / 2 - (al_get_bitmap_width(highscorepng) / 2)+80, (28 * i) + 80+screen_height / 2 - (al_get_bitmap_height(highscorepng) / 2), allegro_align_left, high_score_name[i]);
	//	al_draw_textf(font18, white, screen_width / 2 - (al_get_bitmap_width(highscorepng) / 2)+360, (28 * i) + 80+screen_height / 2 - (al_get_bitmap_height(highscorepng) / 2), allegro_align_centre, "points: %i", high_score[i]);
	//}
	//if (checksavescores())
	//{
		int x = al_get_display_width(display) / 2;
		int y = al_get_display_height(display) / 2;
		al_draw_textf(font18, WHITE, x, y, ALLEGRO_ALIGN_CENTRE, "enter your name:");
		al_draw_ustr(font18, WHITE, x, y+325, ALLEGRO_ALIGN_CENTRE, edited_text);
	//}
		al_draw_textf(font36, WHITE, x, y + 200, ALLEGRO_ALIGN_CENTRE, "high score");
		al_draw_textf(font24, WHITE, x, y + 250, ALLEGRO_ALIGN_CENTRE, "you scored %i points !", on_screen);
}
void cScore::calculateScore(int selectedBricks) //calculates score for destroyed bricks
{
	score_earned = 0;
	//int iterator = selectionList.size();
	for (int i = 0; i<selectedBricks; i++)
	{
		score_earned += i * 2; //NEEDS EVALUATION
	}
	current += score_earned;
}
void cScore::update()
{
	//updates on_screen score (needs rework)
	if (current / 1000 > 0)on_screen += 1000;
	if (current / 100 > 0)on_screen += 100;
	if (current / 10 > 0)on_screen += 10;
	if (current / 1 > 0)on_screen += 1;
	if (on_screen > current) on_screen = current;
}
void cScore::drawHighScores(cGame& game)
{
	/* DRAWING NEEDS REWORK */
	/*int shake = selectionList.size() / 5;
	if (selectionList.size()>100) shake = 20;
	if (shake == 0) shake = 1;
	int f = rand() % shake;
	al_draw_textf(font36, BLACK, (mouse.bricksX *BRICK_SIZE) + (BRICK_SIZE / 2) - 2 + f, (mouse.bricksY *BRICK_SIZE) + TOP_MARGIN + f, NULL, "%i", last_score);
	al_draw_textf(font36, BLACK, (mouse.bricksX *BRICK_SIZE) + (BRICK_SIZE / 2) + 2 + f, (mouse.bricksY *BRICK_SIZE) + TOP_MARGIN + f, NULL, "%i", last_score);
	al_draw_textf(font36, WHITE, (mouse.bricksX *BRICK_SIZE) + (BRICK_SIZE / 2) + f, (mouse.bricksY *BRICK_SIZE) + TOP_MARGIN + f, NULL, "%i", last_score);*/
	//draw highscores
	float highX = al_get_bitmap_width(game.bitmap->highscorePNG);
	float highY = al_get_bitmap_height(game.bitmap->highscorePNG);

	game.bitmap->high_scoresTMP = al_create_bitmap(highX, highY); //creates empty bitmap of highscoresPNG size
	al_set_target_bitmap(game.bitmap->high_scoresTMP);
	/*cButton * button = NULL;
	button = &game.button[HIGHSCORES_POPUP];
	button -*/
	al_draw_bitmap(game.bitmap->highscorePNG, 0, 0, NULL);
	for (int i = 0; i < MAX_HIGH_SCORE; i++)
	{
		al_draw_textf(font18, WHITE, 70, (28 * i) + 80, ALLEGRO_ALIGN_CENTRE, "1.");
		al_draw_ustr(font18, WHITE, 80, (28 * i) + 80, ALLEGRO_ALIGN_LEFT, high_score_name[i]);
		al_draw_textf(font18, WHITE, 360, (28 * i) + 80, ALLEGRO_ALIGN_CENTRE, "Points: %i", high_score[i]);
	}
	al_set_target_bitmap(al_get_backbuffer(display));
	float opacity = 0.0;
	if (game.game_state == SAVING_SCORE) 
	{
		opacity = 1.0;
	}
	else if (game.game_state == MAIN_MENU)
	{
		opacity=game.button->getOpacity();

	}
	float screen_width = al_get_display_width(display);
	float screen_height = al_get_display_height(display);
	al_draw_tinted_scaled_bitmap(game.bitmap->high_scoresTMP, al_map_rgba_f(opacity, opacity,opacity,opacity),0, 0, highX, highY, screen_width - screen_width / 3, screen_height / 4, highX, highY, 0);
	//float opacity = 
	//al_draw_tinted_scaled_bitmap(game.bitmap->high_scoresTMP, al_map_rgba_f(game. bopacity,opacity,opacity,opacity),	0, 0, highX, highY, SCREEN_X - SCREEN_X / 3, SCREEN_Y / 4, highX, highY, 0);// temp fix al_map_rgba_f needs to takes values from button instead of temp int in drawMenu()
	al_destroy_bitmap(game.bitmap->high_scoresTMP);//always destroy temp bitmaps
}
void cScore::reset()
{
	current = 0;
	on_screen = 0;
	is_saved = false;
	player_name = NULL;
}
cScore::cScore()
{
	current = 0;
	on_screen = 0;
	player_name = NULL;
}
bool cScore::checkSaveScores()//checks highscores & if your score is > than lowest highscore then prompts for username and saves it to file
{
	if (!is_saved)
	{
		int i;
		int t = 0;
		for (i = 0; i<MAX_HIGH_SCORE; i++)
		{
			if (current>high_score[i]) { t++; }
		}
		if (t > 0) return true;
		else return false;
	}
	return false;
}
void cScore::saveScores()
{
	int i = 0;
	int t = 0;

	if (!is_saved)
	{
		for (i = 0; i<MAX_HIGH_SCORE; i++)
		{
			if (current>high_score[i])
			{
				for (t = MAX_HIGH_SCORE - 1; t > i; t--)
				{

					high_score[t] = high_score[t - 1];
					high_score_name[t] = high_score_name[t - 1];
				}
				high_score[i] = current;
				high_score_name[i] = player_name;
				player_name = al_ustr_new("");
				edited_text = al_ustr_new("");
				//		game_state = END_GAME;
				is_saved = true;
				i = MAX_HIGH_SCORE + 1;
			}
		}
	}
	saveHighScore();
}
void cScore::enterPlayerName(int keycode, int unichar)
{
	if (keycode <= ALLEGRO_KEY_9 || keycode == ALLEGRO_KEY_SPACE) //if A-Z and 0-9 pressed or spacebar
		if (al_ustr_length(edited_text) < MAX_USERNAME_LENGTH) al_ustr_append_chr(edited_text, unichar);

	if (keycode == ALLEGRO_KEY_BACKSPACE)
	{
		if (al_ustr_length(edited_text) > 0) { al_ustr_truncate(edited_text, al_ustr_length(edited_text) - 1); }
	}
	if (keycode == ALLEGRO_KEY_ENTER || keycode == ALLEGRO_KEY_PAD_ENTER)
	{
		player_name = edited_text;
		saveScores();
		//game_state = MAIN_MENU;
	}
}
void cScore::saveHighScore()
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
void cScore::loadHighScore()
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
//=====cTile methods
void cTile::create(sPoint position, int _color,int _state)
{
	x = position.x;
	y = position.y;
	state = _state;
	isAnimating = false;
	color = _color;
	frameDelay = 0;
	frameCount = 0;
	curFrame = 0;
	maxFrame = 60;
	animationDelay = 0;
	selected = false;
	screen_shake = 0;
	if (_state == MOVING)						{	distance += BRICK_SIZE;	}
	if (state == EXPLODING || state==SELECTED)	{	isAnimating = true;		}
	if (state == EXPLODING) { curFrame = rand() % MIN_FRAME; }
	posX = x*BRICK_SIZE + distance + screen_shake;
	posY = y*BRICK_SIZE + screen_shake + TOP_MARGIN;
	//selected = false;
}
int cTile::update()
{
	int exit_code = 0;
	if(isAnimating)
	{
		animationDelay--;
		if (animationDelay < 0) animationDelay = 0;
	}
	if (isAnimating && animationDelay==0)
	{
		if (++frameCount >= frameDelay)
		{
			if (++curFrame >= maxFrame)
			{
				curFrame = 0;
				if (state == EXPLODING)
				{
					state = EMPTY;
					isAnimating = false;
				}
			}
			frameCount = 0;
		}
	}
	if (state == EXPLODING || state == SELECTED)
	{
		screen_shake = (rand() % 2) - 1; // -3 to 3 pixels in both directions
	}
	else { screen_shake = 0.0; }

	if (state == MOVING)
	{
		distance-=10.5;
		if (distance <= 0.0) { state = FULL; distance = 0.0; }
	}
	posX = x*BRICK_SIZE + distance + screen_shake;
	posY = y*BRICK_SIZE + screen_shake + TOP_MARGIN;
	return exit_code;
}
bool cTile::compare(cTile brickToComapre)
{
	if (brickToComapre.color == color && brickToComapre.state == state && !brickToComapre.selected)
	{
		return true;
	}
	else return false;
}
void cTile::draw(cBitmaps& bitmap)
{
	if (state!=EMPTY)	
	{	
		if (state == EXPLODING && animationDelay > 0)
		{
			al_draw_bitmap_region(bitmap.bricksPNG, color*BRICK_SIZE, 0, BRICK_SIZE, BRICK_SIZE, posX, posY, NULL);
		}
		else if (state != EXPLODING)
		{
			al_draw_bitmap_region(bitmap.bricksPNG, color*BRICK_SIZE, 0, BRICK_SIZE, BRICK_SIZE, posX, posY, NULL);
		}
	}
	if (animationDelay == 0)
	{
		if (state == SELECTED)		{		al_draw_bitmap_region(bitmap.bricksPNG, 0, BRICK_SIZE, BRICK_SIZE, BRICK_SIZE, posX, posY, NULL);		}
		if (state == EXPLODING)		{		al_draw_bitmap_region(bitmap.explosionPNG, 240 * curFrame, 0, 240, 240, posX - (BRICK_SIZE * 2 + BRICK_SIZE / 2), posY - (BRICK_SIZE * 2 + BRICK_SIZE / 2), NULL);		}
	}
	//al_draw_textf(font18, BLACK, posX, posY, NULL, "%i", curFrame); //DEBUG: shows curFrame on every brick
	//if (distance>0)	al_draw_textf(font18, BLACK, posX, posY, NULL, "%i", distance); //DEBUG: shows distance added to posX	
	//	al_draw_textf(font18, BLACK, posX, posY, NULL, "%i", distance); //DEBUG: shows distance added to posX
	//	al_draw_textf(font18, BLACK, posX, posY+20, NULL, "%i", state); //DEBUG: shows distance added to posX
	//al_draw_textf(font18, BLACK, posX, posY + 20, NULL, "%i", animationDelay);//DEBUG: shows animationDelay on every brick
}
void cTile::setAnimationDelay(int delay)
{
	animationDelay = delay;
}

int cTile::returnState()
{
	return state;
}

int cTile::returnColor()
{
	return color;
}

sPoint cTile::returnPosition()
{
	sPoint position{ static_cast<float>(x),static_cast<float>(y),x,y };
	return position;
}

cBitmaps::cBitmaps()
{
	bricksPNG = al_load_bitmap("bricks.png");
	explosionPNG = al_load_bitmap("explosion.png");
	backgroundPNG = al_load_bitmap("background.png");
	mainPNG = al_load_bitmap("main.png");
	optionsPNG = al_load_bitmap("options.png");
	highscorePNG = al_load_bitmap("highscore.png");
}

//cBitmaps::~cBitmaps()
//{
//	al_destroy_bitmap(bricksPNG);
//	al_destroy_bitmap(explosionPNG);
//	al_destroy_bitmap(backgroundPNG);
//	al_destroy_bitmap(mainPNG);
//	al_destroy_bitmap(optionsPNG);
//	al_destroy_bitmap(highscorePNG);
//}

//void cBitmaps::load()
//{
//	
//}

