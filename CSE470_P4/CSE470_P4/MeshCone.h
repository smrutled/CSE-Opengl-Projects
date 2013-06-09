#include "MeshObject.h"
class MeshCone : public MeshObject
{
public:
    MeshCone(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat height, int seg_y, int seg_r);
	MeshCone(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat height, int seg_y, int seg_r, GLuint texid);
};