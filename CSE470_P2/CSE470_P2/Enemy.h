#include "Entity.h"
#include "Shapes.h"

class Enemy: public Entity
{
public:
	int speed;
	GLfloat size, oy;
	int shape, zig;
	int movement;
	virtual void move();
    virtual void render();
	Enemy(GLfloat x, GLfloat y, GLfloat size, int speed, int movement, int shape);
};