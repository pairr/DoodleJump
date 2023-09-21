#include "Bullet.h"
#include "Framework.h"
#include "frame.h"
#include <math.h>
#include <algorithm>
Bullet::Bullet(double x, double y, double dx, double dy, int a, int b)
{
	sprite_bullet = createSprite(path_bullet);
	getSpriteSize(sprite_bullet, bulletw, bulleth);

	dead = false;
	posx = x; 
	posy = y;

	deltax = dx;
	deltay = dy;

	w = a; 
	h = b;

	speed = 2;
}
void Bullet::draw(void)
{
	if (dead)return;
	getSpriteSize(sprite_bullet, bulletw, bulleth);
	drawSprite(sprite_bullet, (int)posx, (int)posy);
}
void Bullet::update(Grid *map)
{	
	RECT d; //rectangle containing Bullet sprite
	d.left = posx;
	d.top = posy;
	d.right = (deltax * speed + posx + bulletw);
	d.bottom = (deltay * speed + posy + bulleth);

	dead = map->collide_enemy(d); // check collisions with enemies

	if ((int)posx >= w || (int)posy >= h || (int)posx <= 0 || (int)posy <= 0) // if position of bullet is out of window
	{
		dead = true;
	}
	else // else update position
	{
		posx += deltax * speed;
		posy += deltay * speed;
	}
}
bool Bullet::isdead()
{
	return dead;
}
Bullet::~Bullet()
{
	destroySprite(sprite_bullet);
}