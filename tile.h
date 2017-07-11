#ifndef TILE_H
#define TILE_H


class cTile 
{
private:

protected:
	
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
	int state;
	int color;
	int x;
	int y;
public:
	
	bool selected;
	void create(sPoint position, int color, int _state);
	int update();
	bool compare(cTile brickToComapre); //if color and state matches return true
	void draw(ALLEGRO_BITMAP * bricksPNG, ALLEGRO_BITMAP * explosionPNG);
	void setAnimationDelay(int delay);
	int returnState();
	int returnColor();
	sPoint returnPosition();
};

class cScore : public cTile
{
private:
	int score; // players score
	
public:
	int on_screen_score;//shown score
	int calculateScore(int selectedBricks); //takes selectionList.size and calculates score for destroyed bricks 
	void update();
	cScore();
};
#endif