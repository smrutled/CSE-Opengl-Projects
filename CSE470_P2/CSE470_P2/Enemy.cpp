#include "Enemy.h"

Enemy::Enemy(GLfloat x, GLfloat y, GLfloat size, int speed,int movement, int shape){
	Enemy::x=x;
	Enemy::y=y;
	Enemy::size=size;
	boundX=size;
	boundY=size;
	fill=true;
	oy=y;
	zig=1;
	angle=0;
	Enemy::speed=speed;
	Enemy::movement= movement;
	Enemy::shape=shape;
	type=ENT_ENEMY;
}

void Enemy::move(){
	x+=speed;
	switch(movement){
	case MO_SIN:
		angle+=.1;
		if(angle>=360)
		angle=angle-360;
		y=2*size*sin(angle)+oy;
		break;
	case MO_ZIGZAG:
		y+=zig*5;
		if(y>size*2+oy){
			y=size*2+oy;
			zig=-1;
		}
		if(y<oy-size*2){
			y=oy-size*2;
			zig=1;
		}
		break;
	case MO_STRAIGHT:
		break;
	}
}

void Enemy::render(){
	glLoadIdentity();
	glColor3f(0,1,0);
	switch(shape){
	case S_CIRCLE:
	drawCircle(x,y,size,40,2,0,fill);	
		break;
	case S_RECT:
	drawRectangle(x,y,size*2,size*2,angle,fill);
		break;
	case S_SMILEY:
	drawSmileyFace(x,y,size);	
		break;
	}
	
}