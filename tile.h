#ifndef TILE_H
#define TILE_H


class cList
{
public:
	bool state;
	int x;
	int y;
	cList();
	cList(int _x, int _y, bool _state);//constructor
	void setState(bool _state);
	void set(int _x, int _y, bool _state);
};
class cTile : public cList
{
private:
	float posX;
	float posY;
	float height;
	float width;
	float screen_shake;
	float distance;
	int curFrame;
	int frameCount;
	int frameDelay;
	int maxFrame;
	int animationDelay;
	bool isAnimating;
	

public:
	bool isMoving;
	bool selected;
	int color;
	int state;
	//bool select = false;
	void create(sPoint position, int color, int _state);
//	void create(cTile brick);
	int update();
	bool compare(cTile brickToComapre); //if color and state matches return true
	void draw(ALLEGRO_BITMAP * bricksPNG, ALLEGRO_BITMAP * explosionPNG);
	void setAnimationDelay(int delay);
	void move();
};


#endif