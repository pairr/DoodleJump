#pragma once
#include "Framework.h"
#include <list>
#include "Grid.h"
#include "Bullet.h"
class Doodle
{
private:
	//paths to arts
	const char* path_doodle_left = "C:\\DoodleJump\\Project1\\data\\blue-lik-left@2X.png";
	const char* path_doodle_right = "C:\\DoodleJump\\Project1\\data\\blue-lik-right@2X.png";
	const char* path_doodle_shoot = "C:\\DoodleJump\\Project1\\data\\blue-lik-puca@2X.png";
	const char* path_doodle_left_bunny = "C:\\DoodleJump\\Project1\\data\\bunny-left@2X.png";
	const char* path_doodle_right_bunny = "C:\\DoodleJump\\Project1\\data\\bunny-right@2X.png";
	const char* path_doodle_shoot_bunny = "C:\\DoodleJump\\Project1\\data\\bunny-puca@2X.png";
	const char* path_doodle_nose = "C:\\DoodleJump\\Project1\\data\\jungle-njuska@2X.png";

	//sprites corresponding to arts
	Sprite* sprite_doodle_left;
	Sprite* sprite_doodle_right;
	Sprite* sprite_doodle_shoot;
	Sprite* sprite_doodle_left_bunny;
	Sprite* sprite_doodle_right_bunny;
	Sprite* sprite_doodle_shoot_bunny;
	Sprite* sprite_doodle_nose;

	int posx, posy; // pixel coordinates of Doodle
	int speedx, speedy; // (speed = number of pixels traveled at frame)speedx = speed for x axis, speedy = speed for y axis
	int jumplen; // length of a jump in pixels
	int doodlew, doodleh; // doodlew - width, doodleh - height of current sprite

	Grid *map; // map of the game
	std::list<Bullet*> bullets; // list of bullets shot by doodle


	int last_move; // keeps last move along x axis. IF -1 left; IF 1 right
	int jumping_dist_left; // number of pixels left for the current jump. IF 0 then doodle is falling

	bool jump_boost; // tells whether we have jump boost or not
	unsigned int jump_boost_duration; // jump boost duration
	unsigned int jump_boost_start_time; // jump boost start time
	double boost_factor; // factor of a jump boost

public:
	Doodle(int w, int h); // constructor. parameters: height and width of window
	void shoot(int mousex, int mousey); // shoot bullet in the direction of mouse click
	
	bool update(bool move_left, bool move_right, bool shooting, int mousex, int mousey);//update the state of the doodle depending on parameters
																						//returns game over (true or false)

	void draw(bool shooting, bool move_left, bool move_right);//draws depending on the state of parameters
	~Doodle();
};

