#include "button.h"
#include <allegro5\allegro_primitives.h>
#include "define.h"
#include <allegro5\allegro_ttf.h>
#include "game.h"
cTriangle::cTriangle()
{
	exist = false;
}
float cTriangle::sign(sPoint p1, sPoint p2, sPoint p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

void cTriangle::set(sPoint p1, sPoint p2, sPoint p3)
{
	exist = true;
	vertex[0] = p1;
	vertex[1] = p2;
	vertex[2] = p3;
}
void cTriangle::draw(ALLEGRO_COLOR color)
{
	if (exist)
		al_draw_triangle(vertex[0].x, vertex[0].y, vertex[1].x, vertex[1].y, vertex[2].x, vertex[2].y, color, 1);
}
bool cTriangle::pointInTriangle(sPoint mouse)
{
	bool b1, b2, b3;

	b1 = sign(mouse, vertex[0], vertex[1]) < 0.0f;
	b2 = sign(mouse, vertex[1], vertex[2]) < 0.0f;
	b3 = sign(mouse, vertex[2], vertex[0]) < 0.0f;
	if (exist)
	{
		return ((b1 == b2) && (b2 == b3));
	}

	return false;
}
//=====cButton methods
cButton::cButton()//constructor
{
	mouseOver = false;
	clicked = false;
	opacity = 1.0;
	scale = 1.0;
	buttonPNG = NULL;
	fadeIn = true;
}
float cButton::getOpacity()
{
	return opacity;
}
bool cButton::update(sPoint m) //if inside button then change flags to true else make it false
{
	int counter = 0;

	if (upTriangle.pointInTriangle(m) || downTriangle.pointInTriangle(m))
	{
		mouseOver = true;
	}
	else mouseOver = false;

	if (mouseOver) { scale += 0.019; }
	else { scale -= 0.019; }

	if (scale > 1.2) { scale = 1.2; }
	if (scale < 1.0) { scale = 1.0; }

	if (fadeIn)
	{
		opacity += OPACITY_INCREASE;
	}
	else
	{
		opacity -= OPACITY_INCREASE;
	}
	if (opacity > 1.0) { opacity = 1.0; }
	if (opacity < 0.0) { opacity = 0.0; }
	return mouseOver;
}
void cButton::create(float posX, float posY, float width, float height, int type, const char* text) //sets all button parameters
{

	if (type == RECTANGLE)//rectangle
	{
		sPoint point[3] = { { posX,posY },{ posX + width, posY },{ posX, height + posY } };
		sPoint point2[3] = { { posX + width, posY },{ posX, posY + height },{ posX + width, posY + height } };
		upTriangle.set(point[0], point[1], point[2]);
		downTriangle.set(point2[0], point2[1], point2[2]);
	}
	if (type == RHOMB)//rhomb
	{
		sPoint point[3] = { { posX, posY + height / 2 },{ posX + width / 2, posY },{ posX + width, posY + height / 2 } };
		sPoint point2[3] = { { posX + width, posY + height / 2 },{ posX, posY + height / 2 },{ posX + width / 2, posY + height } };
		upTriangle.set(point[0], point[1], point[2]);
		downTriangle.set(point2[0], point2[1], point2[2]);
	}
	if (type == TRIANGLE)//triangle
	{
		sPoint point[3] = { { posX + width, posY + height / 2 },{ posX, posY + height / 2 },{ posX + width / 2, posY + height } };
		sPoint point2[3] = { { posX + width, posY + height / 2 },{ posX, posY + height / 2 },{ posX + width / 2, posY + height } };
		upTriangle.set(point[0], point[1], point[2]);
		downTriangle.set(point2[0], point2[1], point2[2]);
	}
	buttonPNG = al_load_bitmap("buttons.png");
	buttonNotClicked = al_create_bitmap(BUTTON_SIZE, BUTTON_SIZE);
	buttonClicked = al_create_bitmap(BUTTON_SIZE, BUTTON_SIZE);

	//al_set_target_bitmap(buttonPNG);//draw on button sprite

	if (type != GAME_AREA) //if not game area create button bitmaps 
	{
		al_set_target_bitmap(buttonNotClicked);//draw on button sprite
		int horizontal_position = 0;
		if (type == TRIANGLE) { horizontal_position = -18; }
		else horizontal_position = 27;
		al_draw_bitmap_region(buttonPNG, type*BUTTON_SIZE, 0, BUTTON_SIZE, BUTTON_SIZE, 0, 0, NULL);
		al_draw_textf(font36, WHITE, BUTTON_SIZE / 2, BUTTON_SIZE / 2 - horizontal_position, ALLEGRO_ALIGN_CENTRE, text); //font size is 36 / 2 = 18 + (1/2 *18)=27 <---magic number

		al_set_target_bitmap(buttonClicked);//draw on button sprite
		al_draw_bitmap_region(buttonPNG, type*BUTTON_SIZE, 0, BUTTON_SIZE, BUTTON_SIZE, 0, 0, NULL);
		al_draw_textf(font36, BUTTON, BUTTON_SIZE / 2, BUTTON_SIZE / 2 - horizontal_position, ALLEGRO_ALIGN_CENTRE, text); // (36 / 2) + (1/2 *18)=27 <---magic number
		al_set_target_bitmap(al_get_backbuffer(display)); //sets drawing to screen again
	}

	if (type == GAME_AREA)
	{
		sPoint point[3] = { { posX, posY },{ posX + width, posY },{ posX, height + posY } };
		sPoint point2[3] = { { posX + width, posY },{ posX, posY + height },{ posX + width, posY + height } };
		upTriangle.set(point[0], point[1], point[2]);
		downTriangle.set(point2[0], point2[1], point2[2]);
	}
	buttonPNG = al_load_bitmap("highscores.png");
	if (type == HIGHSCORES_END)
	{

	}
	x = posX;		y = posY; //sets bitmap position on screen
}

void cButton::draw(bool debug)//draw button on screen 
{

	if (clicked)
	{
		al_draw_tinted_scaled_bitmap(buttonClicked, al_map_rgba_f(opacity, opacity, opacity, opacity), 0, 0, BUTTON_SIZE, BUTTON_SIZE, x - (((BUTTON_SIZE*scale) - BUTTON_SIZE) / 2), y - (((BUTTON_SIZE*scale) - BUTTON_SIZE) / 2), BUTTON_SIZE*scale, BUTTON_SIZE*scale, 0);
	}
	else al_draw_tinted_scaled_bitmap(buttonNotClicked, al_map_rgba_f(opacity, opacity, opacity, opacity), 0, 0, BUTTON_SIZE, BUTTON_SIZE, x - (((BUTTON_SIZE*scale) - BUTTON_SIZE) / 2), y - (((BUTTON_SIZE*scale) - BUTTON_SIZE) / 2), BUTTON_SIZE*scale, BUTTON_SIZE*scale, 0);

	if (debug) //if debug overlay is active
	{
		if (mouseOver)
		{
			upTriangle.draw(GREEN);
			downTriangle.draw(GREEN);
		}
		if (!mouseOver)
		{
			upTriangle.draw(RED);
			downTriangle.draw(RED);
		}
	}
}