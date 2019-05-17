#pragma once
#include <GL/freeglut.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class CObjLoader
{
public:
	CObjLoader();
	CObjLoader(string filename);
	~CObjLoader();
	void Draw();

private:
	vector<vector<GLfloat>> vSets;
	vector<vector<GLint>> fSets;
	void convertToTriangle(string filename);
	void processing();

};

