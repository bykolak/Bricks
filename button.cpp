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
	fadeIn = false;
}

bool cButton::update(sPoint m) //if inside button then change flags to true else make it false
{
	int counter = 0;
	if (type != MENU_ITEM)
	{
	if (upTriangle.pointInTriangle(m) || downTriangle.pointInTriangle(m) )
	{
		mouseOver = true;
	}
	else mouseOver = false;

	
		if (mouseOver) { scale += 0.019; }
		else { scale -= 0.019; }

		if (scale > 1.2) { scale = 1.2; }
		if (scale < 1.0) { scale = 1.0; }
	}
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
void cButton::create(float posX, float posY, int font_size, int _type, const char* text) //sets all button parameters
{
	ALLEGRO_FONT * font = al_load_font("bebas.ttf", font_size, 0);
	x = posX;		y = posY; //sets bitmap position on screen
	type = _type;
	//width=w;
	//height=h;
	scale = 1.0;
	//buttonNotClicked = al_create_bitmap(width, height);
	//buttonClicked = al_create_bitmap(width, height);
	buttonPNG = al_load_bitmap("buttons.png");
	if (type == TEXT_BUTTON)//every clickable text
	{

		width = al_get_text_width(font, text);
		height = al_get_font_line_height(font);
		buttonNotClicked = al_create_bitmap(width, height);
		buttonClicked = al_create_bitmap(width, height);
		sPoint point[3] = { { posX,posY },{ posX + width, posY },{ posX, height + posY } };
		sPoint point2[3] = { { posX + width, posY },{ posX, posY + height },{ posX + width, posY + height } };
		upTriangle.set(point[0], point[1], point[2]);
		downTriangle.set(point2[0], point2[1], point2[2]);
		opacity = 0.0;
		al_set_target_bitmap(buttonNotClicked);
		al_draw_textf(font, WHITE, width / 2, 0, ALLEGRO_ALIGN_CENTRE, text);	
		al_set_target_bitmap(buttonClicked);
		al_draw_textf(font, WHITE, width / 2, 0, ALLEGRO_ALIGN_CENTRE, text);
	}
	if (type == MENU_BUTTON)// diamond menu button
	{
		width = BUTTON_SIZE;
		height = BUTTON_SIZE;
		buttonNotClicked = al_create_bitmap(width, height);
		buttonClicked = al_create_bitmap(width, height);
		fadeIn = true;
		sPoint point[3] = { { posX, posY + height / 2 },{ posX + width / 2, posY },{ posX + width, posY + height / 2 } };
		sPoint point2[3] = { { posX + width, posY + height / 2 },{ posX, posY + height / 2 },{ posX + width / 2, posY + height } };
		upTriangle.set(point[0], point[1], point[2]);
		downTriangle.set(point2[0], point2[1], point2[2]);
		opacity = 1.0;
		al_set_target_bitmap(buttonNotClicked);//start drawing on not cliked button sprite
		al_draw_bitmap_region(buttonPNG, 0, 0, width, height, 0, 0, NULL);
		al_draw_textf(font, WHITE, BUTTON_SIZE / 2, BUTTON_SIZE / 2 -(al_get_font_line_height(font)/2), ALLEGRO_ALIGN_CENTRE, text);
		
		al_set_target_bitmap(buttonClicked);//start drawing on clicked button sprite
		al_draw_bitmap_region(buttonPNG, 0, 0, width, height, 0, 0, NULL);
		al_draw_textf(font, BUTTON, BUTTON_SIZE / 2, BUTTON_SIZE / 2 - (al_get_font_line_height(font) / 2), ALLEGRO_ALIGN_CENTRE, text); // (36 / 2) + (1/2 *18)=27 <---magic number	
	}
	if (type == MASTER_VOLUME)
	{
		width = 0;//temp
		height = 0;//temp
		buttonNotClicked = al_create_bitmap(width, height);
		buttonClicked = al_create_bitmap(width, height);
		sPoint point[3] = { { posX,posY },{ posX + width, posY },{ posX, height + posY } };
		sPoint point2[3] = { { posX + width, posY },{ posX, posY + height },{ posX + width, posY + height } };
		upTriangle.set(point[0], point[1], point[2]);
		downTriangle.set(point2[0], point2[1], point2[2]);
		//TODO:
		//load master volume bitmap
		
	}
	if (_type == MUTE_BUTTON)//TODO mute button
	{
		width = BRICK_SIZE * BRICKS_MAP_X;
		height = BRICK_SIZE * BRICKS_MAP_Y;
		buttonNotClicked = al_create_bitmap(width, height);
		buttonClicked = al_create_bitmap(width, height);
		sPoint point[3] = { { posX,posY },{ posX + width, posY },{ posX, height + posY } };
		sPoint point2[3] = { { posX + width, posY },{ posX, posY + height },{ posX + width, posY + height } };
		upTriangle.set(point[0], point[1], point[2]);
		downTriangle.set(point2[0], point2[1], point2[2]);
	}
	if (_type == GAME_AREA)
	{
		width = BRICK_SIZE * BRICKS_MAP_X;
		height = BRICK_SIZE * BRICKS_MAP_Y;
		buttonNotClicked = al_create_bitmap(width, height);
		buttonClicked = al_create_bitmap(width, height);
		sPoint point[3] = { { posX,posY },{ posX + width, posY },{ posX, height + posY } };
		sPoint point2[3] = { { posX + width, posY },{ posX, posY + height },{ posX + width, posY + height } };
		upTriangle.set(point[0], point[1], point[2]);
		downTriangle.set(point2[0], point2[1], point2[2]);
	}
	if (type == MENU_ITEM)
	{

		width = al_get_text_width(font, text);
		height = al_get_font_line_height(font);
		buttonNotClicked = al_create_bitmap(width, height);
		buttonClicked = al_create_bitmap(width, height);
		sPoint point[3] = { { posX,posY },{ posX + width, posY },{ posX, height + posY } };
		sPoint point2[3] = { { posX + width, posY },{ posX, posY + height },{ posX + width, posY + height } };

		opacity = 0.0;
		al_set_target_bitmap(buttonNotClicked);
		al_draw_textf(font, WHITE, width / 2, 0, ALLEGRO_ALIGN_CENTRE, text);
		al_set_target_bitmap(buttonClicked);
		al_draw_textf(font, WHITE, width / 2, 0, ALLEGRO_ALIGN_CENTRE, text);
	}
	al_set_target_bitmap(al_get_backbuffer(display)); //sets drawing to screen again
	al_destroy_font(font);
}

void cButton::draw(bool debug)//draw button on screen 
{
	ALLEGRO_COLOR buttonOpacity = al_map_rgba_f(opacity, opacity, opacity, opacity);
	if (clicked)
	{
		al_draw_tinted_scaled_bitmap(buttonClicked, buttonOpacity, 0, 0, width, height, x - ((BUTTON_SIZE*scale - BUTTON_SIZE) / 2), y - ((BUTTON_SIZE*scale - BUTTON_SIZE) / 2), width*scale, height*scale, 0);
	}
	else al_draw_tinted_scaled_bitmap(buttonNotClicked, buttonOpacity, 0, 0, width, height, x - ((BUTTON_SIZE*scale - BUTTON_SIZE) / 2), y - ((BUTTON_SIZE*scale - BUTTON_SIZE) / 2), width*scale, height*scale, 0);
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