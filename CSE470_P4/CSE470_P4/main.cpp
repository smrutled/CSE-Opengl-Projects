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
#include "stb_image.h"
#include "Light.h"
#include "MeshSphere.h"

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
GLuint tex_grass, tex_stone, tex_earth, tex_cardboard, tex_redroof, tex_gold;
bool norm=false;
bool navToggle=false;
int normtype=M_NORMPF;
int mattype=M_MAT_DIFF;
unsigned char *image;
int mouse_x		= 0;
int mouse_y		= 0;

Camera* cam;
vector<MeshObject*> objects;
vector<Light*> lights;

GLfloat mat_zero[] = { 0, 0, 0, 1 };
GLfloat mat_low[] = { 0.2, 0.2, 0.2, 1 };
GLfloat mat_high[]= { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_gold[] = { 0.8, 0.8, 0.2, 1.0 };
GLfloat mat_red[] = { .5, 0.0, 0.0, 1.0 };
GLfloat mat_blue[] = { 0.0, 0.0, 0.5, 1.0 };
GLfloat mat_green[] = { 0.0, 0.5, 0.0, 1.0 };
GLfloat mat_highshine[]= { 100.0 };
GLfloat mat_noshine[]= { 0.0 };

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
static const char* texturefile[] = {
	"./Textures/earth.jpg",
	"./Textures/stone2.jpg",
	"./Textures/grass.jpg",
	"./Textures/cardboard.jpg",
	"./Textures/redroof.jpg",
	"./Textures/gold.jpg",
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

void material_design(GLuint tex){
	if(tex==tex_grass){

	}
	else if(tex==tex_stone){
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_red);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_high);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_zero);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_low);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_noshine);
	}
	else if(tex==tex_earth){
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_green);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_high);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_zero);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_low);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_noshine);
	}
	else if(tex==tex_cardboard){
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_zero);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_high);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_zero);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_low);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_noshine);
	}
	else if(tex==tex_redroof){
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_blue);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_high);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_zero);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_blue);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_noshine);
	}
	else if(tex==tex_gold){
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_gold);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_gold);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_gold);
		glMaterialfv(GL_FRONT, GL_EMISSION,  mat_gold);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_highshine);
	}
	else if(tex==0){
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_low);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_high);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_zero);
		glMaterialfv(GL_FRONT, GL_EMISSION,  mat_zero);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_noshine);
	}

}

void loadPNG(const char* filename,  int width, int height, int bitdepth)
{
	image = stbi_load(filename, &width, &height, &bitdepth, 0);
}


void load_textures(){
	loadPNG(texturefile[0],  720, 360, 24);
	glGenTextures(1, &tex_earth);
	glBindTexture(GL_TEXTURE_2D, tex_earth);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, 720, 360, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	loadPNG(texturefile[1],  512, 512, 24);
	glGenTextures(1, &tex_stone);
	glBindTexture(GL_TEXTURE_2D, tex_stone);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	loadPNG(texturefile[2],  1024, 1024, 24);
	glGenTextures(1, &tex_grass);
	glBindTexture(GL_TEXTURE_2D, tex_grass);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	loadPNG(texturefile[3],  485, 364, 24);
	glGenTextures(1, &tex_cardboard);
	glBindTexture(GL_TEXTURE_2D, tex_cardboard);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, 485, 364, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	loadPNG(texturefile[4],  1171, 772, 24);
	glGenTextures(1, &tex_redroof);
	glBindTexture(GL_TEXTURE_2D, tex_redroof);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, 1171, 772, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	loadPNG(texturefile[5],  898, 895, 24);
	glGenTextures(1, &tex_gold);
	glBindTexture(GL_TEXTURE_2D, tex_gold);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, 898, 895, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
}
void load_model(const char* name)
{
	if(model) delete model;
	model = ObjLoader::load(name);
	calc_bbox(bbxMin, bbxMax);
	bbxCenter = (bbxMin+bbxMax)*0.5f;
}

void renderMesh(MeshObject* ob){
	vector<Vec3f> nt;
	if(normtype==M_NORMPF)
		nt=ob->n;
	else if(normtype==M_NORMPV)
		nt=ob->nv;
	else if(normtype==M_NORMPVA)
		nt=ob->nva;

	vector<Vec3f>& v=ob->v;
	vector<Vec3f>& vcenter=ob->vcenter;
	vector<Vec3i>& vi=ob->vi;
	vector<Vec3f>&	n=nt;
	vector<Vec2f>& t=ob->t;
	vector<Vec3i>& ti=ob->ti;



	if(shade==M_NONE){
		for (int i=0; i < vi.size(); i++ )
		{
			glBegin(GL_LINE_LOOP);
			Vec3f offset;
			for(int j=0; j <3; j++){
				offset = (v[vi[i][j]] - bbxCenter);
				if(normtype==M_NORMPF)
					glNormal3fv(&n[i].x);
				else
					glNormal3fv(&n[vi[i][j]].x);
				glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
				glVertex3fv(&v[vi[i][j]].x);
			}
			glEnd();
		}
	}
	else{
		if(ob->textured){
			glBindTexture(GL_TEXTURE_2D, ob->texid);
			if(mattype==M_MAT_DESIGN)
				material_design(ob->texid);
			glBegin(GL_TRIANGLES);
			for (int i=0; i < vi.size(); i++ )
			{
				Vec3f offset;
				for(int j=0; j <3; j++){
					offset = (v[vi[i][j]] - bbxCenter);
					if(normtype==M_NORMPF)
						glNormal3fv(&n[i].x);
					else
						glNormal3fv(&n[vi[i][j]].x);
					//glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
					glTexCoord2fv(&t[ti[i][j]].x);
					glVertex3fv(&v[vi[i][j]].x);
				}
			}
			glEnd();
		}
		else{
			glBindTexture(GL_TEXTURE_2D, 0);
			if(mattype==M_MAT_DESIGN)
				material_design(0);
			glBegin(GL_TRIANGLES);
			for (int i=0; i < vi.size(); i++ )
			{
				Vec3f offset;
				for(int j=0; j <3; j++){
					offset = (v[vi[i][j]] - bbxCenter);
					if(normtype==M_NORMPF){
						glNormal3fv(&n[i].x);
					}
					else{
						glNormal3fv(&n[vi[i][j]].x);
					}
					glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
					glVertex3fv(&v[vi[i][j]].x);
				}
			}
			glEnd();
		}
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
	case M_TEX_ON:
		glEnable (GL_TEXTURE_2D);
		break;
	case M_TEX_OFF:
		glDisable (GL_TEXTURE_2D);
		break;
	case M_TEX_MOD:
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		break;
	case M_TEX_DEC:
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		break;
	case M_MAT_DIFF:
		mattype=M_MAT_DIFF;
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_low);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_high);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_zero);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_low);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_noshine);
		break;
	case M_MAT_SPEC:
		mattype=M_MAT_SPEC;
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_low);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_high);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_high);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_low);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_noshine);
		break;
	case M_MAT_DESIGN:
		mattype=M_MAT_DESIGN;
		break;
	case M_LIGHT_ONE:
		lights[1]->Disable();
		break;
	case M_LIGHT_CUST:
		lights[1]->Enable();
		break;
	case M_NAV_CAM:
		navToggle=false;
		break;
	case M_NAV_LIGHT:
		navToggle=true;
		break;
	case M_QUIT:
		glutDestroyWindow(winId);
		break;
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
	GLint texturingMenu = glutCreateMenu(menu);
	glutAddMenuEntry("ON", M_TEX_ON);
	glutAddMenuEntry("OFF", M_TEX_OFF);
	GLint textureMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Replace", M_TEX_DEC);
	glutAddMenuEntry("Modulate", M_TEX_MOD);
	glutCreateMenu(menu);
	GLint materialMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Diffuse", M_MAT_DIFF);
	glutAddMenuEntry("Specular", M_MAT_SPEC);
	glutAddMenuEntry("Design", M_MAT_DESIGN);
	GLint lightMenu = glutCreateMenu(menu);
	glutAddMenuEntry("One Light", M_LIGHT_ONE);
	glutAddMenuEntry("Custom", M_LIGHT_CUST);
	GLint navMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Camera", M_NAV_CAM);
	glutAddMenuEntry("Light Source", M_NAV_LIGHT);
	glutCreateMenu(menu);
	glutAddSubMenu("Lighting", lightMenu);
	glutAddSubMenu("Materials", materialMenu);
	glutAddSubMenu("Navigation", navMenu);
	glutAddSubMenu("Normal Visual", normalsVMenu);
	glutAddSubMenu("Normal", normalsMenu);
	glutAddSubMenu("Shading", shadingMenu);
	glutAddSubMenu("Texturing", texturingMenu);
	glutAddSubMenu("Texture", textureMenu);
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



	glEnable(GL_LIGHTING);


	lights.push_back(new Light(GL_LIGHT0,V3f(0,10,5),15,V3f(1,1,1),V3f(1.0,1.0,1.0),V3f(1.0,1.0,1.0)));
	lights.push_back(new Light(GL_LIGHT1,V3f(-2,10,0),0,V3f(0,0,.5),V3f(.1,.1,.5),V3f(0,0,0)));
	lights[0]->Enable();
	lights[1]->Disable();

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
	glEnable(GL_NORMALIZE);
	glPushMatrix();
	lights[0]->UpdatePosition();
	lights[1]->UpdatePosition();

	for_each (objects.begin(), objects.end(), renderMesh);


	glPopMatrix();

	glutSwapBuffers();
}

void keyboard(unsigned char key , int x , int y){

	if(!navToggle){
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
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	gluLookAt(cam->x, cam->y,cam->z,cam->lookX,cam->lookY, 
		cam->lookZ, cam->upX, cam->upY, 
		cam->upZ);
	glutPostRedisplay();
}
void keyboardsp(int key , int x , int y){
	if(!navToggle){
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
	}
	else{
		if(key==GLUT_KEY_LEFT){
			lights[0]->RotateL();
		}
		if(key==GLUT_KEY_RIGHT){
			lights[0]->RotateR();
		}
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
	glutDisplayFunc(display);
	cam=new Camera(-1.6,5,27,-188,90,10,45.0,1.0,.1,100.0);
	cam->UpdateLook();
	init();
	load_textures();
	objects.push_back((MeshObject*) new MeshCylinder(5,0,0,1,5,40,tex_stone));
	objects.push_back((MeshObject*) new MeshCylinder(5,0,10,1,5,40,tex_stone));
	objects.push_back((MeshObject*) new MeshCylinder(10,0,0,1,5,40,tex_stone));
	objects.push_back((MeshObject*) new MeshCylinder(10,0,10,1,5,40,tex_stone));
	objects.push_back((MeshObject*) new MeshBox(1,1,3,2,2,2,tex_cardboard));
	objects.push_back((MeshObject*) new MeshCone(5,5,0,2,2,40,40,tex_redroof));
	objects.push_back((MeshObject*) new MeshCone(5,5,10,2,2,40,40,tex_redroof));
	objects.push_back((MeshObject*) new MeshCone(10,5,0,2,2,40,40,tex_redroof));
	objects.push_back((MeshObject*) new MeshCone(10,5,10,2,2,40,40,tex_redroof));
	objects.push_back((MeshObject*) new MeshCone(7.5,4.5,5,1,5,4,4,tex_gold));
	objects.push_back((MeshObject*) new MeshBox(7.5,1.5,5,5,3,10,tex_stone));
	objects.push_back((MeshObject*) new MeshBox(7.5,3.75,5,2.5,1.5,5,tex_stone));
	objects.push_back((MeshObject*) new MeshPlane(-10,0,-10,2,2,15,15,tex_grass));
	objects.push_back((MeshObject*) new MeshBox(-5,.25,12,5,.5,5,tex_gold));
	objects.push_back((MeshObject*) new MeshBox(-5,.5,12,4,.5,4,tex_gold));
	objects.push_back((MeshObject*) new MeshBox(-5,.75,12,3,.5,3,tex_gold));
	objects.push_back((MeshObject*) new MeshBox(-5,1,12,2,.5,2,tex_gold));
	objects.push_back((MeshObject*) new MeshSphere(-5,2.25,12,1,50,50,tex_earth));
	load_model(modelfile[0]);
	objects.push_back((MeshObject*) new MeshModel(1,3.5,0,model->m_v,model->m_vi));
	load_model(modelfile[1]);
	objects.push_back((MeshObject*) new MeshModel(-5,0,0,model->m_v,model->m_vi));
	load_model(modelfile[2]);
	objects.push_back((MeshObject*) new MeshModel(5.5,3,5,model->m_v,model->m_vi));


	glutMainLoop();

	shut();
}






