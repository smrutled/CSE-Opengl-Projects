#include "Orbiter.h"


Orbiter::Orbiter(int sx, int sy, int rad, int speed, bool staticOb, Orbiter* ob){
	x=sx;
	y=sy;
	boundX=rad;
	boundY=rad;
	radius=rad;
	fill=true;
	Orbiter::staticOb=staticOb;
	orbit=ob;
	Orbiter::speed=speed;
	if(orbit!=NULL){
		orbitrad=sqrt((orbit->x-x)*(orbit->x-x)+(orbit->y-y)*(orbit->y-y));
		if(orbit->x-x!=0)
		angle=tanh((orbit->y-y)/(orbit->x-x));
		else if(orbit->y>y)
		angle=-90;
		else
		angle=90;
	}
	else
	angle=0;
}

void Orbiter::move(){
	if(orbit!=NULL){
		orbit->move();
		angle+=speed;
		if(angle>=360)
		angle=angle-360;
		
	}
	else{
	
	}
}

void Orbiter::render(){
	if(orbit!=NULL){
	orbit->fill=fill;
	orbit->render();
	glRotatef(angle,0,0,1);
	glTranslatef(orbitrad,0,0);
	glGetFloatv(GL_MODELVIEW_MATRIX, buffer);
	x=buffer[12];
	y=buffer[13];
	if(staticOb)
		glRotatef(-angle,0,0,1);
	if(fill)
	glBegin(GL_POLYGON);
	else
	glBegin(GL_LINE_LOOP);
		glColor4f(0,1,1,1);
		glVertex2f(-radius/2,-radius/2);
		glVertex2f(-radius/2,radius/2);
		glColor4f(1,0,0,1);
		glVertex2f(radius/2,radius/2);
		glVertex2f(radius/2,-radius/2);
	glEnd();
	if(!staticOb)
	glRotatef(-angle,0,0,1);
	
	}
	else{
	glLoadIdentity();
	glTranslatef(x,y,0);
	glRotatef(angle,0,0,1);
	glColor3f(1,0,1);
	drawRectangle(radius,radius,fill);
	}
}
void Orbiter::renderBound(){
	if(orbit!=NULL)
		orbit->renderBound();
	glColor3f(0,1,1);
	glLoadIdentity();
	glTranslatef(x,y,0);
	if(!staticOb)
	glRotatef(angle,0,0,1);
	drawRectangle(boundX,boundY,false);
}
Orbiter::~Orbiter(){
	delete orbit;
}