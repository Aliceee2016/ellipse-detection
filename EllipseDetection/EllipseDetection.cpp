// EllipseDetection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "common.h"
using namespace std;
using namespace cv;

int main()
{
	//processVideos();
	//for (double i = 0.05; i <= 1.0; i += 0.05) {
	//	double edgePercent = i;
	//	edgeForImage(edgePercent);
	//}
	//double edgePercent = 0.1;

	//edgeForImage(0.45);
	//edgeForImage(0.50);
	//edgeForImage(0.55);

	//720, 610,300,320
	//findROI();

	//testHough();
	//test_min_dist();
	//applyClahe();
	//applyHoughLine();

	//applyPyramid();

	//Mat img0 = imread("resource\\applyClahe\\2019-02-25_12-05-26\\img_1.jpg");
	//Mat img = imread("resource\\applyPyramid\\2019-02-25_12-05-26_2\\img_1.jpg");
	//Mat img1 = imread("resource\\applyPyramid\\2019-02-25_12-05-26\\img_1.jpg");

	//cout << img0.cols << " " << img0.rows << endl;
	//cout << img.cols << " " << img.rows << endl;
	//cout << img1.cols << " " << img1.rows << endl;
	//applyHoughCircle();
	//findSquare();
	applyHoughCircle();
	system("pause");
	return 0;
}

