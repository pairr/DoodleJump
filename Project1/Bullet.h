#pragma once
#include "Framework.h"
#include <vector>
#include "Grid.h"
using namespace std;
class Bullet
{
private:
	bool dead = false; // dead = true,   if position is out of window OR killed enemy
					  // used to detect dead bullets in Bullets list(Doodle class) and remove them

	double posx, posy; // x and y coordinates of Bullet
	double deltax, deltay; // unit vector of direction of moving
	int speed = 20; // speed in pixels (euclidean metrics)
	int bulletw, bulleth; // width and height of a bullet sprite
	int w, h; // width and height of a window    (needed to detect if position of bullet is out of window)


	const char* path_bullet = "C:\\DoodleJump\\Project1\\data\\bubble@2x.png";
	Sprite* sprite_bullet;

public:
	Bullet(double x, double y, double dx, double dy, int a, int b);
	void update(Grid *map); // update Bullet state depending on the state of the game MAP objects
	void draw(void);
	bool isdead(); // IF dead then TRUE, else FALSE
	~Bullet();
};

