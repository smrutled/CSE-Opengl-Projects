#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "glut.h"
#include "Definitions.h"
#include "Camera.h"
#include "MeshObject.h"
#include "MeshBox.h"
#include "MeshCylinder.h"
#include "MeshPlane.h"
#include "MeshCone.h"
#include "MeshModel.h"

#include <fstream>
using namespace std;

#include "loaders.h"
#include "mesh.h"

#define MIN2(a,b) (((a) < (b))?(a):(b))
#define MAX2(a,b) (((a) > (b))?(a):(b))

// window parameters
GLint winId;
int width=SCREENWIDTH; 
int	height=SCREENHEIGHT;
int shade=M_FLAT;
bool norm=false;
int normtype=M_NORMPF;

int mouse_x		= 0;
int mouse_y		= 0;

Camera* cam;
vector<MeshObject*> objects;

// lighting parameters
GLfloat AmbientLight[]		= { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat DiffuseLight[]		= { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat SpecularLight[]		= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPos[]			= {-50.0f,50.0f,100.0f,1.0f};

// scene parameters
Vec3f bbxMin, bbxMax, bbxCenter;

// mesh model
Mesh* model = 0;

// models
static const char* modelfile[] = {
	"./Models/al.obj",
	"./Models/temple.obj",
	"./Models/dragon-1500.obj",
	"./Models/3DEye.obj",
};

/* 
 * Facilities for drawing models. The functions below show:
 * - How to use ILM(Industrial Light & Magic)'s math library
 * - Mesh data structure
 * - How to draw the mesh in immediate mode
 */

/* 
 * Compute the bounding box of the model
 */

void calc_bbox(Vec3f& bbxmin, Vec3f& bbxmax)
{
	vector<Vec3f>& v = model->m_v;

	if(v.empty()) return;		

	bbxmax = bbxmin= v[0];

	for(unsigned int i = 1;i < v.size();i++)
	{	
		bbxmin.x = MIN2(bbxmin.x, v[i].x);
		bbxmin.y = MIN2(bbxmin.y, v[i].y);
		bbxmin.z = MIN2(bbxmin.z, v[i].z);

		bbxmax.x = MAX2(bbxmax.x, v[i].x);
		bbxmax.y = MAX2(bbxmax.y, v[i].y);
		bbxmax.z = MAX2(bbxmax.z, v[i].z);
	}
}

/* 
 * Load a model from a file
 */

void load_model(const char* name)
{
	if(model) delete model;
	model = ObjLoader::load(name);
	calc_bbox(bbxMin, bbxMax);
	bbxCenter = (bbxMin+bbxMax)*0.5f;
}


void renderMesh(MeshObject* ob){
	vector<Vec3f>& v=ob->v;
	vector<Vec3f>& vcenter=ob->vcenter;
	vector<Vec3i>& vi=ob->vi;
	vector<Vec3f>& n=ob->n;
	if(normtype==M_NORMPF)
		n=ob->n;
	else if(normtype==M_NORMPV)
		n=ob->nv;
	else if(normtype==M_NORMPVA)
		n=ob->nva;


	if(shade==M_NONE){
	for (int i=0; i < vi.size(); i++ )
	{
		glBegin(GL_LINE_LOOP);
		Vec3f offset = (v[vi[i].x] - bbxCenter);
		glNormal3fv(&n[vi[i].x].x);
		glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		glVertex3fv(&v[vi[i].x].x);
		glNormal3fv(&n[vi[i].y].x);
		offset = (v[vi[i].y] - bbxCenter);
		glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		glVertex3fv(&v[vi[i].y].x);
		glNormal3fv(&n[vi[i].z].x);
		offset = (v[vi[i].z] - bbxCenter);
		glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		glVertex3fv(&v[vi[i].z].x);
		glEnd();
	}
	}
	else{
	glBegin(GL_TRIANGLES);
	for (int i=0; i < vi.size(); i++ )
	{
/*
		if(!t.empty() && !ti.empty())
			glTexCoord2fv(&t[ti[i]].x);
		*/

		// assign some funny color to each vertex
		Vec3f offset = (v[vi[i].x] - bbxCenter);
		glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		glNormal3fv(&n[vi[i].x].x);
		glVertex3fv(&v[vi[i].x].x);
		offset = (v[vi[i].y] - bbxCenter);
		glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		glNormal3fv(&n[vi[i].y].x);
		glVertex3fv(&v[vi[i].y].x);
		offset = (v[vi[i].z] - bbxCenter);
		glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		glNormal3fv(&n[vi[i].z].x);
		glVertex3fv(&v[vi[i].z].x);
	}
	glEnd();
	}

	if(norm && normtype==M_NORMPF){
	glColor3f(0,1,1);
	V3f vec;
	for (int j=0; j < vi.size(); j++ ){
	glBegin(GL_LINES);
		glVertex3fv(&vcenter[j].x);
		vec=vcenter[j]+n[j];
		glVertex3fv(&vec.x);
		glEnd();
	}
	}
	if(norm && (normtype==M_NORMPV || normtype==M_NORMPVA)){
		glColor3f(0,1,1);
		V3f vec;
		for (int j=0; j < v.size(); j++ ){
		glBegin(GL_LINES);
		glVertex3fv(&v[j].x);
		vec=v[j]+n[j];
		glVertex3fv(&vec.x);
		glEnd();
	}
	}
	

}

// ================================================================================
// Main program functions

void shut()
{
	if(model) delete model;
}



void menu(int menuId)
{
	switch(menuId){
	case M_SMOOTH:
		shade=M_SMOOTH;
		glShadeModel(GL_SMOOTH);
		break;
	case M_FLAT:
		shade=M_FLAT;
		glShadeModel(GL_FLAT);
		break;
	case M_NONE:
		shade=M_NONE;
		glShadeModel(GL_NONE);
		break;
	case M_NORMPF:
		normtype=M_NORMPF;
		break;
	case M_NORMPV:
		normtype=M_NORMPV;
		break;
	case M_NORMPVA:
		normtype=M_NORMPVA;
		break;
	case M_NORM_ON:
		norm=true;
		break;
	case M_NORM_OFF:
		norm=false;
		break;
	case M_QUIT:
		glutDestroyWindow(winId);
	}

	glutPostRedisplay();
}

void glutMenus (void)
{	
	GLint shadingMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Flat", M_FLAT);
	glutAddMenuEntry("Smooth", M_SMOOTH);
	glutAddMenuEntry("None", M_NONE);
	GLint normalsMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Per Face", M_NORMPF);
	glutAddMenuEntry("Per Vertex Average", M_NORMPV);
	glutAddMenuEntry("Per Vertex Weighted", M_NORMPVA);
	GLint normalsVMenu = glutCreateMenu(menu);
	glutAddMenuEntry("ON", M_NORM_ON);
	glutAddMenuEntry("OFF", M_NORM_OFF);
	glutCreateMenu(menu);
	glutAddSubMenu("Normal Visual", normalsVMenu);
	glutAddSubMenu("Normal", normalsMenu);
	glutAddSubMenu("Shading", shadingMenu);
	glutAddMenuEntry("Quit", M_QUIT);	

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init()
{	
	// use black to clean background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	// always re-normalize normal (due to scale transform)
	glEnable(GL_NORMALIZE);

	// lighting (disabled)
	// To achieve lighting effect
	//	- enable lighting
	//	- setup lighting parameters
	//	- setup materials
	//	- attach normals to each vertex
	
	glDisable(GL_LIGHTING);
	/* 
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	glEnable(GL_LIGHT0);
	*/

	// smooth shading
	glShadeModel(GL_FLAT);

	// load the first model by default
	
	// setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	cam->aspect = (double)width/(double)height;

	gluPerspective(cam->fov, cam->aspect,cam->nearPlane, cam->farPlane);

	// setup viewing matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(cam->x, cam->y,cam->z,cam->lookX,cam->lookY, 
		cam->lookZ, cam->upX, cam->upY, 
		cam->upZ);

	// set up menus
	glutMenus();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	for_each (objects.begin(), objects.end(), renderMesh);


	glPopMatrix();

	glutSwapBuffers();
}

void keyboard(unsigned char key , int x , int y){
	
	
	if(key=='s'){
		cam->MoveU();
	}
	if(key=='a'){
		cam->MoveD();
	}

	if(key=='w'){
		cam->RotateU();
	}

	if(key=='q'){
		cam->RotateD();
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
		gluLookAt(cam->x, cam->y,cam->z,cam->lookX,cam->lookY, 
		cam->lookZ, cam->upX, cam->upY, 
		cam->upZ);
	glutPostRedisplay();
}
void keyboardsp(int key , int x , int y){

	if(key==GLUT_KEY_LEFT){
		cam->RotateL();
	}
	if(key==GLUT_KEY_UP){
		cam->MoveF();
	}
	if(key==GLUT_KEY_RIGHT){
		cam->RotateR();
	}
	if(key==GLUT_KEY_DOWN){
		cam->MoveB();
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
		gluLookAt(cam->x, cam->y,cam->z,cam->lookX,cam->lookY, 
		cam->lookZ, cam->upX, cam->upY, 
		cam->upZ);
	glutPostRedisplay();

}


void reshape(int x, int y)
{
    width  = x;
	height = y;

	if (height == 0)		// not divided by zero
		height = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0,0,width,height);

	cam->aspect = (float)width/(float)height;

	gluPerspective(cam->fov, cam->aspect,cam->nearPlane,cam->farPlane);
	glMatrixMode(GL_MODELVIEW);

}


void main(int argc, char* argv[])
{
	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-SCREENWIDTH)/2,(glutGet(GLUT_SCREEN_HEIGHT)-SCREENHEIGHT)/2);
	glutInitWindowSize(width,height);
	winId = glutCreateWindow("Mesh Viewer");
	
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardsp);
	glutDisplayFunc(display);// display function
	cam=new Camera(-1.6,5,27,-188,90,10,45.0,1.0,.1,100.0);
	cam->UpdateLook();
	objects.push_back((MeshObject*) new MeshCylinder(5,0,0,1,5,40));
	objects.push_back((MeshObject*) new MeshCylinder(5,0,10,1,5,40));
	objects.push_back((MeshObject*) new MeshCylinder(10,0,0,1,5,40));
	objects.push_back((MeshObject*) new MeshCylinder(10,0,10,1,5,40));
	objects.push_back((MeshObject*) new MeshBox(1,1,3,2,2,2));
	objects.push_back((MeshObject*) new MeshCone(5,5,0,2,2,40));
	objects.push_back((MeshObject*) new MeshCone(5,5,10,2,2,40));
	objects.push_back((MeshObject*) new MeshCone(10,5,0,2,2,40));
	objects.push_back((MeshObject*) new MeshCone(10,5,10,2,2,40));
	objects.push_back((MeshObject*) new MeshCone(7.5,4.5,5,1,5,4));
	objects.push_back((MeshObject*) new MeshBox(7.5,1.5,5,5,3,10));
	objects.push_back((MeshObject*) new MeshBox(7.5,3.75,5,2.5,1.5,5));
	objects.push_back((MeshObject*) new MeshPlane(-10,0,-10,2,2,15,15));
	objects.push_back((MeshObject*) new MeshBox(-5,.25,12,5,.5,5));
	objects.push_back((MeshObject*) new MeshBox(-5,.5,12,4,.5,4));
	objects.push_back((MeshObject*) new MeshBox(-5,.75,12,3,.5,3));
	objects.push_back((MeshObject*) new MeshBox(-5,1,12,2,.5,2));
	load_model(modelfile[0]);
	objects.push_back((MeshObject*) new MeshModel(1,3.5,0,model->m_v,model->m_vi));
	load_model(modelfile[1]);
	objects.push_back((MeshObject*) new MeshModel(-5,0,0,model->m_v,model->m_vi));
	load_model(modelfile[2]);
	objects.push_back((MeshObject*) new MeshModel(5.5,3,5,model->m_v,model->m_vi));
	load_model(modelfile[3]);
	objects.push_back((MeshObject*) new MeshModel(-5,2.10,12,model->m_v,model->m_vi));
	init();

	glutMainLoop();

	shut();
}






