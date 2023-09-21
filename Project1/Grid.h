#pragma once
#include "Framework.h"
#include "frame.h"
#include <list>
using namespace std;
struct Tile
{
	int posx, posy;
	bool destroyable; // destroyable tile or not
};
struct Enemy
{
	int posx, posy;
};
struct Jump_booster
{
	int posx, posy;
};
struct Black_hole
{
	int posx, posy;
};
class Grid
{
private:
	list<Tile*> tiles; //list of tiles
	list<Enemy*> enemies; //list of enemies
	list<Jump_booster*> jump_boosters; //list of jump_boosters
	list<Black_hole*> black_holes; // list of black_holes

	int tilew, tileh; // tile width and height
	int enemyw, enemyh; // enemy width and height
	int jumpw, jumph; // jumpbooster width and height
	int holew, holeh; // black_hole wisth and height
	int numw, numh; // width and height of digits of score and tile_score

	double prob_enemy, prob_jump, prob_hole, tile_destr_prob; // probability enemy, jumpbooster, hole appearing on tile
												  //  probability of tile being destructable 

	int black_hole_gravity_power; // gravity power of a black hole in pixels along X axis
	int black_hole_effect_dist; // distance on which black hole's gravity has effect

	int height, width; // hegiht and width of game map
	int stop_height; // limiting point of doodle's jumps
	int max_dist; // max distance between tiles along y axis. Dictated by doodle's jump hight
	int score; // SCORE = how many pixels map moved
	int tile_score; // tile_score = how many times bounced from tile or enemy

	const char* path_tile = "C:\\DoodleJump\\Project1\\data\\set.png";
	const char* path_background = "C:\\DoodleJump\\Project1\\data\\space-bck@2x.png";
	const char* path_enemy = "C:\\DoodleJump\\Project1\\data\\doodlestein-left@2x.png";
	const char* path_jump_booster = "C:\\DoodleJump\\Project1\\data\\ach-egg-hunter-50@2x.png";
	const char* path_destr_tile = "C:\\DoodleJump\\Project1\\data\\done-on.png";
	const char* path_black_hole = "C:\\DoodleJump\\Project1\\data\\hole@2x.png";

	const char* path_zero = "C:\\DoodleJump\\Project1\\data\\Number0 7x10.png";
	const char* path_one = "C:\\DoodleJump\\Project1\\data\\Number1 7x10.png";
	const char* path_two = "C:\\DoodleJump\\Project1\\data\\Number2 7x10.png";
	const char* path_three = "C:\\DoodleJump\\Project1\\data\\Number3 7x10.png";
	const char* path_four = "C:\\DoodleJump\\Project1\\data\\Number4 7x10.png";
	const char* path_five = "C:\\DoodleJump\\Project1\\data\\Number5 7x10.png";
	const char* path_six = "C:\\DoodleJump\\Project1\\data\\Number6 7x10.png";
	const char* path_seven = "C:\\DoodleJump\\Project1\\data\\Number7 7x10.png";
	const char* path_eight = "C:\\DoodleJump\\Project1\\data\\Number8 7x10.png";
	const char* path_nine = "C:\\DoodleJump\\Project1\\data\\Number9 7x10.png";

	Sprite* sprite_tile;
	Sprite* sprite_background;
	Sprite* sprite_enemy;
	Sprite* sprite_jump_booster;
	Sprite* sprite_destr_tile;
	Sprite* sprite_black_hole;

	Sprite* numbers[10];

	void create(void); // create tiles, enemies, jump_boosters. Generate for 2 * height - 1 pixels long map.

	void draw_score();
	void draw_tiles();
	void draw_enemies();
	void draw_jump_boosters();
	void draw_black_holes();
public:
	void draw();

	Grid(int w, int h, int f, int m);

	void update(int steps); // shift map by "steps" pixels 

	int get_height();
	int get_width();
	int get_stop_height(void);

	void first_tile(int& x, int& y, int doodlew); // place doodle on the bottom(first) tile

	bool collide_enemy(RECT d); // checks collisions with enemies
	bool collide_jump_booster(RECT d); // checks collisions with jump_boosters
	bool collide_back_hole(RECT d); // checks collisions with black_holes

	int gravitate_black_hole(RECT d); // check for any visible black holes and return gravity power

	bool bounce_enemy(RECT d, int speedy); // checks if should bounce of some enemy
	bool bounce_tile(RECT d, int speedy); // checks if should bounce of some tile
	~Grid();
};

