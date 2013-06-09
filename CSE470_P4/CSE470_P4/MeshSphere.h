#include "MeshObject.h"

class MeshSphere : public MeshObject
{
public:

	MeshSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, int seg_y, int seg_r);
	MeshSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, int seg_y, int seg_r, GLuint texid);
};