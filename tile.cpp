#include "define.h"
#include "button.h"
#include "game.h"

//=====cTile methods
void cTile::create(sPoint position, int _color)
{
	posX = position.x*BRICK_SIZE + (SCREEN_X - AREA_WIDTH) / 2;
	posY = position.y*BRICK_SIZE + (SCREEN_Y - AREA_HEIGHT) / 2;
	state = FULL;
	color = _color;
	frameDelay = 0;
	frameCount = 0;
	curFrame = 0;
	maxFrame = 60;
	resetCurFrame = false;
}
void cTile::update()
{
	if (resetCurFrame) { resetCurFrame = false;curFrame=0; }
	if (++frameCount >= frameDelay)
	{
		if (++curFrame >= maxFrame)
		{
			curFrame = 0;
			if (state == EXPLODING)
			{
				//curFrame = maxFrame;
				state = EMPTY;
			}
		}
		frameCount = 0;
	}
	if (state == EXPLODING)
	{
		screen_shake = (rand() % 4) - 2; // -3 to 3 pixels in both directions
	}
}
void cTile::draw(ALLEGRO_BITMAP * bricksPNG, ALLEGRO_BITMAP * explosionPNG)
{
	if (state == FULL || state == SELECTED)
	{
		al_draw_bitmap_region(bricksPNG, color*BRICK_SIZE, 0, BRICK_SIZE, BRICK_SIZE, posX, posY, NULL);
	}
	if (state == SELECTED)
	{
		al_draw_bitmap_region(bricksPNG, 0, BRICK_SIZE, BRICK_SIZE, BRICK_SIZE, posX, posY, NULL);
	}
	if (state == EXPLODING)
	{
		al_draw_bitmap_region(explosionPNG, 240 * curFrame, 0, 240, 240, posX - (BRICK_SIZE*2+BRICK_SIZE/2)+screen_shake, posY - (BRICK_SIZE * 2 + BRICK_SIZE / 2) + screen_shake, NULL); //
		al_draw_textf(font18, BLACK, posX, posY, NULL, "%i", curFrame);
	}
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
