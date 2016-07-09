#ifndef CLASSES_H
#define CLASSES_H

#include "define.h"
#include <fstream>
#include <string>
extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_FONT *font18;
extern ALLEGRO_FONT *font24;
extern ALLEGRO_FONT *font36;

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
struct cTile
{
	int color;
	int state;
	bool select=false;
};
struct sPoint 
{
	int x;
	int y;
};
class cTriangle
{
private:
	sPoint vertex[3];
public:
	float sign(sPoint p1, sPoint p2, sPoint p3);
	bool pointInTriangle(sPoint mouse);
	void set(int x0, int y0, int x1, int y1, int x2, int y2);
};
class cButton
{
	friend class cGame;
private:
	float opacity;
	sPoint point0;
	sPoint point1;
	cTriangle upTriangle;
	cTriangle downTriangle;
	cTriangle subMenuTriangleArea;
	sPoint subMenuSquareArea0;
	sPoint subMenuSquareArea1;
	ALLEGRO_BITMAP * buttonPNG;
	//ALLEGRO_BITMAP *buttonBMP;
	//ALLEGRO_BITMAP *buttonPressedBMP;
	int type;
	//int state;
	bool mouseOver;
public:
	cButton();//constructor
	bool update(sPoint m); //if inside button then change flags to true else make it false
	void create(int type); //sets all button presets
	void draw();//draw button on screen
	};
class cGame
{
	friend class cButton;
private:
	sPoint mouse;// contains postion of mouse on screen
	std::vector<cList> selectionList;
	int currently_selected;
	int score; // players score
	int on_screen_score;//shown score
	int score_count;
	//int number_of_selected; //how many selected 
	bool selection; //if something selected
	bool saved_scores; //if high scores already saved
	bool update_position;
	int bricks_on_screen; //total number of bricks on screen
	int area_width;//number of bricks in x axis * brick_size
	int area_height;//number of bricks in y axis * brick_size
	int left_button_margin;//where to start draw top buttons
	int left_game_area_margin;// where to start draw game area
	int screen_shake;// move everything on screen (in pixels)
	int frameCount;
	const int frameDelay = 0;
	int curFrame;
	ALLEGRO_EVENT ev;
	const int maxFrame=40;
	int last_score; //holds value of last destroyed bricks
	int last_clicked_x ;
	int last_clicked_y ;
	int mouseX;
	int mouseY;
public:
//	float opacity;
	ALLEGRO_SAMPLE *explosionOGG;
	ALLEGRO_SAMPLE *clickWAV;
	ALLEGRO_SAMPLE_INSTANCE *instance;
	ALLEGRO_SAMPLE_INSTANCE *instance2;
	ALLEGRO_SAMPLE_INSTANCE *instanceClick;
	ALLEGRO_SAMPLE_INSTANCE *instanceClick2;
	int brick_size; //size of bricks
	int game_state; // using enum GAME_STATE to tell program in what state game is
	bool destroy_brick;
	int screen_width;//screen size x in pixels
	int screen_height;//screen size y in pixels	
	int high_score[MAX_HIGH_SCORE]; //array that holds all high scores
	ALLEGRO_USTR* high_score_name[MAX_HIGH_SCORE];
	cButton button[MAX_BUTTONS];
	ALLEGRO_USTR* player_name = al_ustr_new("_");
	ALLEGRO_USTR* edited_text = al_ustr_new("");
	//ALLEGRO_BITMAP *scoreBMP = NULL;
	ALLEGRO_BITMAP *bricksPNG = NULL;
	//ALLEGRO_BITMAP *optionsBMP = NULL;
	ALLEGRO_BITMAP *shadowBMP = NULL;
	//ALLEGRO_BITMAP *buttonsBMP = NULL;
	ALLEGRO_BITMAP *backgroundBMP = NULL;
	ALLEGRO_BITMAP *explosionBMP = NULL;
	ALLEGRO_BITMAP * mainPNG = NULL;
	ALLEGRO_BITMAP * optionsPNG = NULL;
	ALLEGRO_BITMAP * highscorePNG = NULL;
	ALLEGRO_BITMAP * newgamePNG = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_TIMER *timer2 = NULL;
	std::vector< std::vector<cTile> > bricks;
	cGame(); //default constructor
	void updateScore();//updates on_screen score
	void drawScore();//draws score to the screen
	void checkButtons();//checks if mouse over button
	void clickButtons(int mouseButton);
	void update();
	void loadButton(); //loads button locations from file
	//bool checkGameState(int state);//returns true if state=game_state
	void resetHighScores();//resets and saves new high score file
	//void changeBricksXY(int _x, int _y); //changes map size
	//void changeBrickSize(int x);//changes brick size
	void changeTile(int x, int y, int color); //changes color of x,y tile
	void updateNumberOfSelected(); //checks and update number of selected brcisk
	void drawGameArea(); // draw all bricks on screen
	void newGame(bool debug); // restart game
	void checkEndGame(); //checks if game ended (no more bricks to destroy)
	//void highScores(); // draw high scores;
	//void options(); // draw options screen;
	void endGame(); //draw end game screen
	void selectBrick(int _mouse_x, int _mouse_y); // takes mouse input and selects all same color bricks that are connected to  Brick[x][y]
	//int checkNeighbourBrick(int x, int y);  //selects neighbouring bricks
	void deselectBrick(); // clears selection of bricks 
	void destroyBrick(); // after clicking selected bricks destroys them
	int calculateScore(); //calculates score for destroyed bricks
	void dropBrick(); //after destroying bricks fill holes by dropping them (checks from bottom)
	void moveBrickLeft();//if empty vertical line then move rest of screen to the left
	bool checkSaveScores();//checks highscores & if your score is > than lowest highscore then return true
	void saveScores();//
	void enterPlayerName(int keycode, int unichar); 
	void saveHighScore();
	void loadHighScore();
	void saveGame();
	void loadGame();
	void drawMenu();
};


#endif