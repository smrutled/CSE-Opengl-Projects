#include <stdio.h>

#include "freeglut.h"
#include <math.h>
#define M_PI 3.14159265358979323846264338327950288419716939937

enum SHAPE{S_TRIANGLE,S_RECTANGLE,S_CIRCLE,S_SMILEY,S_PACMAN};
double ratioW = 1;
double ratioH = 1;
int screenWidth =500;
int screenHeight= 500;
int submenu;
enum SHAPE shapeType =S_TRIANGLE;

//Shape switcher
void shapeMenu(int value){
	switch(value){
	case S_TRIANGLE:
	case S_RECTANGLE:
	case S_CIRCLE:
	case S_SMILEY:
	case S_PACMAN:
		{
			shapeType=(enum SHAPE)value;
			break;
		}
	}
	glutPostRedisplay();
}

//Draws a rectangle at (x,y) with width w and height h
void drawRectangle(int x, int y, int w, int h){
	float sX=(float)x/screenWidth;
	float sY=(float)y/screenHeight;
	float sW=(float)w/screenWidth;
	float sH=(float)h/screenHeight;

	glBegin(GL_POLYGON);
		glVertex2f(sX-sW/2,sY-sH/2);
		glVertex2f(sX-sW/2,sY+sH/2);
		glVertex2f(sX+sW/2,sY+sH/2);
		glVertex2f(sX+sW/2,sY-sH/2);
	glEnd();
}

//Draws a line loop at (x,y)
//radW: horizontal radius
//radH: vertical radius
//width: line width
//seg: number of segments
//shift: moves the starting 
void drawLineLoop(int x, int y, int radW,int radH, int width, int seg, float shift){
	float sX = (float)x/screenWidth;
	float sY =(float)y/screenHeight;
	float sR =(float)radW/screenWidth;
	float sR2 =(float)radH/screenHeight;
	glLineWidth(width);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i <= seg; i++)
		{
			glVertex2f(sR*cos(i*2*M_PI/seg+shift)+sX,sR2*sin(i*2*M_PI/seg+shift)+sY);
		}
	glEnd();
}

//Draws a oval at (x,y)
//radW: horizontal radius
//radH: vertical radius
//width: line width
//seg: number of segments
//part: how much to draw (-2.0 to 2.0)
//shift: moves the starting point
void drawOval(int x, int y, int radW,int radH, int seg , float part,float shift){
	float sX = (float)x/screenWidth;
	float sY =(float)y/screenHeight;
	float sR =(float)radW/screenWidth;
	float sR2 =(float)radH/screenHeight;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(sX,sY);
	for (int i = 0; i <= seg; i++)
		{
			glVertex2f(sR*cos(i*part*M_PI/seg+shift)+sX,sR2*sin(i*part*M_PI/seg+shift)+sY);
		}
	glEnd();
}
void drawCircle(int x, int y, int radius, int seg, float part,float shift){
	drawOval(x,y,radius,radius,seg,part,shift);
}

//Draws a triangle with the given points (x1,y1), (x2,y2), (x3,y3)
void drawTriangle(float x1, float y1, float x2,float y2, float x3, float y3){
	float sX1 = (float)x1/screenWidth;
	float sY1 =(float)y1/screenHeight;
	float sX2 = (float)x2/screenWidth;
	float sY2 =(float)y2/screenHeight;
	float sX3= (float)x3/screenWidth;
	float sY3 =(float)y3/screenHeight;
	
	glBegin(GL_TRIANGLES);
		glVertex2f(sX1,sY1);;
		glVertex2f(sX2,sY2);
		glVertex2f(sX3,sY3);
	glEnd();

}

//Draws a smiley face at (x,y)
void drawSmileyFace(int x,int y, int radius){
	glColor3f(1.0, 1.0, 0.0);
	drawCircle(x,y,radius,40,2,0);
	glColor3f(0,0,0);
	drawOval(x-(radius*2/4),y+(radius*2/4),radius/15,radius/8,30,2,0);
	drawOval(x+(radius*2/4),y+(radius*2/4),radius/15,radius/8,30,2,0);
	glColor3f(0,0,0);
	drawOval(0,0,200,200,40,-1,0);
	glColor3f(1.0, 1.0, 0.0);
	drawOval(0,0,180,180,40,-1,0);
}

//Draw Pacman at (x,y)
void drawPacMan(int x, int y, int radius){
	glColor3f(1.0, 1.0, 0.0);
	drawCircle(x,y,radius,40,1.7,M_PI/6);
}

//Called when window is resized
void reshape(int w, int h){
	ratioW=(double)w/screenWidth;
	ratioH=(double)h/screenHeight;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0*ratioW, 1.0*ratioW, -1.0*ratioH, 1.0*ratioH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0, 0, w, h);
	if(ratioH<ratioW)
		glScalef(ratioH,ratioH,1);
	else
		glScalef(ratioW,ratioW,1);
}


/* 
 * The rountine display() gets called each time the window is redrawn.
 */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	switch(shapeType)
	{
	case S_TRIANGLE:
		drawTriangle(-100,0,0,250,100,0);
		break;
	case S_RECTANGLE:
		drawRectangle(0,0,500,500);
		break;
	case S_CIRCLE:
		drawCircle(0,0,250,40,2,0);
		break;
	case S_SMILEY:
		drawSmileyFace(0,0,250);
		break;
	case S_PACMAN:
		drawPacMan(0,0,250);
		break;
	default:
		break;
	}
	
	glutSwapBuffers();
}

/* Initialize states -- called before */

void init()
{
	/* set background clear color to black */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	/* set current color to white */
	glColor3f(1.0, 1.0, 1.0);

	/* identify the projection matrix that we would like to alter */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	/* the window will correspond to these world coorinates */
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	/* identify the modeling and viewing matrix that can be modified from here on */
	/* we leave the routine in this mode in case we want to move the object around */
	/* or specify the camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* The main program */

int main(int argc, char** argv)
{

	/* create a window, position it, and name it */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(screenWidth,screenHeight);
	glutInitWindowPosition(200,200);
	glutCreateWindow("basics");
	
	/* create a callback routine for (re-)display */
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	//Menu init
	submenu= glutCreateMenu(shapeMenu);
	glutAddMenuEntry("Triangle",S_TRIANGLE);
	glutAddMenuEntry("Rectangle",S_RECTANGLE);
	glutAddMenuEntry("Circle",S_CIRCLE);
	glutAddMenuEntry("Smiley",S_SMILEY);
	glutAddMenuEntry("PacMan",S_PACMAN);
	glutCreateMenu(shapeMenu);
	glutAddSubMenu("Draw",submenu);
	glutAttachMenu(GLUT_LEFT_BUTTON);
	
	/* init some states */
	init();

	/* entering the event loop */
	glutMainLoop();

}
