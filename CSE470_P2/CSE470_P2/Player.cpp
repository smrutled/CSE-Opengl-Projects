#include "Player.h"

Player::Player(int sx, int sy, int rad){
	type=ENT_PLAYER;
	x=sx;
	y=sy;
	fill=true;
	boundX=rad;
	boundY=rad;
	radius=rad;
	angle=90;
	cannonWidth=radius/2;
	cannonHeight=radius;
	bulletNum=0;
}

void::Player::move(bool dir){
	if(dir)
		x+=1;
	else
		x-=1;
}

void Player::rotate(bool dir){
	if(dir)
		angle+=1;
	else
		angle-=1;
}
Bullet* Player::fire(){
	return new Bullet(x+cannonHeight*cos(angle*DEGTORAD),y+radius/2+cannonHeight*sin(angle*DEGTORAD),10,angle,10,fill);
}
void Player::render(){
	glLoadIdentity();
	glColor3f(1,0,0);
	drawCircle(x,y,radius,40,1,0,fill);
	glColor3f(1,1,0);
	glTranslatef(0,radius/2,0);
	glRotatef(angle,0,0,1);
	if(fill)
	glBegin(GL_POLYGON);
	else
	glBegin(GL_LINE_LOOP);
		glVertex2f(0,-cannonWidth/2);
		glVertex2f(0,cannonWidth/2);
		glVertex2f(cannonHeight,cannonWidth/2);
		glVertex2f(cannonHeight,-cannonWidth/2);
	glEnd();
}

void Player::renderBound(){
	glColor3f(0,1,1);
		glLoadIdentity();
		drawRectangle(x,y+radius/2,boundX*2,boundY*2,0,false);
}