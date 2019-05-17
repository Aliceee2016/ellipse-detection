// EllipseDetection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "draw3D.h"
#include "common.h"
#include "preprecessing.h"
#include "GenerateObj.h"
#include "process.h"
#include "veinReconstruction.h"
using namespace std;
//using namespace cv;
//#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>


//#define minwidth 30
cv::Size2f a;
const decltype(a.width) minwidth = 30;  //if you dont know the type of width 
#define minheight  30                   
#define maxwidth   100
#define maxheight  100

int slider_pos = 30;

IplImage *image02 = 0, *image03 = 0, *image04 = 0, *image05 = 0;
void process_image(int h);
void taskbarActivity();
int main(int argc, char** argv)
{
	CGenerateObj generator;
	string infile = "resource\\test\\ellipse_info_test.txt";
	string outfile = "resource\\test\\vsets.txt";
	//cout << sin(30) << endl;
	generator.getMeshEllipse();
	//drawPoint();
	//taskbarActivity();
	//const char* filename = rgc == 2 ? argv[1] : (char*)"2.jpg";
	//detectEllipse();
	//string in_path = "C:\\Users\\guixi\\Documents\\AVesselReconstructionResult\\2019-5-7\\2019-5-7_10-49-44\\image_roi\\";
	//string in_path = "resource\\imageMarch\\ROI_CLAHE\\oneImage\\2019-03-18_12-36-12\\";
	//	//string out_path = "C:\\Users\\guixi\\Documents\\AVesselReconstructionResult\\2019-5-7\\2019-5-7_10-49-44\\ellipse_detection_fixedThreshold_";
	//string out_path = "resource\\imageMarch\\ellipseDetection\\";
	//makeDirectoty(out_path);
	//out_path = out_path + "2019-03-18_12-36-12\\";
	//makeDirectoty(out_path);
	////for (int i = 30; i < 150; i += 5) {
	//int i = 30;
	//	string out_dir = out_path + to_string(i) + "\\";
	//	cout << out_dir << endl;
	//	detectEllipsedir(in_path, out_dir,i );
		//cvEllipse
	//}
	
	
	system("pause");
	return 0;
}

// Define trackbar callback functon. This function find contours,
// draw it and approximate it by ellipses.
void taskbarActivity() {
	//string filename = "C:\\Users\\guixi\\Documents\\AVesselReconstructionResult\\2019-5-7\\2019-5-7_10-49-44\\image_roi\\img_2.jpg";
	string filename = "resource\\imageMarch\\ROI_CLAHE\\oneImage\\2019-03-18_12-36-12\\img_34.jpg";
	if ((image03 = cvLoadImage(filename.c_str(), 0)) == 0)
		return ;

	// Create the destination images
	image02 = cvCloneImage(image03);
	image04 = cvCloneImage(image03);
	image05 = cvCloneImage(image03);

	// Create windows.
	cvNamedWindow("Source", 1);
	cvNamedWindow("Result", 1);

	// Show the image.
	cvShowImage("Source", image03);

	// Create toolbars. HighGUI use.
	cvCreateTrackbar("Threshold", "Result", &slider_pos, 255, process_image);

	process_image(0);

	// Wait for a key stroke; the same function arranges events processing                
	cvWaitKey(0);
	cvReleaseImage(&image02);
	cvReleaseImage(&image03);

	cvDestroyWindow("Source");
	cvDestroyWindow("Result");

}
void process_image(int h)
{
	CvMemStorage *stor;
	CvSeq *cont;
	CvBox2D32f *box;
	CvPoint *PointArray;
	CvPoint2D32f *PointArray2D32f;

	stor = cvCreateMemStorage(0);
	cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), stor);

	cvThreshold(image03, image02, slider_pos, 255, CV_THRESH_BINARY);

	cvFindContours(image02, stor, &cont, sizeof(CvContour),
		CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

	cvZero(image02);
	cvZero(image04);

	//绘制所有轮廓并用椭圆拟合
	for (;cont;cont = cont->h_next)
	{
		int i;
		int count = cont->total;//轮廓个数
		CvPoint center;
		CvSize size;

		/*个数必须大于6，这是cvFitEllipse_32f的要求*/
		if (count<6)
		{
			continue;
		}

		//分配内存给点集
		PointArray = (CvPoint *)malloc(count * sizeof(CvPoint));
		PointArray2D32f = (CvPoint2D32f*)malloc(count * sizeof(CvPoint2D32f));

		//分配内存给椭圆数据
		box = (CvBox2D32f *)malloc(sizeof(CvBox2D32f));

		//得到点集（这个方法值得借鉴）
		cvCvtSeqToArray(cont, PointArray, CV_WHOLE_SEQ);

		//将CvPoint点集转化为CvBox2D32f集合
		for (i = 0;i<count;i++)
		{
			PointArray2D32f[i].x = (float)PointArray[i].x;
			PointArray2D32f[i].y = (float)PointArray[i].y;
		}

		//拟合当前轮廓
		cvFitEllipse(PointArray2D32f, count, box);

		//绘制当前轮廓
		cvDrawContours(image04, cont, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255),
			0, 1, 8, cvPoint(0, 0));

		//将椭圆数据从浮点转化为整数表示
		center.x = cvRound(box->center.x);
		center.y = cvRound(box->center.y);
		size.width = cvRound(box->size.width*0.5);
		size.height = cvRound(box->size.height*0.5);
		box->angle = -box->angle;

		//画椭圆
		cvEllipse(image04, center, size, box->angle, 0, 360, CV_RGB(0, 0, 255), 1, CV_AA, 0);

		free(PointArray);
		free(PointArray2D32f);
		free(box);
	}
	cvShowImage("Result", image04);

}

//double threshold1 = 50;
//double threshold2 = 150;
//int apertureSize = 3;//aperture size for the Sobel() operator.
//int point_num_min = 100; //least point in a contour
//
//int main(int argc, char** argv)
//{
//	//if (argc < 2)
//	//	return -1;
//	cv::Mat image1 = cv::imread("resource\\test\\img_2.jpg");
//	cv::Mat image2 = image1;
//	cv::namedWindow("canny_edge1", 1);
//	cv::cvtColor(image1, image1, CV_BGR2GRAY);
//	cv::GaussianBlur(image1, image1, cv::Size(7, 7), 1.5, 1.5);
//	cv::Canny(image1, image1, threshold1, threshold2, apertureSize, false);
//	cv::imshow("canny_edge1", image1);
//	std::vector<std::vector<cv::Point> > contours1;
//	std::vector<cv::RotatedRect> contours01;
//
//	cv::findContours(image1, contours1, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
//
//	cv::Mat ellipse_contour;
//
//	cv::Mat allcontours(image1.size(), CV_8U, cv::Scalar(255));
//	//ellipse_contour.create(image1.size(), image1.type());          //two types of initiation
//	ellipse_contour = image2;
//	contours01.reserve(contours1.size());
//
//	for (std::size_t k = 0; k < contours1.size(); k++)
//	{
//
//		int count = contours1[k].size(); // This is point's number in the contour
//		if (count > point_num_min)   //  the least points to form a contour
//			contours01.push_back(cv::fitEllipse(contours1[k]));
//	}
//
//
//	cv::drawContours(allcontours, contours1, -1, CV_RGB(0, 0, 0), 1, 8, cv::noArray(), contours1.size(), cvPoint(0, 0));
//
//
//	cv::Size2f min;
//	cv::Size2f max;
//	min.width = minwidth;
//	min.height = minheight;
//	max.width = maxwidth;
//	max.height = maxheight;
//
//	for (auto a : contours01)
//	{
//		auto ellipse_size = a.size;
//		if (
//			ellipse_size.width < max.width && ellipse_size.height < max.height
//			&&    ellipse_size.width > min.width && ellipse_size.height > min.height
//			)
//		{
//			cv::ellipse(ellipse_contour, a, CV_RGB(255, 255, 255));
//			std::cout << "size = " << a.size << " , "
//				<< "center =" << a.center << std::endl;
//		}
//
//	}
//	cv::namedWindow("drawcontour", 1);
//	cv::namedWindow("ellipsecontour", 1);
//	cv::imshow("drawcontour", allcontours);
//	cv::imwrite("resource\\test\\img_1_allcontours.jpg", allcontours);
//	cv::imshow("ellipsecontour", ellipse_contour);
//	cv::imwrite("resource\\test\\img_1_ellipse_contours.jpg", ellipse_contour);
//	cv::waitKey(0);
//	system("pause");
//	return 0;
//}

//int main(int argc, char**argv)
//{
//	cout << "Start!" << endl;
//	//labelTheVein();
//	string img_file = "resource\\veinReconstruction\\data_img\\";
//	string txt_file = "resource\\veinReconstruction\\res_vien_obj_select.txt";
//	string out_path = "resource\\veinReconstruction\\vein_3\\";
//	//labelTheVein(img_file, txt_file, out_path);
//	string test_dir = "resource\\imageMarch\\ROI_CLAHE\\oneImage\\2019-03-18_13-04-55\\";
//	//string test_dir = "resource\\imageMarch\\test\\";
//	//string test_res_fir = "resource\\imageMarch\\result_square\\res8\\";
//	string cut_res_dir = "resource\\imageMarch\\hough\\oneImage\\2019-03-18_13-04-55\\";
//	//makeDirectory(test_res_fir);
//
//	//int x = 405;
//	//int y = 185;
//	//int width = 1110;
//	//int height = 695;
//	int x = 115;
//	int y = 50;
//	int width = 640;
//	int height = 410;
//	//for(;x< 200,y<100;x+=5,y+=5)
//		//drawRectangle(test_dir, test_res_fir, x, y, width, height);
//	//cutImage(test_dir, cut_res_dir,x, y, width, height);
//	string hough_in = "resource\\imageMarch\\square\\2019-03-18_13-04-55\\";
//	string hough_out = "resource\\imageMarch\\hough\\oneImage\\2019-03-18_13-04-55_different_txt\\";
//	double edgePercent = 0.7;
//	double param2 = 55;
//	int maxRadius = 55;
//	int minRadius = 30;
//	int minDist = 500;
//	houghCircle(hough_in, hough_out, edgePercent, param2, maxRadius, minRadius, minDist);
//	
//
//
//	//processing4_4();
//	//processingTwoImage();
//	//reduceError();
//
//
//	//CGenerateObj genObj;
//	//string outfileName = "resource\\imageFeb\\PrepareforReconstruction\\data_txt\\fsets.txt";
//	//string vetexfile = "resource\\imageFeb\\PrepareforReconstruction\\data_txt\\vetex.txt";
//	//string fsetsfile = "resource\\imageFeb\\PrepareforReconstruction\\data_txt\\fsets.txt";
//	//string meshfile = "resource\\imageFeb\\PrepareforReconstruction\\data_txt\\mesh3.txt";
//	//string objfile = "resource\\mesh\\test_vessel\\mesh3.obj";
//	//int numCircle = 22;
//	//int num = 24;
//	
//	string veinTxtFile = "resource\\veinReconstruction\\vein_3\\res_vien_obj_select_300.txt";
//	string infileName = "resource\\imageFeb\\PrepareforReconstruction\\reduceError\\artery_reduce.txt";
//	//string infileName = "resource\\imageFeb\\PrepareforReconstruction\\data_txt\\info_addOffset.txt";
//	string outPath = "resource\\imageFeb\\PrepareforReconstruction\\res\\";
//	
//	//float beginZ = 0;
//	//float offset = 400;
//	//int smoothMethod = 37;
//	//int method[7] = { 3,5,7,25,27,35,37 };
//	//for (int i = 0; i < 7; i++) {
//	//	int smoothMethod = method[i];
//	//	genObj.getObj(infileName, outPath, smoothMethod);
//	//}
//	//genObj.getObj(infileName, outPath, 37);
//	//41
//	//genObj.getObj(veinTxtFile, outPath, 37);
//	//genObj.getDynamicObj(infileName, outPath, 7);
//
//
//	//int method[7] = { 3,5,7,25,27,35,37 };
//	//for (int i = 0; i < 7; i++) {
//	//	int smoothMethod = method[i];
//	//	genObj.getDynamicObj(infileName, outPath, smoothMethod);
//	//	//genObj.getObj(infileName, outPath, smoothMethod);
//	//}
//
//
//	//genObj.getDynamicObj(infileName, outPath, 7);
//	
//	//genObj.readFile(infileName1, outfileName);
//	//processing3_26();
//	//preprocessing3_20();
//	//preprocessing();
//	//processVideos();
//	//for (double i = 0.05; i <= 1.0; i += 0.05) {
//	//	double edgePercent = i;
//	//	edgeForImage(edgePercent);
//	//}
//	//double edgePercent = 0.1;
//
//	//edgeForImage(0.45);
//	//edgeForImage(0.50);
//	//edgeForImage(0.55);
//
//	//720, 610,300,320
//	//findROI();
//
//	//testHough();
//	//test_min_dist();
//	//applyClahe();
//	//applyHoughLine();
//
//	//applyPyramid();
//
//	//Mat img0 = imread("resource\\applyClahe\\2019-02-25_12-05-26\\img_1.jpg");
//	//Mat img = imread("resource\\applyPyramid\\2019-02-25_12-05-26_2\\img_1.jpg");
//	//Mat img1 = imread("resource\\applyPyramid\\2019-02-25_12-05-26\\img_1.jpg");
//
//	//cout << img0.cols << " " << img0.rows << endl;
//	//cout << img.cols << " " << img.rows << endl;
//	//cout << img1.cols << " " << img1.rows << endl;
//	//applyHoughCircle();
//	//findSquare();
//	//applyHoughCircle();
//
//	//3-26 showing mesh test
//	//glutInit(&argc, argv);
//	init();
//	//glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
//	//glutInitWindowPosition(320, 180);
//	//glutInitWindowSize(1280, 720);
//	//glutReshapeFunc(&myReshape);
//	//glutKeyboardFunc(&myKeyboard);
//	//glutMouseFunc(&myMouse);
//	//glutMotionFunc(&myMotion);
//	////resource\imageFeb\PrepareforReconstruction\res\mesh
//	//string wholePath = "resource\\imageFeb\\PrepareforReconstruction\\res\\mesh\\mesh_";
//	//glutDisplayFunc(display);
//	//glutIdleFunc(myIdle);
//	//glutMainLoop();
//	
//
//	//glutInit(&argc, argv);
//	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//	//glutInitWindowSize(400, 400);
//	//glutInitWindowPosition(100, 100);
//	//glutCreateWindow("OpengGL");
//	//init();
//	////glutDisplayFunc(DrawCircle);
//	//glutDisplayFunc(display);
//	//glutReshapeFunc(reshape);
//	////glutKeyboardFunc(keyboard);
//	//glutMouseFunc(mouse);
//	//glutMainLoop();
//
//	//preprocessing();
//	//adjustThreshold();
//	//string in_folder = "resource\\imageMarch\\findROI\\2019-03-18_12-36-12\\test_image\\";
//	//string out_folder = "resource\\imageMarch\\findROI\\2019-03-18_12-36-12\\test_res2\\";
//	//makeDirectoty(out_folder);
//	//int x = 465;
//	//int y = 180;
//	//int width = 988;
//	//int height = 702;
//
//	//string in_folder = ""
//
//	//for (int i = 5, j = 0; j < 61; j ++) {
//
//	//	width += i;
//	//	height += i;
//	//	cout << y << " ";
//	//	cout << "processing " << x << " " << y << " " << width << " " << height << endl;
//		//string out = out_folder + "_x_" + to_string(x) + "_y_" + to_string(y) + "_width_" + to_string(width) + "_height_" + to_string(height);
//		//makeDirectoty(out);
//		//processVideos(in_folder,out_folder);
//		//findROI(in_folder, out_folder, x, y, width, height);
//		
//	//}
//	//string out = out_folder + "x_" + to_string(x) + "_y_" + to_string(y) + "_width_" + to_string(width) + "_height_" + to_string(height) +"\\";
//	//makeDirectoty(out);
//	////processVideos(in_folder,out_folder);
//	//fingROI(in_folder, out, x, y, width, height);
//
//
//	//preprocessWaysHough();
//
//	cout << "END!" << endl;
//	system("pause");
//	return 0;
//}

