#pragma once

#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "ObjLoader.h"
#define PI 3.1415926


//CObjLoader objModel = CObjLoader(filePath);

static float c = PI / 180.f;
static float r = 1.0f;
static int degree = 90;
static int oldPosY = -1;
static int oldPosX = -1;
static GLfloat dx = 0, dy = 0, dz = 0;
static GLfloat ax = 0, ay = 0, az = 0;
static GLint mx = 0, my = 0;
static GLint MouseDown = 0;
static GLfloat aspect = 1;
static int num = 0;

void setLightRes();
void init();
void display();
void display(string meshFilePath);
void reshape(int width, int height);
void moseMove(int button, int state, int x, int y);
void changeViewPoint(int x, int y);
void myIdle();
void myReshape(int width, int height);
void myKeyboard(unsigned char key, int x, int y);
void myMouse(int button, int state, int x, int y);
void myMotion(int x, int y);
void setMaterial();
void setLight();
double CalFrequency();
