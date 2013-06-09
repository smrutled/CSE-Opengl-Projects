#include "MeshObject.h"

class MeshBox : public MeshObject
{
public:
	GLfloat x_len, y_len, z_len;

	MeshBox(GLfloat x, GLfloat y, GLfloat z, GLfloat x_len, GLfloat y_len, GLfloat z_len);
	MeshBox(GLfloat x, GLfloat y, GLfloat z, GLfloat x_len, GLfloat y_len, GLfloat z_len, GLuint texid);
};
