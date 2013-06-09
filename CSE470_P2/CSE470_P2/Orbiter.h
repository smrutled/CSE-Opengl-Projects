#include "Entity.h"
class Orbiter : public Entity
{
public:
	bool staticOb;
	int radius, speed;
	GLfloat buffer[16];
	GLfloat orbitrad;
	Orbiter* orbit;
	virtual void move();
    virtual void render();
	virtual void renderBound();
	Orbiter(int sx, int sy ,int rad, int speed, bool staticOb, Orbiter* ob );
	~Orbiter();
};