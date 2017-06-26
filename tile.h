#ifndef TILE_H
#define TILE_H

class cTile 
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
	int x;
	int y;
	bool selected;
	int color;
	int state;
	void create(sPoint position, int color, int _state);
	int update();
	bool compare(cTile brickToComapre); //if color and state matches return true
	void draw(ALLEGRO_BITMAP * bricksPNG, ALLEGRO_BITMAP * explosionPNG);
	void setAnimationDelay(int delay);
};


#endif