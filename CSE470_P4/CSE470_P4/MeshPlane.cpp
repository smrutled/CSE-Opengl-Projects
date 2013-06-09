#include "MeshPlane.h"

MeshPlane::MeshPlane(GLfloat x, GLfloat y, GLfloat z, GLfloat x_len, GLfloat y_len, int x_tiles, int y_tiles){
	textured=false;
	for(int i = 0; i<=x_tiles; i++)
		for(int j = 0; j<=y_tiles; j++)
			v.push_back(V3f(i*x_len+x,y, j*y_len+z));

	for(int i = 1; i<v.size()-y_tiles-1; i++){
		if(i%(y_tiles+1)!=0){
		vi.push_back(V3i(i-1,i,i+y_tiles+1));
		vi.push_back(V3i( i-1,i+y_tiles+1, i+y_tiles));
		}
	}
	calcFaceNormals();
	calcVertexNormalsAverage();
	calcVertexNormalsWeight();
}

MeshPlane::MeshPlane(GLfloat x, GLfloat y, GLfloat z, GLfloat x_len, GLfloat y_len, int x_tiles, int y_tiles, GLuint texid){
	MeshPlane::texid=texid;
	textured=true;
	t.push_back(V2f(0,0));
	t.push_back(V2f(0,1));
	t.push_back(V2f(1,1));
	t.push_back(V2f(1,0));
	for(int i = 0; i<=x_tiles; i++)
		for(int j = 0; j<=y_tiles; j++)
			v.push_back(V3f(i*x_len+x,y, j*y_len+z));

	for(int i = 1; i<v.size()-y_tiles-1; i++){
		if(i%(y_tiles+1)!=0){
		vi.push_back(V3i(i-1,i,i+y_tiles+1));
		ti.push_back(V3i(0,1,2));
		vi.push_back(V3i( i-1,i+y_tiles+1, i+y_tiles));
		ti.push_back(V3i(0,2,3));
		}
	}
	calcFaceNormals();
	calcVertexNormalsAverage();
	calcVertexNormalsWeight();
}