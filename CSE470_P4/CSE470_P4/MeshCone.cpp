#include "MeshCone.h"

MeshCone::MeshCone(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat height, int seg_y, int seg_r){
	GLfloat sy=seg_y;
	GLfloat sr=seg_r;
	textured=false;

	for (int i = 0; i <= sr; i++) {
      GLfloat theta = i * M_PI / sr;
      GLfloat sTheta = (sr-i)/sr;
      GLfloat cTheta =i*height/sr;
      for (int j = 0; j <= sy; j++) {
		v.push_back(V3f((radius*cos(j*2*M_PI/sy)*sTheta)+x,cTheta+y,(radius*sin(j*2*M_PI/sy)*sTheta)+z));  
      }
    }

	for (int i = 0; i < sy; i++) {
      for (int j = 0; j < sr; j++) {
        int first = (i * (sr + 1)) + j;
        int second = first + sr + 1;
		vi.push_back(V3i(first,second,first + 1));
        vi.push_back(V3i(second,second + 1,first + 1));
      }
    }
calcFaceNormals();
	calcVertexNormalsAverage();
	calcVertexNormalsWeight();
}

MeshCone::MeshCone(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat height, int seg_y, int seg_r, GLuint texid){
	GLfloat sy=seg_y;
	GLfloat sr=seg_r;
	int count;
	MeshCone::texid=texid;
	textured=true;

	for (int i = 0; i <= sr; i++) {
      GLfloat theta = i * M_PI / sr;
      GLfloat sTheta = (sr-i)/sr;
      GLfloat cTheta =i*height/sr;
      for (int j = 0; j <= sy; j++) {
		v.push_back(V3f((radius*cos(j*2*M_PI/sy)*sTheta)+x,cTheta+y,(radius*sin(j*2*M_PI/sy)*sTheta)+z));
		t.push_back(V2f(1 - (j / sy),1 - (i / sr)));
      }
    }

	count=v.size();

	for (int i = 0; i <= sr; i++){
		v.push_back(V3f((radius*cos(i*2*M_PI/sr))+x,y,(radius*sin(i*2*M_PI/sr))+z));
		t.push_back(V2f(1 - (i / sr),1 - (i / sr)));
	}

	v.push_back(V3f(x,y,z));
	t.push_back(V2f(1,1));

	for (int i = 0; i < sy; i++) {
      for (int j = 0; j < sr; j++) {
        int first = (i * (sr + 1)) + j;
        int second = first + sr + 1;
		vi.push_back(V3i(first,second,first + 1));
        vi.push_back(V3i(second,second + 1,first + 1));
		ti.push_back(V3i(first,second,first + 1));
        ti.push_back(V3i(second,second + 1,first + 1));
      }
    }

	for (int i = 0; i <= sr; i++){
		vi.push_back(V3i(count+sr+1,count+i+1,count+i));
		ti.push_back(V3i(count+sr+1,count+i+1,count+i));
	}


calcFaceNormals();
	calcVertexNormalsAverage();
	calcVertexNormalsWeight();
}