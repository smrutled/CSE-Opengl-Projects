#ifndef Entity_h
#define Entity_h

#include "Definitions.h"
#include "glut.h"
#include "Shapes.h"
#include <cmath>
class Entity
{
    public: GLfloat x,y,angle;
	int type,boundX,boundY;
	bool fill;
	virtual void move(){
        return;
    };
    virtual void render(){
		return;
    };
	virtual void renderBound(){
		glColor3f(0,1,1);
		glLoadIdentity();
		drawRectangle(x,y,boundX*2,boundY*2,angle,false);
    };


    
};

#endif