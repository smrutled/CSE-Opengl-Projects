#include "MeshCone.h"

MeshCone::MeshCone(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat height, int seg){
	GLfloat s=seg;

	for(int i=0; i<s; i++)
	v.push_back(V3f(radius*cos((i*2*M_PI)/s)+x, y, radius*sin((i*2*M_PI)/s)+z));
	
	v.push_back(V3f(x, y+height, z));
	v.push_back(V3f(x, y, z));

	for(int i=0; i<s-1; i++)
	vi.push_back(V3i(s, i+1, i ));

	vi.push_back(V3i(s, 0 , s-1));

	for(int i=0; i<s-1; i++)
	vi.push_back(V3i(s+1,i,i+1 ));

	vi.push_back(V3i(s+1,s-1, 0));

	calcFaceNormals();
	calcVertexNormalsAverage();
	calcVertexNormalsWeight();
}