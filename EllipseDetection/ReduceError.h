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
#include <algorithm>


using namespace std;
using namespace cv;
class CReduceError
{
public:
	CReduceError();
	~CReduceError();
	float Precision;
	float VeinPrecision;
	void readFile(string infile, string outfile);
	void writeResult(string outfile);
	void reduceVeinError(string infile, string outfile);
};

