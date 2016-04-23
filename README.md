<h1>Bricks 1.0<br></h1>
It`s my first project to make a Bejeweled, Same, Candy Crush like game using c++ and allegro5 library.<br>
**Basic Rules:**

 * [x] You start with a screen full of randomized 6-colored bricks. 
 * [x] You get points by selecting and removing bricks of the same color.
 * [x] You can't remove single bricks.
 * [x] More points are awarded for big chunks of bricks that are removed by a single selection.
 * [x] If only single bricks left or all are removed, you win the game.
 * [x] Bricks are affected by gravity so if you make a hole they will fall down.

**MAIN MENU:**

 * [ ] New Story - loads map[0] from story.bri 
 * [x] Random Game game.newGame(); 
 * [ ] Continue Story game.loadGame(); 
 * [x] Exit Game

**INGAME BUTTONS:**

**"New Game"**

 * [x] Left Click creates new game and resets your current score.
 * [x] Right Click is for debug (creates new map with one colored bricks).

**"High Scores"**

* [ ] List of 10 best scores and usernames. (needs some work)
* [x] Close button
* [x] Reset Score
* [x] High Scores are kept in a binary file called high.bri
* 
**"Score"** - Shows current Score

**"Options"**

 * [ ] Volume Slider - slide between 0% and 100% of volume 
 * [ ] Mute Button - disable sound 
 * [ ] Restart Game - restart map 
 * [x] Game saves automatically. 
 * [ ] There are two files story_save.bri random_save.bri. 

**SPECIAL BLOCKS:**

* [ ] anti-gravity block 
* [ ] undestructable block 
* [ ] exploding block 
* [ ] paint block 
* [ ] line clear block 
* [ ] column clear block 

**FEATURES TODO:**
* [ ] level editor
* [ ] 20 predefined levels





