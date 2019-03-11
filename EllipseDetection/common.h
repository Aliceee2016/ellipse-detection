#pragma once
#include <iomanip>
#include <iostream>
//#include <cv.h>
//#include <highgui.h>
#include <opencv2/opencv.hpp>
#include<direct.h>    
#include<io.h>
#include <vector>
#include <fstream>

#include <windows.h> 

using namespace std;
using namespace cv;

void videoToImages(int period, int fileCount, string videoFileName, string resultFileName);
void makeDirectoty(string dirName);
void processVideos();
Mat MyCanny3(Mat src, int *thresholds,double edgePercent, double aperture_size = 3);
void edgeForImage(double edgePercent);
void cutImage(int x1, int y1, int x2, int y2);
void findROI();
void testHough();
void test_min_dist();
void applyClahe();
void applyHoughLine();
void applyPyramid();
void applyHoughCircle();
void findSquare();

