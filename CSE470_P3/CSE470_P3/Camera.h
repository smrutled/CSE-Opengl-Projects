#include "glut.h"
#include <cmath>
#include "Definitions.h"
class Camera {
public:
	GLdouble x,y,z,lookX,lookY,lookZ,upX,upY,upZ, lookRad,angle_x, angle_y;
	GLfloat fov, aspect, nearPlane, farPlane;
	void MoveF();
	void RotateL();
	void RotateR();
	void RotateU();
	void RotateD();
	void MoveB();
	void MoveU();
	void MoveD();
	void UpdateLook();
	Camera(GLdouble x, GLdouble y, GLdouble z, GLdouble angle_x, GLdouble angle_y,GLdouble lookRad, GLfloat fov,GLfloat aspect,GLfloat nearPlane,GLfloat farPlane);
};