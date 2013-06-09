#include "Entity.h"
#include "Shapes.h"

class Pacman: public Entity
{
public:
	int speed;
	GLfloat size,mouth;
	bool dir,open;
	virtual void move();
    virtual void render();
	Pacman(GLfloat x, GLfloat y, GLfloat size, int speed);
};