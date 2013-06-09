

#include "Entity.h"
#include "Shapes.h"
#include "Bullet.h"
class Player: public Entity{
public:
int radius,bulletNum;
GLfloat cannonWidth,cannonHeight;
bool fired;
virtual void render();
virtual void renderBound();
virtual void move(bool dir);
void rotate(bool dir);
Bullet* fire();
Player(int sx, int sy, int radius);
};