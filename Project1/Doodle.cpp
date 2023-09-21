#include "Doodle.h"
#include "Framework.h"
#include <list>
#include "Bullet.h"
#include "Grid.h"
#include <math.h>
using namespace std;

Doodle::Doodle(int w, int h)
{
	sprite_doodle_left = createSprite(path_doodle_left);
	sprite_doodle_right = createSprite(path_doodle_right);
	sprite_doodle_shoot = createSprite(path_doodle_shoot);
	sprite_doodle_left_bunny = createSprite(path_doodle_left_bunny);
	sprite_doodle_right_bunny = createSprite(path_doodle_right_bunny);
	sprite_doodle_shoot_bunny = createSprite(path_doodle_shoot_bunny);
	sprite_doodle_nose = createSprite(path_doodle_nose);

	last_move = -1; // at the start dodle looks left
	getSpriteSize(sprite_doodle_left, doodlew, doodleh);

	speedx = 2; speedy = 1;
	jumplen = 225; 
	jumping_dist_left = jumplen; // at the start doodle is jumping
	
	map = new Grid(w, h, 2 * h / 5, 1 * jumplen / 5); // create map. "focus" = 2 * h / 5. max dist along y axis between objects = 1 * jumplen / 5.
	
	map->first_tile(posx, posy, doodlew); // place doodle on a first tile

	jump_boost_duration = 5 * 1000;
	jump_boost = false;
	boost_factor = 2.5;
	jump_boost_start_time = getTickCount();
	
}

void Doodle::shoot(int mousex, int mousey)
{
	//spawn point of a bullet: on the top of doodle in the middle
	double fromx = posx + (double)doodlew / 2;
	double fromy = posy;
	
	double length = sqrt((double)(mousex - fromx) * (mousex - fromx) + (double)(mousey - fromy) * (mousey - fromy));
	
	if (length < 1e-10)return;

	//calculating unit vector of a shot
	double deltax = (double)(mousex - fromx) / length;
	double deltay = (double)(mousey - fromy) / length;

	int w = map->get_width();
	int h = map->get_height();

	//creating and placing bullet in list
	bullets.push_back(new Bullet(fromx, fromy, deltax, deltay, w, h));
}
bool Doodle::update(bool move_left, bool move_right, bool shooting, int mousex, int mousey)
{
	int stop_height = map->get_stop_height(); // stop_height = doodle can't go above that point
	int w = map->get_width();
	int h = map->get_height();

	RECT d = { posx, posy, posx + doodlew, posy + doodleh }; // rectangle containing doodle sprite

	if (move_left)posx -= speedx; // move left
	if (move_right)posx += speedx; // move right
	
	posx += map->gravitate_black_hole(d); // gravity towards black holes
	
	posx = (posx + w) % w; // If the player goes out the left or right side, they appear on the opposite side
	
	if (shooting) 
	{
		shoot(mousex, mousey); // shooting bullets 
	}

	for (auto it = bullets.begin(); it != bullets.end();)
	{
		(*it)->update(map); // updating bullet's state
		if ((*it)->isdead()) // if bullet is dead (out of the window or killed enemy)
		{
			delete (*it);
			it = bullets.erase(it);
		}
		else it++;
	}

	double factor = 1;
	if (jump_boost) // if have jump boost
	{
		factor = boost_factor; // set the jump factor
		unsigned int time_now = getTickCount(); // time_now
		unsigned int elapsed_milliseconds = time_now - jump_boost_start_time;//milliseconds elapsed since acquiring jump boost

		if (elapsed_milliseconds >= jump_boost_duration) {
			jump_boost = false;
		}
	}

	if (jumping_dist_left > 0)
	{
		// check collisions while moving upwards

		if (map->collide_enemy(d))return true; // collision with enemy results in death

		//if (map->collide_back_hole(d))return true; // collision with black_hole results in death

		if (map->collide_jump_booster(d))// collision with jump_booster results in boost
		{
			jump_boost = true;
			jump_boost_start_time = getTickCount();
		}

		if (posy - speedy >= stop_height)posy -= speedy; // moving upwards
		else // if going above stop_height
		{
			int height_left = stop_height - (posy - speedy); // distance left to travel above stop_height

			posy = stop_height; // fix doodle on stop_height

			map->update(height_left); // move map 
		}
		jumping_dist_left -= speedy;
	}

	else if (jumping_dist_left <= 0) // if not jumping currently = going downwards
	{
		jumping_dist_left = 0;

		RECT d = { posx, posy, posx + doodlew, posy + doodleh };  // rectangle containing doodle sprite
		bool die = false, boost = false, hop = false;

		// check collisions while moving downwards

		if (map->collide_jump_booster(d)) // collision with jump_booster results in boost
		{
			jump_boost = true;
			jump_boost_start_time = getTickCount();
		}

		//if (map->collide_back_hole(d))return true; // collision with black_hole results in death

		if (map->bounce_tile(d, speedy) || map->bounce_enemy(d, speedy))// hopped from the tile or enemy
		{
			jumping_dist_left = jumplen; 
			if (jump_boost)jumping_dist_left = jumping_dist_left * factor; // if got jump boost, boost jump factor times
		}

		else if (map->collide_enemy(d))return true; // collision with enemy results in death

		if(jumping_dist_left == 0) posy += speedy; // didn't hop THEN still going downwards
	}

	if (posy > h)return true; // if fell lower than the window - game over = true

	return false; // else game over = false
}
void Doodle::draw(bool shooting, bool move_left, bool move_right)
{
	map->draw(); // draw map first

	if (move_left)last_move = -1; // set the last move
	if (move_right)last_move = 1;

	Sprite* s;
	if (jump_boost) // if jump boosted - costume of a bunny
	{
		if (shooting)
		{
			s = sprite_doodle_shoot_bunny;
		}
		else if (last_move == -1) s = sprite_doodle_left_bunny;
		else s = sprite_doodle_right_bunny;
	}
	else // else ordinary costume
	{
		if (shooting)
		{
			s = sprite_doodle_shoot;
		}
		else if (last_move == -1) s = sprite_doodle_left;
		else s = sprite_doodle_right;
	}

	drawSprite(s, posx, posy);
	getSpriteSize(s, doodlew, doodleh); // update sprite size

	if(shooting)drawSprite(sprite_doodle_nose, posx + 2 * doodlew / 5, posy); // draw nose if shooting

	for (auto it = bullets.begin(); it != bullets.end(); it++)//draw bullets
	{
		(*it)->draw();
	}
}
Doodle::~Doodle()
{
	delete map;
	for (auto bullet : bullets) {
		delete bullet;
	}
	bullets.clear();

	destroySprite(sprite_doodle_left);
	destroySprite(sprite_doodle_right);
	destroySprite(sprite_doodle_shoot);
	destroySprite(sprite_doodle_left_bunny);
	destroySprite(sprite_doodle_right_bunny);
	destroySprite(sprite_doodle_shoot_bunny);
}