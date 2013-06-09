#include "MeshCylinder.h"

MeshCylinder::MeshCylinder(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat height, int seg){
	GLfloat s=seg;
	textured=false;
	for(int i=0; i<s; i++)
	v.push_back(V3f(radius*cos((i*2*M_PI)/s)+x, height+y, radius*sin((i*2*M_PI)/s)+z));

	v.push_back(V3f(radius+x, height+y, z));

	for(int i=0; i<s; i++)
	v.push_back(V3f(radius*cos((i*2*M_PI)/s)+x, y, radius*sin((i*2*M_PI)/s)+z));

	v.push_back(V3f(radius+x, y, z));
	
	v.push_back(V3f(x, y+height, z));
	v.push_back(V3f(x, y, z));

	for(int i=0; i<s; i++)
	vi.push_back(V3i(s*2+2, i+1, i ));

//	vi.push_back(V3i(s*2, 0 , s-1));

	for(int i=0; i<s; i++)
	vi.push_back(V3i(s*2+3,i+s+1,i+s+2));

//	vi.push_back(V3i(s*2+1,s,s*2-1));

	for(int i=0; i<s; i++)
	vi.push_back(V3i(i,i+s+2,i+s+1));

//	vi.push_back(V3i(s-1,s,s*2-1));

	for(int i=0; i<s; i++)
	vi.push_back(V3i(i, i+1,i+s+2));

//	vi.push_back(V3i(s-1,0,s));

	calcFaceNormals();
	calcVertexNormalsAverage();
	calcVertexNormalsWeight();
}

MeshCylinder::MeshCylinder(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat height, int seg, GLuint texid){
	GLfloat s=seg;
	textured=true;
	MeshCylinder::texid=texid;
	for(int i=0; i<s; i++){
	v.push_back(V3f(radius*cos((i*2*M_PI)/s)+x, height+y, radius*sin((i*2*M_PI)/s)+z));
	t.push_back(V2f(i/s,1));
	}
	v.push_back(V3f(radius+x, height+y, z));
	t.push_back(V2f(1,1));

	for(int i=0; i<s; i++){
	v.push_back(V3f(radius*cos((i*2*M_PI)/s)+x, y, radius*sin((i*2*M_PI)/s)+z));
	t.push_back(V2f(i/s,0));
	}
	
	v.push_back(V3f(radius+x, y, z));
	t.push_back(V2f(1,0));

	v.push_back(V3f(x, y+height, z));
	t.push_back(V2f(.5,0));
	v.push_back(V3f(x, y, z));
	t.push_back(V2f(0,0));

	for(int i=0; i<s; i++){
	vi.push_back(V3i(s*2+2, i+1, i ));
	ti.push_back(V3i(s*2+2, i+1, i ));
	}

	for(int i=0; i<s; i++){
	vi.push_back(V3i(s*2+3,i+s+1,i+s+2));
	ti.push_back(V3i(s*2+3,i+s+1,i+s+2));
	}

	for(int i=0; i<s; i++){
	vi.push_back(V3i(i,i+s+2,i+s+1));
	ti.push_back(V3i(i,i+s+2,i+s+1));
	}

	for(int i=0; i<s; i++){
	vi.push_back(V3i(i, i+1,i+s+2));
	ti.push_back(V3i(i, i+1,i+s+2));;
	}

	calcFaceNormals();
	calcVertexNormalsAverage();
	calcVertexNormalsWeight();
}