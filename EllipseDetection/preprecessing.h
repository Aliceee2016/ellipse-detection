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
#include "common.h"
#include <windows.h> 

using namespace std;
using namespace cv;

#define Gamma 3

void on_trackbar(int pos);
void adjustThreshold();
void thresholdOTSU(string in_folder, string out_folder);
//void makeDirectoty(string dirName);

void applyHE(string in_folder, string out_folder);
void applyLaplace(string in_folder, string out_folder);
void applyLog(string in_folder, string out_folder);
void applyGamma(string in_folder, string out_folder);
void detectEllipse();
//void detectEllipsedir();