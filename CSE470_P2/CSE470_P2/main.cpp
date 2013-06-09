#include <vector>
#include "freeglut.h"
#include "Player.h"
#include "Enemy.h"
#include "Shapes.h"
#include "Orbiter.h"
#include "Pacman.h"
#include <stdlib.h>
#include <sstream>
#include <string>

//Variables
GLfloat ratioH=1; 
GLfloat ratioW=1;
GLfloat scale=1;
std::stringstream ss;
std::string text;
int score=0;
int gtimer=0;
int timer=0;
int time;
int fps;
int currentScreenW=SCREENWIDTH; 
int	currentScreenH=SCREENHEIGHT;
int speedMenu;
int polyMenu;
int boundMenu;
int win;
int tms=20;
bool bound=false;
Player* player;
std::vector<Entity*>gameObjects;
std::vector<Entity*>bulletObjects;
//Call back functions
void init();
void addObjects();
void menu(int value);
void keyboard(unsigned char key , int x , int y);
void keyboardsp(int key , int x , int y);
void start();
void tick();
void reshape(int w, int h);
void draw();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	init();
	glutMainLoop();

}

void init(){
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(SCREENWIDTH,SCREENHEIGHT);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-SCREENWIDTH)/2,(glutGet(GLUT_SCREEN_HEIGHT)-SCREENHEIGHT)/2);
	win=glutCreateWindow("basics");
	glutReshapeFunc(reshape);
	glutIdleFunc(tick);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardsp);
	glutDisplayFunc(draw);

	//Menu init
	speedMenu= glutCreateMenu(menu);
	glutAddMenuEntry("Fast",M_FAST);
	glutAddMenuEntry("Medium",M_MEDIUM);
	glutAddMenuEntry("Slow",M_SLOW);
	polyMenu=glutCreateMenu(menu);
	glutAddMenuEntry("Filled",M_FILLED);
	glutAddMenuEntry("Outline",M_OUTLINE);
	boundMenu=glutCreateMenu(menu);
	glutAddMenuEntry("On",M_BOUND_ON);
	glutAddMenuEntry("Off",M_BOUND_OFF);
	glutCreateMenu(menu);
	glutAddSubMenu("Speed",speedMenu);
	glutAddSubMenu("Polygon",polyMenu);
	glutAddSubMenu("Bounding Box",boundMenu);
	glutAddMenuEntry("Restart",M_RESTART);
	glutAddMenuEntry("Quit",M_QUIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 600.0, 0.0, 800.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	addObjects();

}
void addObjects(){
	gameObjects.push_back((Entity*)new Orbiter(200,250,20,2,true,new Orbiter(250,250,30,1,false,new Orbiter(350,250,40,1,false,NULL))));
	gameObjects.push_back((Entity*)new Enemy(200,450,20,4,MO_SIN,S_CIRCLE));
	gameObjects.push_back((Entity*)new Enemy(300,550,20,4,MO_ZIGZAG,S_SMILEY));
	gameObjects.push_back((Entity*)new Pacman(100,650,30,3));
	player=new Player(200,0,40);
}

void menu(int value){
	switch(value){
	case M_FAST:
		tms=10;
		break;
	case M_MEDIUM:
		tms=20;
		break;
	case M_SLOW:
		tms=60;
		break;
	case M_FILLED:
		for(int i=0; i<gameObjects.size(); i++){
         gameObjects[i]->fill=true;
		}
		player->fill=true;
		break;
	case M_OUTLINE:
		for(int i=0; i<gameObjects.size(); i++)
         gameObjects[i]->fill=false;
		player->fill=false;
		break;
	case M_BOUND_ON:
		bound=true;
		break;
	case M_BOUND_OFF:
		bound=false;
		break;
	case M_RESTART:
		for(int i=0; i<gameObjects.size(); i++){
         delete gameObjects[i];
		 gameObjects.clear();
		}
		delete player;
		timer=0;
		score=0;
		addObjects();
		break;
	case M_QUIT:
		glutDestroyWindow(win);
	}

	glutPostRedisplay();
}
void keyboard(unsigned char key , int x , int y){
	
	if(key==32){
		gameObjects.push_back(player->fire());
	}
}
void keyboardsp(int key , int x , int y){

	if(key==GLUT_KEY_LEFT){
		player->move(false);
	}
	if(key==GLUT_KEY_RIGHT){
		player->move(true);
	}
	if(key==GLUT_KEY_UP){
		player->rotate(false);
	}
	if(key==GLUT_KEY_DOWN){
		player->rotate(true);
	}

	glutPostRedisplay();

}
void start(){

}
void tick(){
	
	if((glutGet(GLUT_ELAPSED_TIME)-time>tms)){
		

	for(int i=0; i<gameObjects.size(); i++){
         gameObjects[i]->move();
		 if(gameObjects[i]->x>SCREENWIDTH ||gameObjects[i]->x<0 ||gameObjects[i]->y>SCREENHEIGHT ||gameObjects[i]->y<0){
			 switch(gameObjects[i]->type){
			 case ENT_ENEMY:{
					 Enemy* en=static_cast<Enemy*>(gameObjects[i]);
					 en->speed*=-1;
					 gameObjects[i]->move();
					 break;
				}
			 case ENT_PACMAN:{
				 Pacman* en=static_cast<Pacman*>(gameObjects[i]);
					 en->speed*=-1;
					 en->dir=!(en->dir);
					 gameObjects[i]->move();
					 break;
							 }
				 case ENT_BULLET:
					 gameObjects.erase(gameObjects.begin() + i);
					 break;
					 
			 }
			 
		 }

	}
	time=glutGet(GLUT_ELAPSED_TIME);
	if(time/1000>gtimer){
		timer+=1;
		gtimer+=1;
	}
	glutPostRedisplay();
	}
	
}
void reshape(int w, int h){
	currentScreenW=w;
	currentScreenH=h;
	ratioW=(GLfloat)w/SCREENWIDTH;
	ratioH=(GLfloat)h/SCREENHEIGHT;

	if(ratioH<ratioW)
	scale=ratioH;
else
	scale=ratioW;
}

void draw(){
glClear(GL_COLOR_BUFFER_BIT);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0.0, 600.0, 0.0, 800.0);
glViewport(currentScreenW/2-(SCREENWIDTH*scale)/2,100*scale, SCREENWIDTH*scale, SCREENHEIGHT*scale);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
player->render();
if(bound)
player->renderBound();
for(int i=0; i<gameObjects.size(); i++){
         gameObjects[i]->render();
		 if(bound)
		 gameObjects[i]->renderBound();
}


glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0.0, 600, 0.0, 100.0);
glViewport(currentScreenW/2-(SCREENWIDTH*scale)/2,0, SCREENWIDTH*scale, 100*scale);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glColor4f(0,1,1,.2);
drawRectangle((GLfloat)300,(GLfloat)50,(GLfloat)600,(GLfloat)100,0,true);
glLoadIdentity();
glColor4f(0,1,1,1);
drawRectangle((GLfloat)300,(GLfloat)50,(GLfloat)599,(GLfloat)99,0,false);
glColor4f(1,0,0,1);
glLoadIdentity();
glTranslatef(100,50,0);
glPushMatrix();
glScalef(.2,.2,0);
ss.str("");
ss<<"Score:"<<score;
text =ss.str();
glutStrokeString(GLUT_STROKE_ROMAN,(const unsigned char *)(text.c_str()));
glPopMatrix();
glTranslatef(0,-25,0);
glScalef(.2,.2,0);
ss.str("");
ss<<"Time:"<<timer;
text =ss.str();
glutStrokeString(GLUT_STROKE_ROMAN,(const unsigned char *)(text.c_str()));
glutSwapBuffers();
}