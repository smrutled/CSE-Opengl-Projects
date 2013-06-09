#include "Bullet.h"


Bullet::Bullet(int x,int y, int radius, GLfloat angle , int speed,bool fill){
	type=ENT_BULLET;
	boundX=radius;
	boundY=radius;
	Bullet::fill=fill;
	Bullet::x=x;
	Bullet::y=y;
	Bullet::radius=radius;
	Bullet::angle=angle;
	Bullet::speed=speed;
}

void Bullet::move(){
	x+=speed*cos(angle*DEGTORAD);
	y+=speed*sin(angle*DEGTORAD);
}

void Bullet::render(){
	glLoadIdentity();
	glColor3f(0,0,1);
	drawCircle(x,y,radius,40,2,0,fill);
}