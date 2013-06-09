#include "Light.h"

Light::Light(GLenum num,V3f pos, GLfloat rotrad, V3f ambient, V3f diffuse, V3f specular ){
	Light::num = num;
	for(int i=0; i<3;i++){
		Light::cpos[i]=pos[i];
		Light::rpos[i]=pos[i];
		Light::ambient[i]=ambient[i];
		Light::diffuse[i]=diffuse[i];
		Light::specular[i]=specular[i];
	}
	angle=0;
	Light::rpos[3]=1;
	Light::rpos[0]=cpos[0]+rotrad*cos(angle);
	Light::rpos[2]=cpos[2]+rotrad*sin(angle);
	Light::ambient[3]=1;
	Light::diffuse[3]=1;
	Light::specular[3]=1;
	Light::rotrad=rotrad;
	glLightfv(num, GL_AMBIENT, Light::ambient);
	glLightfv(num, GL_DIFFUSE, Light::diffuse);
	glLightfv(num, GL_SPECULAR, Light::specular);
	glLightfv(num, GL_POSITION, rpos);
}

void Light::Enable(){
	glEnable(num);
}

void Light::Disable(){
	glDisable(num);
}


void Light::RotateL(){
	angle+=.05;
	Light::rpos[0]=cpos[0]+rotrad*cos(angle);
	Light::rpos[2]=cpos[2]+rotrad*sin(angle);
}

void Light::RotateR(){
	angle-=.05;
	Light::rpos[0]=cpos[0]+rotrad*cos(angle);
	Light::rpos[2]=cpos[2]+rotrad*sin(angle);
}

void Light::UpdatePosition(){
	glLightfv(num, GL_POSITION, rpos);
}

