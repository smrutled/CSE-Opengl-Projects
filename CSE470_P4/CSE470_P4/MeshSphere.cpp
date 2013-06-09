#include "MeshSphere.h"


MeshSphere::MeshSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, int seg_y, int seg_r){
	GLfloat sy=seg_y;
	GLfloat sr=seg_r;
	textured=false;

	for (int i = 0; i <= sr; i++) {
      GLfloat theta = i * M_PI / sr;
      GLfloat sTheta = sin(theta);
      GLfloat cTheta =cos(theta);
      for (int j = 0; j <= sy; j++) {
		v.push_back(V3f((radius*cos(j*2*M_PI/sy)*sTheta)+x,(radius*cTheta)+y,(radius*sin(j*2*M_PI/sy)*sTheta)+z));  
      }
    }

	for (int i = 0; i < sy; i++) {
      for (int j = 0; j < sr; j++) {
        int first = (i * (sr + 1)) + j;
        int second = first + sr + 1;
		vi.push_back(V3i(first,first + 1,second));
        vi.push_back(V3i(second,first + 1,second + 1));
      }
    }
calcFaceNormals();
	calcVertexNormalsAverage();
	calcVertexNormalsWeight();
}

MeshSphere::MeshSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, int seg_y, int seg_r, GLuint texid){
	GLfloat sy=seg_y;
	GLfloat sr=seg_r;
	MeshSphere::texid=texid;
	textured=true;

	for (int i = 0; i <= sr; i++) {
      GLfloat theta = i * M_PI / sr;
      GLfloat sTheta = sin(theta);
      GLfloat cTheta =cos(theta);
      for (int j = 0; j <= sy; j++) {
		v.push_back(V3f((radius*cos(j*2*M_PI/sy)*sTheta)+x,(radius*cTheta)+y,(radius*sin(j*2*M_PI/sy)*sTheta)+z));
        t.push_back(V2f(1 - (j / sy),1 - (i / sr)));
        
      }
    }

	for (int i = 0; i < sy; i++) {
      for (int j = 0; j < sr; j++) {
        int first = (i * (sr + 1)) + j;
        int second = first + sr + 1;
		vi.push_back(V3i(first,first + 1,second));
        vi.push_back(V3i(second,first + 1,second + 1));
		ti.push_back(V3i(first,first + 1,second));
        ti.push_back(V3i(second,first + 1,second + 1));
      }
    }
calcFaceNormals();
	calcVertexNormalsAverage();
	calcVertexNormalsWeight();
}