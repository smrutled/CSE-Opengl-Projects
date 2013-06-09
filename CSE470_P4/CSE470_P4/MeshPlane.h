#include "MeshObject.h"

class MeshPlane : public MeshObject
{
public:
	MeshPlane(GLfloat x, GLfloat y, GLfloat z, GLfloat x_len, GLfloat y_len, int x_tiles, int y_tiles);
	MeshPlane(GLfloat x, GLfloat y, GLfloat z, GLfloat x_len, GLfloat y_len, int x_tiles, int y_tiles, GLuint texid);
}; 