#pragma once
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "common.h"
#include <time.h>
#include <math.h>
#include "interpolation.h"

using namespace alglib;
using namespace std;
using namespace cv;
#define MINVAL 0.000001


class CGenerateObj
{
public:
	CGenerateObj();
	CGenerateObj(int num);
	CGenerateObj(string in_file, string out_file, int num, float beginZ);
	void process();
	~CGenerateObj();
	void getObj(string infileName, string outPath, int smoothMethod = 0);
	void getDynamicObj(string infileName, string outPath, int smoothMethod = 0);
	
	void getVsetsEllipse(string infilename, string outfilename,int divide,int &num_of_points, int &number_of_ellipse);
	int getPointsAtEllipse(vector<Vec6f> ellipses, vector<Vec4f> &points, int num);
	void getMeshEllipse(string infilename, string outfilename);
	void getMeshEllipse();
private:
	int ip_num;
	int numOfCircles;
	int numParts_of_circle;
	float beginZ;
	float slice_dis;
	float x_offset ;
	float y_offset ;
	int smoothMethod;
	fstream flog;
	//void readFile(string infileName, string outfileName);
	void readFile(string infileName, string outfileName, int smoothMethod);
	void getvSets(string infileName, string outfileName, int smoothMethod = 0);
	void getfSets(string outfileName);
	void getfSets(string outfileName,int number_of_points,int number_of_ellipse);
	void getMesh(string meshFile, string fsetsFile, string vextexFile);
	void getPointAtCircle(vector<Vec4f> circles, vector<Vec4f> &points, int number);
	//void getPointAtCircle(vector<Vec4f> circles, vector<Vec4f> &points, int number);
	void saveInfo(string outPath);
	void linearSmooth3(vector<float> &x , int N );  //3
	void linearSmooth5(vector<float> &x , int N );  //5
	void linearSmooth7(vector<float> &x , int N );  //7
	void quadraicSmooth5(vector<float> &x , int N); //25
	void quadraicSmooth7(vector<float> &in, int N); //27
	void cubicSmooth5(vector<float> &in, int N);    //35
	void cubicSmooth7(vector<float> &in, int N);    //37
	void cubicSpline(real_1d_array x, vector<float> &in, int N, int ip_num);
	void readDynamicFile(string infileName, string outfileName, int smoothMethod, int r_offset );
	void cubicSplineForEllipse(real_1d_array x, vector<float> &in, int N, int dis);

};

float angle_to_radian(float degree, float min, float second);
float str2float(string num);