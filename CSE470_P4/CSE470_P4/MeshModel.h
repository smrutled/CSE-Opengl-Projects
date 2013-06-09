#include "MeshObject.h"

class MeshModel : public MeshObject
{
public:
    MeshModel(GLfloat x,GLfloat y,GLfloat z,vector<V3f> &mv,vector<int> &mvi);
};