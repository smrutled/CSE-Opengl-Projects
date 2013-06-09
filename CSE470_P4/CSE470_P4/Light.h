#ifndef Light_h
#define Light_h

#include "glut.h"
#include <cmath>
#include "Imath\ImathVec.h"
#include "Definitions.h"
using namespace Imath;
class Light {
public:
	GLenum num;
	GLfloat rpos[4],cpos[4],ambient[4], diffuse[4], specular[4];
	GLfloat rotrad,angle;
	void RotateL();
	void RotateR();
	void Enable();
	void Disable();
	void UpdatePosition();
	Light(GLenum num,V3f pos, GLfloat rotrad, V3f ambient, V3f diffuse, V3f specular );
};

#endif