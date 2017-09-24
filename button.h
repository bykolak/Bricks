#ifndef BUTTON_H
#define BUTTON_H

#include <allegro5\allegro.h>
#include <vector>


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
//	friend class cGame;
private:
	float opacity;
	float scale;
	float x;
	float y;
	float width;
	float height;
	int type;
	cTriangle upTriangle;
	cTriangle downTriangle;
	ALLEGRO_BITMAP * buttonPNG;
	ALLEGRO_BITMAP * buttonClicked;
	ALLEGRO_BITMAP * buttonNotClicked;
	bool mouseOver;
	bool clicked;
	bool fadeIn;
	ALLEGRO_USTR * text = NULL;
public:
	void fade(bool fade);
	cButton();//constructor
	//float getOpacity();
	void changeClicked(bool click);
	bool isClicked();
	bool isMouseOver();
	bool update(sPoint m); //if inside button then change flags to true else make it false
	void create(float posX, float posY, int font_size, int _type, ALLEGRO_USTR * text); //sets all button presets
	void draw(bool debug);//draw button on screen
};
class cMenu
{
	friend class cScore;
private:
	std::vector<cButton> menu_items;
	cButton temp_button;
	ALLEGRO_USTR * text;
public:
	void fade(bool fade);
	//void create(int menu_type);
	void createOptions();
	void createScores(cScore & _score);
	void draw();
	void update(sPoint m);
};
#endif