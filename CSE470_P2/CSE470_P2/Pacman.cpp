#include "Pacman.h"
Pacman::Pacman(GLfloat x, GLfloat y, GLfloat size, int speed){
	Pacman::x=x;
	Pacman::y=y;
	Pacman::size=size;
	boundX=size;
	boundY=size;
	fill=true;
	mouth=2;
	angle=0;
	dir=false;
	Pacman::speed=speed;
	type=ENT_PACMAN;
}

void Pacman::move(){
	x+=speed;
	if(mouth<=1.6)
		open=false;
	if(mouth>=2)
		open=true;
	if(open)
	mouth-=.05;
	else
	mouth+=.05;
	
}

void Pacman::render(){
	glLoadIdentity();
	drawPacMan(x,y,size,mouth,dir,fill);
	
}