#include "Camera.h"

Camera::Camera(GLdouble x, GLdouble y, GLdouble z, GLdouble angle_x, GLdouble angle_y,GLdouble lookRad, GLfloat fov,GLfloat aspect,GLfloat nearPlane,GLfloat farPlane){
	Camera::fov=fov;
	Camera::aspect=aspect;
	Camera::nearPlane=nearPlane;
	Camera::farPlane=farPlane;
	Camera::angle_x=angle_x*DEGTORAD;
	Camera::angle_y=angle_y*DEGTORAD;
	Camera::x=x;
	Camera::y=y;
	Camera::z=z;
	Camera::lookRad=lookRad;
	upX=0;
	upY=1;
	upZ=0;
}
void Camera::MoveF(){	
	x+=.5*sin(angle_y)*sin(angle_x);
	z+=.5*sin(angle_y)*cos(angle_x);
	UpdateLook();
}
void Camera::RotateL(){
	angle_x+=.05;
	UpdateLook();
}
void Camera::RotateR(){
	angle_x-=.05;
	UpdateLook();
}

void Camera::RotateU(){
	angle_y-=.05;
	UpdateLook();
}
void Camera::RotateD(){
	angle_y+=.05;
	UpdateLook();
}

void Camera::MoveB(){
	x-=.5*sin(angle_y)*sin(angle_x);
	z-=.5*sin(angle_y)*cos(angle_x);
	UpdateLook();
}
void Camera::MoveU(){
	y+=.5;
	lookY+=.5;
}
void Camera::MoveD(){
	y-=.5;
	lookY-=.5;
}
void Camera::UpdateLook(){
	lookX=x+lookRad*sin(angle_y)*sin(angle_x);
	lookY=y+lookRad*cos(angle_y);
	lookZ=z+lookRad*sin(angle_y)*cos(angle_x);
}