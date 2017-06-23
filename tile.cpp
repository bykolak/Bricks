#include "define.h"
#include "button.h"
#include "game.h"

//=====cTile methods
void cTile::create(sPoint position, int _color,int _state)
{
	x = position.x;
	y = position.y;
	//distance = 0;
	
	state = _state;
	isAnimating = false;
	
	color = _color;
	frameDelay = 0;
	frameCount = 0;
	curFrame = 0;
	maxFrame = 30;
	animationDelay = 0;
	selected = false;
	if (_state == MOVING)
	{
		distance += BRICK_SIZE;
	//	isMoving = true;
	}
	else if (state==EMPTY || state==EXPLODING || state==SELECTED)
	{
	//	distance = 0;
	}
	if (state == EXPLODING || state==SELECTED)
	{
		isAnimating = true;
	}
	posX = position.x*BRICK_SIZE + distance;
	posY = position.y*BRICK_SIZE + (SCREEN_Y - AREA_HEIGHT) / 2;
	//	animationDelay = 0;
//	}
//	if (state == SELECTED)
//	{
//		curFrame = 0;
//		isAnimating = true;
//		animationDelay = 0;
//	}	
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
	if (state == EXPLODING)
	{
		screen_shake = (rand() % 4) - 2; // -3 to 3 pixels in both directions
	}
	/*if (isMoving && distance > 0)	
	{		
		distance-=2;
	}*/
	if (state == MOVING)
	{
		distance-=2.3;
		if (distance<=0.0)
		{
			distance = 0.0;
			state = FULL;
			//isMoving = false; distance = 0;
		}
	}
	posX = x*BRICK_SIZE + distance;
	posY = y*BRICK_SIZE + (SCREEN_Y - AREA_HEIGHT) / 2;
	//if (animationDelay>0)exit_code = DELAYED;
	//if (animationDelay==0)exit_code = ANIMATING;
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
	if (state!=EMPTY)
	{
		al_draw_bitmap_region(bricksPNG, color*BRICK_SIZE, 0, BRICK_SIZE, BRICK_SIZE, posX, posY, NULL);
	}
	if (animationDelay == 0)
	{
		if (state == SELECTED)
		{
			al_draw_bitmap_region(bricksPNG, 0, BRICK_SIZE, BRICK_SIZE, BRICK_SIZE, posX, posY, NULL);
		}
		if (state == EXPLODING)
		{
			al_draw_bitmap_region(explosionPNG, 240 * curFrame, 0, 240, 240, posX - (BRICK_SIZE * 2 + BRICK_SIZE / 2) + screen_shake, posY - (BRICK_SIZE * 2 + BRICK_SIZE / 2) + screen_shake, NULL);
			//al_draw_textf(font18, BLACK, posX, posY, NULL, "%i", curFrame); //DEBUG: shows curFrame on every brick
		}
		
			

	}
	//if (distance>0)	al_draw_textf(font18, BLACK, posX, posY, NULL, "%i", distance); //DEBUG: shows distance added to posX	
		al_draw_textf(font18, BLACK, posX, posY, NULL, "%i", distance); //DEBUG: shows distance added to posX
		al_draw_textf(font18, BLACK, posX, posY+20, NULL, "%i", state); //DEBUG: shows distance added to posX
	//al_draw_textf(font18, BLACK, posX, posY + 20, NULL, "%i", animationDelay);//DEBUG: shows animationDelay on every brick
}
void cTile::setAnimationDelay(int delay)
{
	animationDelay = delay;
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

