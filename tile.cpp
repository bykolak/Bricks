#include "define.h"
#include "button.h"
#include "game.h"

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
	maxFrame = 30;
	animationDelay = 0;
	selected = false;
	screen_shake = 0;
	if (_state == MOVING)						{	distance += BRICK_SIZE;	}
	if (state == EXPLODING || state==SELECTED)	{	isAnimating = true;		}
	if (state == EXPLODING) { curFrame = rand() % MIN_FRAME; }
	posX = x*BRICK_SIZE + distance + screen_shake;
	posY = y*BRICK_SIZE + screen_shake + TOP_MARGIN;
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
	if (brickToComapre.color == color && brickToComapre.state == state)
	{
		return true;
	}
	else return false;
}
void cTile::draw(ALLEGRO_BITMAP * bricksPNG, ALLEGRO_BITMAP * explosionPNG)
{
	if (state!=EMPTY)	{		al_draw_bitmap_region(bricksPNG, color*BRICK_SIZE, 0, BRICK_SIZE, BRICK_SIZE, posX, posY, NULL);	}
	if (animationDelay == 0)
	{
		if (state == SELECTED)		{		al_draw_bitmap_region(bricksPNG, 0, BRICK_SIZE, BRICK_SIZE, BRICK_SIZE, posX, posY, NULL);		}
		if (state == EXPLODING)		{		al_draw_bitmap_region(explosionPNG, 240 * curFrame, 0, 240, 240, posX - (BRICK_SIZE * 2 + BRICK_SIZE / 2), posY - (BRICK_SIZE * 2 + BRICK_SIZE / 2), NULL);		}
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