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
	//num how many parts we divide the a-axis to;
	int getPointsAtEllipse(vector<Vec6f> ellipses, vector<Vec4f> &points, int num);
	int getPointsAtEllipse(vector<Vec6f> ellipses, vector<Vec4f> &points, int num, float z_start);
	void getMeshEllipse(string infilename, string outfilename);
	void EllipseMeshTest();
	//void two_Ellipse(string infilename, string outfilename);
	void two_Ellipse(string infilename, vector<Vec6f> &left_ellipse, vector<Vec6f> &right_ellipse);
	void two_Ellipse_TEST();
	//void one_Ellipse(string infilename, string outfilename);
	void one_Ellipse(string infilename, vector<Vec6f> &one_ellipse);
	void getVsetsEllipseUpdate(string infile, string outfile, int divide, int &num_of_points, Vec2i &number_of_ellipse);
	void filterEllipse(vector<Vec6f> &ellipses);
	void filterEllipseJUSTspline(vector<Vec6f> &ellipses);
	void combineEllipse(vector<Vec6f> &one, vector<Vec6f> &left, vector<Vec6f> &right, vector<Vec4f> &points,vector<Vec3i> &fsets);
	void getMeshFromPointsAndFsets(string resultFile,vector<Vec4f> points, vector<Vec3i> fsets);
	void filterAllEllipse(vector<Vec6f> &one, vector<Vec6f> &left, vector<Vec6f> &right);
	void cubicSmooth7Ellipse(vector<Vec6f> &ellipses);




	void getEllipseInfoOne(string infileName, vector<Vec6f> &one_ellipse, vector<string> imgNameSet);
	//void two_Ellipse(string infilename, vector<Vec6f> &left_ellipse, vector<Vec6f> &right_ellipse);
	void getEllipseInfoTwo(string infileName, vector<Vec6f> &left_ellipse, vector<Vec6f> &right_ellipse, vector<string> imgNameSet);
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
	//new add
	void getfSets(string outfileName,int number_of_points,int number_of_ellipse);
	void getfSets(vector<Vec3i> &fsets, int number_of_points, int number_of_ellipse, int point_start);


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
float image_name_float(string str_name);