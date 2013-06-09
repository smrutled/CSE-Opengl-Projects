#include "MeshBox.h"

MeshBox::MeshBox(GLfloat x, GLfloat y, GLfloat z, GLfloat x_len, GLfloat y_len, GLfloat z_len){
	MeshBox::x=x;
	MeshBox::y=y;
	MeshBox::z=z;
	MeshBox::x_len=x_len;
	MeshBox::y_len=y_len;
	MeshBox::z_len=z_len;
	v.push_back(V3f(-0.5*x_len+x,-0.5*y_len+y, 0.5*z_len+z));
	v.push_back(V3f(-0.5*x_len+x, 0.5*y_len+y, 0.5*z_len+z));
	v.push_back(V3f( 0.5*x_len+x, 0.5*y_len+y, 0.5*z_len+z));
	v.push_back(V3f( 0.5*x_len+x,-0.5*y_len+y, 0.5*z_len+z));
	v.push_back(V3f(-0.5*x_len+x,-0.5*y_len+y,-0.5*z_len+z));
	v.push_back(V3f(-0.5*x_len+x, 0.5*y_len+y,-0.5*z_len+z));
	v.push_back(V3f( 0.5*x_len+x, 0.5*y_len+y,-0.5*z_len+z));
	v.push_back(V3f( 0.5*x_len+x,-0.5*y_len+y,-0.5*z_len+z));
	vi.push_back(V3i(0,3,2));
	vi.push_back(V3i(0,2,1));
	vi.push_back(V3i(4,5,6));
	vi.push_back(V3i(4,6,7));
	vi.push_back(V3i(0,1,5));
	vi.push_back(V3i(0,5,4));
	vi.push_back(V3i(1,2,6));
	vi.push_back(V3i(1,6,5));
	vi.push_back(V3i(2,3,7));
	vi.push_back(V3i(2,7,6));
	vi.push_back(V3i(0,7,3));
	vi.push_back(V3i(0,4,7));
	
	calcFaceNormals();
	calcVertexNormalsAverage();
	calcVertexNormalsWeight();
}
