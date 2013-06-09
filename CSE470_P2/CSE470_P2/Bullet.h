#include "Definitions.h"
#include "glut.h"
#include <cmath>
#include "Entity.h"
#include "Shapes.h"

class Bullet: public Entity
{
    public:
	int radius,speed;
	virtual void move();
    virtual void render();
	Bullet(int x,int y,int radius,GLfloat angle, int speed,bool fill);
    
};