#include "Grid.h"
#include "Framework.h"
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
using namespace std;
Grid::Grid(int w, int h, int f, int m)
{
	sprite_tile = createSprite(path_tile);
	sprite_enemy = createSprite(path_enemy);
	sprite_jump_booster = createSprite(path_jump_booster);
	sprite_destr_tile = createSprite(path_destr_tile);
	sprite_black_hole = createSprite(path_black_hole);

	sprite_background = createSprite(path_background);
	setSpriteSize(sprite_background, w, h);

	numbers[0] = createSprite(path_zero);
	numbers[1] = createSprite(path_one);
	numbers[2] = createSprite(path_two);
	numbers[3] = createSprite(path_three);
	numbers[4] = createSprite(path_four);
	numbers[5] = createSprite(path_five);
	numbers[6] = createSprite(path_six);
	numbers[7] = createSprite(path_seven);
	numbers[8] = createSprite(path_eight);
	numbers[9] = createSprite(path_nine);

	numw = 14; numh = 20;
	for(int i = 0; i <= 9; i++)
	setSpriteSize(numbers[i], numw, numh);


	height = h;
	width = w;
	stop_height = f;
	max_dist = m;

	score = 0;
	tile_score = 0;

	getSpriteSize(sprite_tile, tilew, tileh);
	getSpriteSize(sprite_jump_booster, jumpw, jumph);
	getSpriteSize(sprite_enemy, enemyw, enemyh);
	getSpriteSize(sprite_black_hole, holew, holeh);

	prob_enemy = 0.05;
	prob_jump = 0.05;
	tile_destr_prob = 0.05;
	prob_hole = 0.005;

	black_hole_gravity_power = 1;
	black_hole_effect_dist = 400;

	srand(time(0));

	create();

}
int Grid::get_height()
{
	return height;
}
int Grid::get_width()
{
	return width;
}
int Grid::get_stop_height()
{
	return stop_height;
}
void Grid::update(int steps)
{
	score += steps; // score = amount of pixels map shifted

	while (tiles.size() > 0)
	{
		if ((*tiles.begin())->posy >= score)break;
		tiles.erase(tiles.begin()); // if tile is not visible erase it
	}

	while (enemies.size() > 0)
	{
		if ((*enemies.begin())->posy >= score)break;
		enemies.erase(enemies.begin()); // if enemy is not visible erase it
	}

	while (jump_boosters.size() > 0)
	{
		if ((*jump_boosters.begin())->posy >= score)break;
		jump_boosters.erase(jump_boosters.begin()); // if jump_booster is not visible erase it
	}

	while (black_holes.size() > 0)
	{
		if ((*black_holes.begin())->posy >= score)break;
		black_holes.erase(black_holes.begin()); // if black_hole is not visible erase it
	}

	create();
}
void Grid::create(void)
{
	list<Tile*>::iterator tile_it;
	list<Enemy*>::iterator enemy_it;
	list<Jump_booster*>::iterator jump_booster_it;
	list<Black_hole*>::iterator black_hole_it;

	//IMPROTANT:
	// tile's, enemy's, juump_booster's Y coordinates are increasing from bottom to the top of the screen
	// as opposed to sprite's Y coordinates which increase from top to bottom
	//Y coordinates of new objects keep increasing as the player progressess forward
	//Y - score = Y coordinate on the screen counting from bottom(0) to top(h - 1)
	//(height - 1) - (Y - score) = coordinate of the sprite corresponding to object

	int lastx = 0;  // store X coordinate of the highest object(tile, enemy, jump_booster)
	int lasty = 0; // store Y coordinate of the highest object(tile, enemy, jump_booster)
	if (tiles.size() > 0)
	{
		tile_it = prev(tiles.end());
		lasty = (*tile_it)->posy;
	}
	if (enemies.size() > 0)
	{
		enemy_it = prev(enemies.end());
		if (lasty < (*enemy_it)->posy)
		{
			lasty = (*enemy_it)->posy;
		}
	}
	if (jump_boosters.size() > 0)
	{ 
		jump_booster_it = prev(jump_boosters.end());
		if (lasty < (*jump_booster_it)->posy)
		{
			lasty = (*jump_booster_it)->posy;
		}
	}
	if (black_holes.size() > 0)
	{
		black_hole_it = prev(black_holes.end());
		if (lasty < (*black_hole_it)->posy)
		{
			lasty = (*black_hole_it)->posy;
		}
	}
	
	Tile* new_tile;
	Enemy* new_enemy;
	Jump_booster* new_jump_booster;
	Black_hole* new_black_hole;

	int dx, dy;
	int k;
	int from, to;
	int max_num = 2; // maximum number of tiles with the same Y coordinate

	while (lasty - score < 2 * height - 1) // while highest object is lower than 2 * height - 1
	{
		//choosing Y
		from = lasty + (tileh + 5);// not closer than (tile width + 5) from highest object
		to = lasty + max_dist - 5; // not farther than (max_dist - 5) from highest object

		dy = rand() % (to + 1 - from) + from;

		k = rand() % max_num + 1; // number of tiles with chosen Y axis equal to "dy"

		lastx = 0;
		for(int i = 0; i < k; i++) // generating k tiles with the same Y coordinate equal to "dy"
		{
			//choosing X
			from = lastx + (tilew + 5); // not closer than 5 to the previous one
			to = width - (k - i) * (tilew + 5); // making sure (k - i) (including itself) tiles will have enough space

			dx = rand() % (to + 1 - from) + from; 
			 
			new_tile = new Tile;

			new_tile->posx = dx;
			new_tile->posy = dy;
			new_tile->destroyable = false;

			if ((double)rand() <= (double)RAND_MAX * tile_destr_prob) // randonly make tile destroyable
			{
				new_tile->destroyable = true;
			}

			tiles.push_back(new_tile);

			lasty = new_tile->posy;
			lastx = new_tile->posx;

			if ((double)rand() <= (double)RAND_MAX * prob_enemy && score > height) // randonly decide to put an enemy on tile
			{
				new_enemy = new Enemy;

				new_enemy->posx = new_tile->posx + (tilew - enemyw) / 2; // putting the enemy in the middle of the tile
				new_enemy->posy = new_tile->posy + enemyh;

				enemies.push_back(new_enemy);
				lasty = new_enemy->posy;
			}

			else if ((double)rand() <= (double)RAND_MAX * prob_jump && score > height) // randonly decide to put a jump_booster on tile
			{
				new_jump_booster = new Jump_booster;

				new_jump_booster->posx = new_tile->posx + (tilew - jumpw) / 2; // putting the jump_booster in the middle of the tile
				new_jump_booster->posy = new_tile->posy + jumph;

				jump_boosters.push_back(new_jump_booster);
				lasty = new_jump_booster->posy;
			}

			else if ((double)rand() <= (double)RAND_MAX * prob_hole && score > height) // randonly decide to put a black_hole on tile
			{
				new_black_hole = new Black_hole;

				new_black_hole->posx = new_tile->posx + (tilew - holew) / 2; // putting the black_hole in the middle of the tile
				new_black_hole->posy = new_tile->posy + holew;

				black_holes.push_back(new_black_hole);
				lasty = new_black_hole->posy;
			}
		}
	}
}
bool Grid::bounce_tile(RECT d, int speedy)
{
	if (tiles.size() == 0)return false; // should never happen, added for the sake of safety
	RECT t; // rectangle containing tile
	for (auto it = tiles.begin(); it != tiles.end(); it++)
	{
		t.left = (*it)->posx;
		t.top = (height - 1) - ((*it)->posy - score); // tile's Y coordinates are increasing from bottom to the top of the screen
													 // as opposed to sprite's Y coordinates which increase from top to bottom
		t.right = t.left + tilew;
		t.bottom = t.top + tileh;

		if (0 <= t.top - d.bottom && t.top - d.bottom <= speedy) // if "d" is on top of the "tile" with "speedy" precision
		{
			if (t.right < d.left || t.left > d.right)continue; // if "t" is to the left of "d", or "t" is to the right of "d"
															  // then no intersection
			//else we have intersection

			if((*it)->destroyable) // if destroyable
			{
				delete (*it);
				tiles.erase(it);
			}

			tile_score++;
			return true;
		}
	}
	return false;
}
bool Grid::bounce_enemy(RECT d, int speedy)
{
	if (enemies.size() == 0)return false;
	RECT t;
	for (auto it = enemies.begin(); it != enemies.end(); it++)
	{
		t.left = (*it)->posx;
		t.top = height - 1 - ((*it)->posy - score);// enemy's Y coordinates are increasing from bottom to the top of the screen
												  // as opposed to sprite's Y coordinates which increase from top to bottom
		t.right = t.left + enemyw;
		t.bottom = t.top + enemyh;

		if (0 <= t.top - d.bottom && t.top - d.bottom <= speedy) // if "d" is on top of the "enemy" with "speedy" precision
		{
			if (t.right < d.left || t.left > d.right)continue; // if "t" is to the left of "d", or "t" is to the right of "d"
															  // then no intersection
			//else we have intersection

			delete (*it);
			enemies.erase(it);

			tile_score++;
			return true;
		}
	}
	return false;
}
bool Grid::collide_enemy(RECT d)
{
	if (enemies.size() == 0)return false;
	RECT t, res;
	for (auto it = enemies.begin(); it != enemies.end(); it++)
	{
		t.left = (*it)->posx;
		t.top = height - 1 - ((*it)->posy - score); // enemy's Y coordinates are increasing from bottom to the top of the screen
												   // as opposed to sprite's Y coordinates which increase from top to bottom
		t.right = t.left + enemyw;
		t.bottom = t.top + enemyh;

		if (IntersectRect(&res, &d, &t))
		{
			delete (*it); // delete enemy
			enemies.erase(it);
			return true;
		}
	}
	return false;
}
bool Grid::collide_jump_booster(RECT d)
{
	if (jump_boosters.size() == 0)return false;
	RECT t, res;
	for (auto it = jump_boosters.begin(); it != jump_boosters.end(); it++)
	{
		t.left = (*it)->posx;
		t.top = height - 1 - ((*it)->posy - score); // jump_booster's Y coordinates are increasing from bottom to the top of the screen
												   // as opposed to sprite's Y coordinates which increase from top to bottom
		t.right = t.left + jumpw;
		t.bottom = t.top + jumph;

		if (IntersectRect(&res, &d, &t))
		{
			delete (*it); // delete jump_booster
			jump_boosters.erase(it);
			return true;
		}
	}
	return false;
}
bool Grid::collide_back_hole(RECT d)
{
	if (black_holes.size() == 0)return false;
	RECT t, res;
	for (auto it = black_holes.begin(); it != black_holes.end(); it++)
	{
		t.left = (*it)->posx;
		t.top = height - 1 - ((*it)->posy - score); // black_hole's Y coordinates are increasing from bottom to the top of the screen
												   // as opposed to sprite's Y coordinates which increase from top to bottom
		t.right = t.left + holew;
		t.bottom = t.top + holeh;

		if (IntersectRect(&res, &d, &t))
		{
			delete (*it); // delete black_hole
			black_holes.erase(it);
			return true;
		}
	}
	return false;
}
double DistanceBetweenRects(RECT& rect1, RECT& rect2, int &dir) // returns Distance between 2 RECTs
{ 
                                                             // dir is side on which rect2 is relative to rect1
	// Calculate the center points of each rectangle
	double rect1CenterX = rect1.left + ((double)(rect1.right - rect1.left)) / 2.0;
	double rect1CenterY = rect1.top + ((double)(rect1.bottom - rect1.top)) / 2.0;
	double rect2CenterX = rect2.left + ((double)(rect2.right - rect2.left)) / 2.0;
	double rect2CenterY = rect2.top + ((double)(rect2.bottom - rect2.top)) / 2.0;

	dir = 0;
	if (rect1CenterX < rect2CenterX)dir = 1;
	else if (rect1CenterX > rect2CenterX)dir = -1;

	// Calculate the distance between the center points using the Euclidean distance formula
	double distanceX = rect2CenterX - rect1CenterX;
	double distanceY = rect2CenterY - rect1CenterY;
	double distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

	return distance;
}
int Grid::gravitate_black_hole(RECT d)
{
	if (black_holes.size() == 0)return 0;

	int ans = 0;
	RECT t;
	for (auto it = black_holes.begin(); it != black_holes.end(); it++)
	{
		t.left = (*it)->posx;
		t.top = height - 1 - ((*it)->posy - score); // black_hole's Y coordinates are increasing from bottom to the top of the screen
												   // as opposed to sprite's Y coordinates which increase from top to bottom
		t.right = t.left + holew;
		t.bottom = t.top + holeh;

		int dir; 

		if (DistanceBetweenRects(d, t, dir) <= black_hole_effect_dist)
		{
			ans += dir * black_hole_gravity_power;
		}
	}
	return ans;
}
void Grid::draw_tiles()
{
	if (tiles.size() == 0)return; // should never happen, added for the sake of safety
	for (auto it = tiles.begin(); it != tiles.end(); it++)
	{
		if((*it)->destroyable) drawSprite(sprite_destr_tile, (*it)->posx, height - 1 - ((*it)->posy - score));
		else drawSprite(sprite_tile, (*it)->posx, height - 1 - ((*it)->posy - score));
		// tile's Y coordinates are increasing from bottom to the top of the screen
		// as opposed to sprite's Y coordinates which increase from top to bottom
	}
}
void Grid::draw_enemies()
{
	if (enemies.size() == 0)return;
	for (auto it = enemies.begin(); it != enemies.end(); it++)
	{
		drawSprite(sprite_enemy, (*it)->posx, height - 1 - ((*it)->posy - score));
		// enemy's Y coordinates are increasing from bottom to the top of the screen
		// as opposed to sprite's Y coordinates which increase from top to bottom
	}
}
void Grid::draw_jump_boosters()
{
	if (jump_boosters.size() == 0)return;
	for (auto it = jump_boosters.begin(); it != jump_boosters.end(); it++)
	{
		drawSprite(sprite_jump_booster, (*it)->posx, height - 1 - ((*it)->posy - score));
		// jump_booster's Y coordinates are increasing from bottom to the top of the screen
		// as opposed to sprite's Y coordinates which increase from top to bottom
	}
}
void Grid::draw_black_holes()
{
	if (black_holes.size() == 0)return;
	for (auto it = black_holes.begin(); it != black_holes.end(); it++)
	{
		drawSprite(sprite_black_hole, (*it)->posx, height - 1 - ((*it)->posy - score));
		// black_holes's Y coordinates are increasing from bottom to the top of the screen
		// as opposed to sprite's Y coordinates which increase from top to bottom
	}
}
void Grid::draw()
{
	drawSprite(sprite_background, 0, 0);

	draw_score();

	draw_tiles();
	draw_enemies();
	draw_jump_boosters();
	draw_black_holes();
}
void Grid::first_tile(int& x, int& y, int doodlew)
{
	auto it = tiles.begin();
	if (tiles.size() == 0) { x = width / 2; y = stop_height; } // should never happen, added for the sake of safety
	x = (*it)->posx + (tilew - doodlew) / 2;
	y = height - 1 - (*it)->posy;
}
void Grid::draw_score()
{
	string res = "";
	int a = score;
	while (a)
	{
		res = (char)('0' + a % 10) + res;
		a /= 10;
	}
	
	for (int i = 0; i < res.size(); i++)
	{
		drawSprite(numbers[res[i] - '0'], numw * i, 10);
	}

	a = tile_score;
	res = "";
	while (a)
	{
		res = (char)('0' + a % 10) + res;
		a /= 10;
	}

	for (int i = 0; i < res.size(); i++)
	{
		drawSprite(numbers[res[i] - '0'], numw * i, 10 + numh + 10);
	}
}
Grid::~Grid()
{
	if(tiles.size() > 0)
	for (auto it = tiles.begin(); it != tiles.end(); it++)
	{
		delete (*it);
	}
	tiles.clear();

	if (enemies.size() > 0)
	for (auto it = enemies.begin(); it != enemies.end(); it++)
	{
		delete (*it);
	}
	enemies.clear();

	if (jump_boosters.size() > 0)
	for (auto it = jump_boosters.begin(); it != jump_boosters.end(); it++)
	{
		delete (*it);
	}
	jump_boosters.clear();

	if (black_holes.size() > 0)
	for (auto it = black_holes.begin(); it != black_holes.end(); it++)
	{
		delete (*it);
	}
	black_holes.clear();

	destroySprite(sprite_tile);
	destroySprite(sprite_enemy);
	destroySprite(sprite_jump_booster);
	destroySprite(sprite_destr_tile);
	destroySprite(sprite_black_hole);
	destroySprite(sprite_background);
}