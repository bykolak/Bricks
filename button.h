#ifndef BUTTON_H
#define BUTTON_H

#include <allegro5\allegro.h>
//#include <allegro5\allegro_image.h>
struct sPoint
{
	float x;
	float y;
	int bricksX;
	int bricksY;
};
class cTriangle
{
private:
	sPoint vertex[3];
	bool exist;
public:
	cTriangle();
	float sign(sPoint p1, sPoint p2, sPoint p3);
	bool pointInTriangle(sPoint mouse);
	void set(sPoint p1, sPoint p2, sPoint p3);
	void draw(ALLEGRO_COLOR  color);
};
class cButton
{
	friend class cGame;
private:
	float opacity;
	float scale;
	float x;
	float y;
	cTriangle upTriangle;
	cTriangle downTriangle;
	ALLEGRO_BITMAP * buttonPNG;
	ALLEGRO_BITMAP * buttonClicked;
	ALLEGRO_BITMAP * buttonNotClicked;
	bool mouseOver;
	bool clicked;
	bool fadeIn;
public:
	cButton();//constructor
	bool update(sPoint m); //if inside button then change flags to true else make it false
	void create(float posX, float posY, float width, float height, int type, const char* text); //sets all button presets
	void draw(bool debug);//draw button on screen
};
#endif