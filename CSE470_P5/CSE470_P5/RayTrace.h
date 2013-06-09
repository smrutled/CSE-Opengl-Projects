/*
NOTE: This is the file you will need to begin working with
You will need to implement the RayTrace::CalculatePixel () function

This file defines the following:
RayTrace Class
*/

#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <stdio.h>
#include <stdlib.h>
#include "Def.h"
#include "Utils.h"

/*
RayTrace Class - The class containing the function you will need to implement

This is the class with the function you need to implement
*/

class RayTrace
{
public:
	/* - Scene Variable for the Scene Definition - */
	Scene m_Scene;

	// -- Constructors & Destructors --
	RayTrace (void) {}
	~RayTrace (void) {}

	// -- Main Functions --
	// - CalculatePixel - Returns the Computed Pixel for that screen coordinate
	
	bool hitSphere(Vector &p, Vector &d, SceneSphere &obj, float &t) 
	{ 

		Vector cv=obj.center - p;
		float v = d.Dot(cv);
		float disc = obj.radius*obj.radius - (cv.Dot(cv)- v*v); 
		if (disc < 0.0f) 
			return false; 
		float t0 = v - sqrtf(disc); 
		float t1 = v + sqrtf(disc);
		bool retvalue = false;  
		if ((t0 > 0.1f) && (t0 < t)) 
		{
			t = t0;
			retvalue = true; 
		} 
		if ((t1 > 0.1f) && (t1 < t)) 
		{
			t = t1; 
			retvalue = true; 
		}
		return retvalue; 
	}

	bool hitTriangle(Vector &p, Vector &d, SceneTriangle &obj, float &t) 
	{  
		Vector e1 = obj.vertex[1]- obj.vertex[0];
		Vector e2 = obj.vertex[2]- obj.vertex[0];
		Vector h = d.Cross(e2);
		float a = e1.Dot(h);
		if (a > -0.00001 && a < 0.00001)
			return(false);
		float f =1/a;
		Vector s= p-obj.vertex[0];
		float u = f * s.Dot(h);

		if (u < 0.0 || u > 1.0)
			return(false);

		Vector q=s.Cross(e1);
		float v = f * d.Dot(q);

		if (v < 0.0 || u + v > 1.0)
			return(false);

		t = f * e2.Dot(q);

		if (t > 0.00001) 
			return true;
		else 
			return false;

	}

	bool hitModel(Vector &p, Vector &d, SceneModel &obj, float &t, int &tnum){
		int trinum=obj.GetNumTriangles();
		float ti=0;
		bool in=false;
		SceneTriangle *tri;
		for(int i = 0; i<trinum; i++){
			tri=obj.GetTriangle(i);
			if(hitTriangle(p,d,*tri,ti))
				if(ti<t){
					tnum=i;
					t=ti;
					in=true;
				}
		}
		
		if(in)
			return true;
		else
			return false;

	}


	Vector CalculatePixel (int screenX, int screenY)
	{
		float t=0;
		float red =0, green=0,blue=0;
		bool in=false;
		float coef = 1.0f;
		SceneObject *obj;
		Camera cam=m_Scene.m_Camera;
		Vector vrs, vrd, pt, color;
		int onum=m_Scene.GetNumObjects();
		int tnum=0;
		int obnum=0;
		float xp = (float)screenX/(float)WINDOW_WIDTH * 2-1; 
		float yp = (float)screenY/(float)WINDOW_HEIGHT * 2-1;
		Vector w = (cam.target-cam.position).Normalize();
		Vector u = cam.up.Cross(w);
		Vector v = w.Cross(u);
		vrd=w.Scale(2) + u.Scale(-xp) + v.Scale(yp);
		vrd.Normalize();
		vrs=cam.GetPosition();

//Detect closest object intersection of ray		
		for(int i=0; i<onum; i++){
			float ti=9999;
			obj=m_Scene.GetObjectA(i);
			if(obj->IsSphere()){
				if(hitSphere(vrs,vrd,*(SceneSphere*)obj,ti))
					if(!in){
						t=ti;
						in=true;
						obnum=i;
					}
			}
			else if(obj->IsTriangle()){
				if(hitTriangle(vrs,vrd,*(SceneTriangle*)obj,ti))
					if(!in){
						t=ti;
						in=true;
						obnum=i;
					}
			}
			else if(obj->IsModel()){
				if(hitModel(vrs,vrd,*(SceneModel*)obj,ti,tnum))
					if(!in){
						t=ti;
						in=true;
						obnum=i;
					} 
			}
			if(in)
				if(ti<t){
					t=ti;
					obnum=i;
				}

		}



		Vector n;
		SceneMaterial mat;
	//If intersection with an object exist
		if(in){
			//Point of intersection
			pt  = vrs + vrd.Scale(t);
			obj=m_Scene.GetObjectA(obnum);
			//Determine the normal
			if(obj->IsSphere()){
				SceneSphere *sph=(SceneSphere*)obj;
				n=pt-sph->center;
				
				mat=m_Scene.GetMaterial(sph->material);
			}
			else if(obj->IsTriangle()){
				Vector v1,v2,v3;
				SceneTriangle *tri=(SceneTriangle*)obj;
				v1=tri->vertex[0];
				v2=tri->vertex[1];
				v3=tri->vertex[2];
				float area = (v2-v1).Cross(v3-v1).Magnitude()/2;
				float a=(v2-pt).Cross(v3-pt).Magnitude()/(2*area);
				float b=(v3-pt).Cross(v1-pt).Magnitude()/(2*area);
				float c = 1-a-b;
				n= tri->normal[0].Scale(a)+tri->normal[1].Scale(b)+tri->normal[2].Scale(c);
				mat=*SceneMaterial::averageMaterial(m_Scene.GetMaterial(tri->material[0]),m_Scene.GetMaterial(tri->material[1]),m_Scene.GetMaterial(tri->material[2]),a,b,c);
			}
			else if(obj->IsModel()){
				Vector v1,v2,v3;
				SceneModel *mod=(SceneModel*)obj;
				SceneTriangle *tri=(SceneTriangle*)mod->GetTriangle(tnum);
				v1=tri->vertex[0];
				v2=tri->vertex[1];
				v3=tri->vertex[2];
				float area = (v2-v1).Cross(v3-v1).Magnitude()/2;
				float a=(v2-pt).Cross(v3-pt).Magnitude()/(2*area);
				float b=(v3-pt).Cross(v1-pt).Magnitude()/(2*area);
				float c = 1-a-b;

				n= tri->normal[0].Scale(a)+tri->normal[1].Scale(b)+tri->normal[2].Scale(c);
				mat=m_Scene.GetMaterial(tri->material[0]);
			}

			n.Normalize();
	//Check for light ray intersections

			for(unsigned int i=0; i < m_Scene.GetNumLights(); i++){
				SceneLight light=m_Scene.GetLight(i);
				Vector lightdir=light.position-pt;
				float lighdist = lightdir.Magnitude();
				lightdir.Normalize();
				bool shadow=false;				
				//Determine if object is in shadow
				for(int i=0; i<onum; i++){
					SceneObject *ob=m_Scene.GetObjectA(i);

					if(ob->IsSphere()){
						if(hitSphere(pt,lightdir,*(SceneSphere*)obj,t)){
							shadow=true;
							break;
						}
					}
					else if(ob->IsTriangle()){
						if(hitTriangle(pt,lightdir,*(SceneTriangle*)obj,t)){
							shadow=true;
							break;
						}
					}
					else if(ob->IsModel()){
						if(hitModel(pt,lightdir,*(SceneModel*)obj,t,tnum)){
							shadow=true;
							break;
						}
					}
				}

				if(mat.diffuse.x>0 || mat.diffuse.y>0 || mat.diffuse.z>0){
					float dot = lightdir.Dot(n);
					if(dot>0){
						color= color + mat.diffuse*light.color*dot;
					}

				}

			//if not in shadow
			if(!shadow){
			float reflect = 2.0f * (n.Dot(lightdir)); 
			Vector phongDir = lightdir - n.Scale(reflect); 
			float phongTerm = max(phongDir.Dot(vrd), 0.0f) ; 
			Vector phongTermV = mat.specular*powf(phongTerm, mat.shininess);
			color =(color + light.color *phongTermV) * 1/(light.attenuationConstant + light.attenuationLinear*lighdist+light.attenuationQuadratic*lighdist*lighdist);
			}

			}
			return Vector(color.x,color.y,color.z);
		}
		else{
			return m_Scene.GetBackground().color;
		}
		
		/*
		if ((screenX < 0 || screenX > WINDOW_WIDTH - 1) ||
			(screenY < 0 || screenY > WINDOW_HEIGHT - 1))
		{
			// Off the screen, return black
			return Vector (0.0f, 0.0f, 0.0f);
		}

		// Until this function is implemented, return white
		return Vector (1.0f, 1.0f, 1.0f);
		*/
	}
};

#endif // RAYTRACE_H
