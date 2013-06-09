#ifndef Shapes_h
#define Shapes_h
#include "Definitions.h"
#include "glut.h"
#include <cmath>




static void drawRectangleFilled(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat ang){

	glTranslatef(x,y,0);
	glRotatef(ang,0,0,1);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
		glVertex2f(-w/2,-h/2);
		glVertex2f(-w/2,h/2);
		glVertex2f(w/2,h/2);
		glVertex2f(w/2,-h/2);
	glEnd();
	glDisable(GL_BLEND);
}

static void drawRectangleFilled(GLfloat w, GLfloat h){
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
		glVertex2f(-w/2,-h/2);
		glVertex2f(-w/2,h/2);
		glVertex2f(w/2,h/2);
		glVertex2f(w/2,-h/2);
	glEnd();
	glDisable(GL_BLEND);
}

static void drawRectangleUnfilled(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat ang){

	glTranslatef(x,y,0);
	glRotatef(ang,0,0,1);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-w/2,-h/2);
		glVertex2f(-w/2,h/2);
		glVertex2f(w/2,h/2);
		glVertex2f(w/2,-h/2);
	glEnd();
	glDisable(GL_BLEND);
}

static void drawRectangleUnfilled(GLfloat w, GLfloat h){
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-w/2,-h/2);
		glVertex2f(-w/2,h/2);
		glVertex2f(w/2,h/2);
		glVertex2f(w/2,-h/2);
	glEnd();
	glDisable(GL_BLEND);
}

static void drawRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat ang, bool fill){
	if(fill)
	drawRectangleFilled(x,y,w,h,ang);
	else
	drawRectangleUnfilled(x,y,w,h,ang);
}

static void drawRectangle(GLfloat w, GLfloat h, bool fill){
	if(fill)
	drawRectangleFilled(w,h);
	else
	drawRectangleUnfilled(w,h);
}

static void drawOvalFilled(GLfloat x, GLfloat y, GLfloat radX,GLfloat radY, int seg , GLfloat part,GLfloat angle){
	
	glTranslatef(x,y,0);
	glRotatef(angle,0,0,1);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0,0);
	for (int i = 0; i <= seg; i++)
		{
			glVertex2f(radX*cos(i*part*M_PI/seg),radY*sin(i*part*M_PI/seg));
		}
	glEnd();
	glDisable(GL_BLEND);
}

static void drawOvalUnfilled(GLfloat x, GLfloat y, GLfloat radX,GLfloat radY, int seg , GLfloat part,GLfloat angle){
	
	glTranslatef(x,y,0);
	glRotatef(angle,0,0,1);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINE_LOOP);
	if(part<2)
	glVertex2f(0,0);
	for (int i = 0; i <= seg; i++)
		{
			glVertex2f(radX*cos(i*part*M_PI/seg),radY*sin(i*part*M_PI/seg));
		}
	glEnd();
	glDisable(GL_BLEND);
}

static void drawOval(GLfloat x, GLfloat y, GLfloat radX,GLfloat radY, int seg , GLfloat part,GLfloat angle, bool fill){
	if(fill)
	drawOvalFilled(x,y, radX, radY, seg , part,angle);
	else
	drawOvalUnfilled(x,y, radX, radY, seg , part,angle);
}

static void drawCircle(GLfloat x, GLfloat y, GLfloat radius, int seg, GLfloat part,GLfloat angle, bool fill){
	if(fill)
	drawOvalFilled(x,y,radius,radius,seg,part,angle);
	else
	drawOvalUnfilled(x,y,radius,radius,seg,part,angle);
}

//Draws a triangle with the given points (x1,y1), (x2,y2), (x3,y3)
static void drawTriangle(float x1, float y1, float x2,float y2, float x3, float y3){
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_TRIANGLES);
		glVertex2f(x1,y1);;
		glVertex2f(x2,y2);
		glVertex2f(x3,y3);
	glEnd();
	glDisable(GL_BLEND);
}

//Draws a smiley face at (x,y)
static void drawSmileyFace(GLfloat x,GLfloat y, GLfloat radius){
	glColor3f(1.0, 1.0, 0.0);
	glLoadIdentity();
	drawCircle(x,y,radius,40,2,0,true);
	glLoadIdentity();
	glColor3f(0,0,0);
	glLoadIdentity();
	drawOval(x-(radius*2/4),y+(radius*2/4),radius/15,radius/8,30,2,0,true);
	glLoadIdentity();
	drawOval(x+(radius*2/4),y+(radius*2/4),radius/15,radius/8,30,2,0,true);
	glColor3f(0,0,0);
	glLoadIdentity();
	drawOval(x,y,radius*4/5,radius*4/5,40,-1,0,true);
	glColor3f(1.0, 1.0, 0.0);
	glLoadIdentity();
	drawOval(x,y,radius*.72,radius*.72,40,-1,0,true);
}

//Draw Pacman at (x,y)
static void drawPacMan(GLfloat x, GLfloat y, GLfloat radius,GLfloat mouth,bool dir, bool fill){
	glColor3f(1.0, 1.0, 0.0);
	if(dir)
	drawCircle(x,y,radius,40,mouth,200-((mouth-1.6)/.4)*20,fill);
	else
	drawCircle(x,y,radius,40,mouth,20-((mouth-1.6)/.4)*20,fill);
}

#endif