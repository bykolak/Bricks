#ifndef TILE_H
#define TILE_H

class cBitmaps
{
private:
	
public:
	ALLEGRO_BITMAP * bricksPNG = NULL;
	ALLEGRO_BITMAP * explosionPNG = NULL;
	ALLEGRO_BITMAP * backgroundPNG = NULL;
	ALLEGRO_BITMAP * mainPNG = NULL;
	ALLEGRO_BITMAP * optionsPNG = NULL;
	ALLEGRO_BITMAP * highscorePNG = NULL;
	ALLEGRO_BITMAP * high_scoresTMP = NULL;
	//ALLEGRO_BITMAP * newgamePNG = NULL;
	cBitmaps(); // loads bitmaps
	//~cBitmaps();
};
class cTile 
{
private:

protected:
	
	float posX;
	float posY;
	//float height;
	//float width;
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
	float maskOpacity;
	bool fadeIn;
public:
	
	bool selected;
	void create(sPoint position, int color, int _state);
	int update();
	bool compare(cTile brickToComapre); //if color and state matches return true
	void draw(cBitmaps& bitmap);
	void setAnimationDelay(int delay);
	int returnState();
	int returnColor();
	sPoint returnPosition();
};

class cScore : public cTile
{
private:
	int current; // players score
	int score_earned; //last score
	ALLEGRO_USTR* high_score_name[MAX_HIGH_SCORE];
	ALLEGRO_USTR* player_name = al_ustr_new("_");
	ALLEGRO_USTR* edited_text = al_ustr_new("");
	int high_score[MAX_HIGH_SCORE]; //array that holds all high scores
public:
	bool enterPlayerName(int keycode, int unichar);
	void resetHighScores();//resets and saves new high score file
	bool is_saved; //if high scores already saved
	int on_screen;//shown score
	bool checkSaveScores();//checks highscores & if your score is > than lowest highscore then return true
	void calculateScore(int selectedBricks); //takes selectionList.size and calculates score for destroyed bricks 
	void update();
	void drawHighScores(cGame& game);//cBitmaps bitmap, float opacity);
	void reset();
	void saveHighScore();
	void loadHighScore();
	void saveScores();//
	void drawEnd(); //draw end game screen
	cScore();
};
#endif