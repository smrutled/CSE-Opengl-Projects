#include "MeshModel.h"
MeshModel::MeshModel(GLfloat x,GLfloat y,GLfloat z, vector<V3f> &mv,vector<int> &mvi){
	textured=false;
	for(int i=0; i<mv.size();i++)
		v.push_back(V3f(mv[i].x+x,mv[i].y+y,mv[i].z+z));
	for(int i=0; i<mvi.size(); i+=3)
		vi.push_back(V3i(mvi[i],mvi[i+1],mvi[i+2]));
	

	calcFaceNormals();
	calcVertexNormalsAverage();
	calcVertexNormalsWeight();
}