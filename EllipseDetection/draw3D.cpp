
#include "stdafx.h"
#include "draw3D.h"

double CalFrequency()
{
	static int count;
	static double save;
	static clock_t last, current;
	double timegap;

	++count;
	if (count <= 50)
		return save;
	count = 0;
	last = current;
	current = clock();
	timegap = (current - last) / (double)CLK_TCK;
	save = 50.0 / timegap;
	return save;
}

void setLightRes() {
	//GLfloat lightPosition[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	GLfloat light_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0, 3, 2.0, 0.0 };       
	GLfloat light_specular[] = { 1.0, 1.0, 0.0, 1.0 };       
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);        
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);    
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);       

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

}

void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//glutInitWindowSize(1000, 1000);
	//glutInitWindowPosition(320, 180);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("ObjLoader");
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	setLightRes();
	glEnable(GL_DEPTH_TEST);

	
	
}
void display(string meshFilePath) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 1, 1, 400000000);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(dx, dy, dz);
	glRotatef(ax, 1.0f, 0.0f, 0.0f);
	glRotatef(ay, 0.0f, 1.0f, 0.0f);

	setLight();
	setMaterial();
	string objfile = "resource\\mesh\\test_vessel\\mesh3.obj";
	string filePath1 = "resource\\imageFeb\\PrepareforReconstruction\\res\\2019-4-3\\testSmoothing\\2019-4-3_17-55-34_3\\mesh.obj";
	string face_human = "resource\\mesh\\faceReconstruction\\mesh_netural_triangle.obj";
	//CObjLoader objModel = CObjLoader(face_human);
	string stringName[9] = { "mesh_0.obj","mesh_5.obj","mesh_10.obj","mesh_15.obj","mesh_20.obj" ,"mesh_15.obj" ,"mesh_10.obj","mesh_5.obj","mesh_0.obj" };
	
	CObjLoader objModel = CObjLoader(meshFilePath);

	objModel.Draw();
	glPopMatrix();
	glutSwapBuffers();
}
void display()
{
	//glColor3f(1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glTranslatef(0.0f, 0.0f, -5.0f);
	//setLightRes();
	//glPushMatrix();

	//gluLookAt(r*cos(c*degree), 0, r*sin(c*degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	double FPS = CalFrequency();
	printf("FPS = %f\n", FPS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glPushMatrix();


	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 1, 1, 400000000);

	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(dx, dy, dz);
	glRotatef(ax, 1.0f, 0.0f, 0.0f);
	glRotatef(ay, 0.0f, 1.0f, 0.0f);

	setLight();
	setMaterial();
	string objfile = "resource\\mesh\\test_vessel\\mesh3.obj";
	string filePath1 = "resource\\imageFeb\\PrepareforReconstruction\\res\\2019-4-3\\testSmoothing\\2019-4-3_17-55-34_3\\mesh.obj";
	
	string path = "resource\\imageFeb\\PrepareforReconstruction\\res\\2019-4-19\\2019-4-19_16-3-11_7\\";

	string face_human = "resource\\mesh\\faceReconstruction\\mesh_netural_triangle.obj";
	//CObjLoader objModel = CObjLoader(face_human);
	string stringName[9] = { "mesh_0.obj","mesh_5.obj","mesh_10.obj","mesh_15.obj","mesh_20.obj" ,"mesh_15.obj" ,"mesh_10.obj","mesh_5.obj","mesh_0.obj" };
	string name = path + stringName[num];
	CObjLoader objModel = CObjLoader(name);

	objModel.Draw();
	glPopMatrix();
	glutSwapBuffers();
}
void setMaterial()
{
	/*
	*
	*
	GLfloat sun_mat_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat sun_mat_diffuse[]  = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat sun_mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat sun_mat_emission[] = {0.5f, 0.0f, 0.0f, 1.0f};
	GLfloat sun_mat_shininess  = 0.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT,   sun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   sun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  sun_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION,  sun_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
	*/

	/*
	*
	*/
	GLfloat earth_mat_ambient_and_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat earth_mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat earth_mat_shininess = 30.0f;
	GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient_and_diffuse);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_ambient_and_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
}

void moseMove(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		oldPosX = x; oldPosY = y;
	}
	//if(state == GLUT_DO)
}
void changeViewPoint(int x, int y)
{
	int temp = x - oldPosX;
	degree += temp;
	oldPosX = x;
	oldPosY = y;
}

void myIdle()
{
	Sleep(100);
	num++;
	if (num > 8) {
		num = 0;
	}
	//glutPostRedisplay();
	display();
}
void myReshape(int width, int height)
{
	aspect = (float)width / (height ? height : 1);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 1, 1, 400000000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);
}
void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		dz += 1.0f;
		break;
	case 's':
		dz -= 1.0f;
		break;
	case 'a':
		dx -= 1.0f;
		break;
	case 'd':
		dx += 1.0f;
		break;
	case 'q':
		dy += 1.0f;
		break;
	case 'e':
		dy -= 1.0f;
		break;
	}
}
void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_DOWN) MouseDown = 1, mx = x, my = y;
	else if (button == 3)
	{
		dz += 1.0f;
	}
	else if (button == 4)
	{
		dz -= 1.0f;
	}
	else
	{
		MouseDown = 0;
	}
}

void myMotion(int x, int y)
{
	if (MouseDown)
	{
		ax += (y - my) / 5.0f;
		ay += (x - mx) / 5.0f;
		mx = x;
		my = y;
	}
}

void setLight()
{
	static const GLfloat light_position[] = { 50.0f,50.0f,50.0f,0.0f };
	static const GLfloat light_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
	static const GLfloat light_diffuse[] = { 1.0f,0.9f,0.9f,0.0f };
	static const GLfloat light_specular[] = { 1.0f,1.0f,1.0f,0.0f };
	static const GLfloat light_direction[] = { -1.0f,-1.0f,-1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0f);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 120.0f);

												//glLightfv(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	//*/

	/*

	*
	{
	GLfloat sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat sun_light_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat sun_light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat sun_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	}
	*/
}

