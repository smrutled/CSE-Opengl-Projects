#ifndef MeshObject_h
#define MeshObject_h

#include "Definitions.h"
#include <vector>
#include "glut.h"
#include "Imath\ImathVec.h"
#include <cmath>
using namespace std;
using namespace Imath;
class MeshObject
{
public: 
	GLfloat x,y,z,angle;
	V3f color;
	vector<V3f> v;
	vector<V3f> n;
	vector<V3f> nv;
	vector<V3f> nva;
	vector<V2f> texc;
	vector<V3i> vi;
	vector<V3f> vcenter;
	vector<float> area;

	void calcFaceNormals(){
		V3f vec;

		for (int i =0; i < vi.size(); i++ ){
			vec = ((v[vi[i].y]-v[vi[i].x]) % (v[vi[i].z]-v[vi[i].x]));
			area.push_back(vec.length()/2);
			vcenter.push_back((v[vi[i].x]+v[vi[i].y]+v[vi[i].z])/3);
			n.push_back(vec.normalize());
		}
	};
    void calcVertexNormalsAverage(){
		V3f vec;
		int vecsize;
		for(int i=0; i<v.size(); i++){
			vec=V3f(0,0,0);
			vecsize=0;
			for(int j=0; j<vi.size(); j++){
				if(vi[j].x==i || vi[j].y==i || vi[j].z==i){
					vec=vec+n[j];
					vecsize++;
				}
			}
			vec=vec/vecsize;
			vec=vec.normalize();
			nv.push_back(vec);
		}

	};
    void calcVertexNormalsWeight(){
		V3f vec;
		int vecsize;
		for(int i=0; i<v.size(); i++){
			vec=V3f(0,0,0);
			vecsize=0;
			for(int j=0; j<vi.size(); j++){
				if(vi[j].x==i || vi[j].y==i || vi[j].z==i){
					vec=vec+(n[j]*area[j]);
					vecsize++;
				}
			}
			vec=vec/vecsize;
			vec=vec.normalize();
			nva.push_back(vec);
		}

	};
    
};

#endif