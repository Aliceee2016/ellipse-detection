#pragma once
#include <iomanip>
#include <iostream>
#include <cv.h>
#include <highgui.h>
//#include <opencv2/opencv.hpp>
#include<direct.h>    
#include<io.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <windows.h> 
//#include "highgui.h"
//#include "cv.h"

using namespace std;
using namespace cv;
string time_day();
string time_now();
void videoToImages(int period, int fileCount, string videoFileName, string resultFileName);
void makeDirectoty(string dirName);
void applyClahe(string in_folder, string out_folder);
void processVideos(string in_folder, string out_folder);
Mat MyCanny3(Mat src, int *thresholds,double edgePercent, double aperture_size = 3);
void getThreshold(Mat src, int *thresholds, double edgePercent, int aperture_size = 3);
void drawRectangle(string in_folder, string out_folder, int x, int y, int width, int height);
void houghCircle(string in_path, string out_path, double edgePercent, double param2, int maxRadius, int minRadius, int minDist);
void houghCircleTxt(string in_path, string out_path, double edgePercent, double param2, int maxRadius, int minRadius, int minDist);
void houghCircleOSTU(string in_path, string out_path, double edgePercent, double param2, int maxRadius, int minRadius, int minDist);
void applyPyramid(string in_folder, string out_folder);
void cutImage(string in_folder, string out_folder, int x, int y, int width, int height);
bool cmp(string a, string b);
void addOffset(string in_folder, string out_folder, double x, double y);
void detectEllipsedir(string in_path, string out_path, int slider_pos);

void drawPoint();

//void edgeForImage(double edgePercent);
//void findEdge(string image_folder, string out_folder, double edgePercent);
//void processVideos();
//void cutImage(int x1, int y1, int x2, int y2);
//void findROI();
//void findROI(string in_folder, string out_folder, int x, int y, int width, int height);
//void testHough();
//void test_min_dist();
//void applyHoughLine();
//void applyPyramid();
//void applyHoughCircle();
//void findSquare();
//void preprocessing();
//void preprocessWaysHough();
//void cutSquare(string in_folder, string out_folder);
//void preprocessing3_20();
