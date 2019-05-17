#include "stdafx.h"
#include "preprecessing.h"
#include "common.h"

IplImage *g_pGrayImage = NULL;
IplImage *g_pBinaryImage = NULL;
const char *pstrWindowsBinaryTitle = "binary image";
//void makeDirectoty(string dirName) {
//	if (_access(dirName.c_str(), 0) == -1)
//	{
//		bool flag = CreateDirectory(dirName.c_str(), NULL);
//		if (flag) {
//			cout << "Creat " << dirName << " successful!" << endl;
//		}
//		else {
//			cout << "FAIL to creat " << dirName << endl;
//		}
//	}
//	else {
//		cout << dirName << " already exists." << endl;
//	}
//
//}
void thresholdOTSU(string in_folder, string out_folder) {
	vector<string> names;
	glob(in_folder + "*.*", names);
	string saving_path = out_folder;
	makeDirectoty(saving_path);
	for (const auto & image_name : names) {
		cout << " processing " << image_name << endl;
		string name = image_name.substr(image_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		string save_name = saving_path + name + ".jpg";

		Mat src = imread(image_name);
		Mat workImg = src.clone();
		if (workImg.empty()) {
			cout << "fail to load Img" << endl;
			return;
		}
		Mat gray;
		cvtColor(workImg, gray, CV_BGR2GRAY);
		Mat dst;
		threshold(gray, dst, 0, 255, CV_THRESH_OTSU);

		//imshow("src", img);
		//imshow("gray", gray);
		//imshow("dst", dst);
		imwrite(save_name, dst);

	}
}
void applyHE(string in_folder, string out_folder) {
	vector<string> names;
	glob(in_folder + "*.*", names);
	string saving_path = out_folder;
	makeDirectoty(saving_path);
	for (const auto & image_name : names) {
		cout << " processing " << image_name << endl;
		string name = image_name.substr(image_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		string save_name = saving_path + name + ".jpg";

		Mat src = imread(image_name);
		Mat workImg = src.clone();
		if (workImg.empty()) {
			cout << "fail to load Img" << endl;
			return;
		}

		Mat src_RGB[3], dst;
		split(workImg, src_RGB);

		for (int i = 0; i < 3; i++)
		{
			equalizeHist(src_RGB[i], src_RGB[i]);
		}

		merge(src_RGB, 3, dst);
		imwrite(save_name, dst);

	}

}

void applyLaplace(string in_folder, string out_folder) {
	vector<string> names;
	glob(in_folder + "*.*", names);
	string saving_path = out_folder;
	makeDirectoty(saving_path);
	for (const auto & image_name : names) {
		cout << " processing " << image_name << endl;
		string name = image_name.substr(image_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		string save_name = saving_path + name + ".jpg";

		Mat src = imread(image_name);
		Mat workImg = src.clone();
		if (workImg.empty()) {
			cout << "fail to load Img" << endl;
			return;
		}


		Mat dst;
		Mat kernel = (Mat_<int>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
		filter2D(workImg, dst, src.depth(), kernel);
		imwrite(save_name, dst);
	}
}
void applyLog(string in_folder, string out_folder) {
	vector<string> names;
	glob(in_folder + "*.*", names);
	string saving_path = out_folder;
	makeDirectoty(saving_path);
	for (const auto & image_name : names) {
		cout << " processing " << image_name << endl;
		string name = image_name.substr(image_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		string save_name = saving_path + name + ".jpg";

		Mat src = imread(image_name);
		Mat workImg = src.clone();
		if (workImg.empty()) {
			cout << "fail to load Img" << endl;
			return;
		}
		Mat dst(src.size(), CV_32FC3);

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				dst.at<Vec3f>(i, j)[0] = log(1 + src.at<Vec3b>(i, j)[0]);
				dst.at<Vec3f>(i, j)[1] = log(1 + src.at<Vec3b>(i, j)[1]);
				dst.at<Vec3f>(i, j)[2] = log(1 + src.at<Vec3b>(i, j)[2]);
			}
		}
		normalize(dst, dst, 0, 255, CV_MINMAX);
		convertScaleAbs(dst, dst);
		//imshow("darknight_log.jpg", dst);
		imwrite(save_name, dst);
	}
}
void applyGamma(string in_folder, string out_folder) {
	vector<string> names;
	glob(in_folder + "*.*", names);
	string saving_path = out_folder;
	makeDirectoty(saving_path);
	for (const auto & image_name : names) {
		cout << " processing " << image_name << endl;
		string name = image_name.substr(image_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		string save_name = saving_path + name + ".jpg";

		Mat src = imread(image_name);
		Mat workImg = src.clone();
		if (workImg.empty()) {
			cout << "fail to load Img" << endl;
			return;
		}
		Mat dst(src.size(), CV_32FC3);

		for (int i = 0; i < src.rows;i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				dst.at<Vec3f>(i, j)[0] = pow(src.at<Vec3b>(i, j)[0], Gamma);
				dst.at<Vec3f>(i, j)[1] = pow(src.at<Vec3b>(i, j)[1], Gamma);
				dst.at<Vec3f>(i, j)[2] = pow(src.at<Vec3b>(i, j)[2], Gamma);
			}
		}
		normalize(dst, dst, 0, 255, CV_MINMAX);
		convertScaleAbs(dst, dst);
		imwrite(save_name, dst);
	}
}
void on_trackbar(int pos)
{

	cvThreshold(g_pGrayImage, g_pBinaryImage, pos, 255, CV_THRESH_BINARY);

	cvShowImage(pstrWindowsBinaryTitle, g_pBinaryImage);
}
void adjustThreshold() {
	const char *pstrWindowsSrcTitle = "original image";
	const char *pstrWindowsToolBarName = "binary image threshold";

	IplImage *pSrcImage = cvLoadImage("resource\\testSquareRes\\2019-02-25_12-08-24_ROI\\img_439.jpg", CV_LOAD_IMAGE_UNCHANGED);

	g_pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImage, g_pGrayImage, CV_BGR2GRAY);

	g_pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);


	cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage(pstrWindowsSrcTitle, pSrcImage);

	cvNamedWindow(pstrWindowsBinaryTitle, CV_WINDOW_AUTOSIZE);


	int nThreshold = 0;
	cvCreateTrackbar(pstrWindowsToolBarName, pstrWindowsBinaryTitle, &nThreshold, 254, on_trackbar);

	on_trackbar(1);

	cvWaitKey(0);

	cvDestroyWindow(pstrWindowsSrcTitle);
	cvDestroyWindow(pstrWindowsBinaryTitle);
	cvReleaseImage(&pSrcImage);
	cvReleaseImage(&g_pGrayImage);
	cvReleaseImage(&g_pBinaryImage);

}
void detectEllipse() {
	string image_name = "resource\\test\\img_33.jpg";
	string save_name = "resource\\test\\img_33_myellipse_47.jpg";
	CvMemStorage* stor;
	CvSeq* cont;
	CvBox2D32f* box;
	CvPoint* PointArray;
	CvPoint2D32f* PointArray2D32f;
	IplImage *src = 0, *workImg01 = 0, *workImg02 = 0, *workImg03 = 0;
	if ((src = cvLoadImage(image_name.c_str(), 0)) == 0)
		return ;
	workImg01 = cvCloneImage(src);
	workImg02 = cvCloneImage(src);
	workImg03 = cvCloneImage(src);
	//Mat src = imread(image_name);
	//Mat workImg01 = src.clone();
	//Mat *workImg02 = src.clone();
	//Mat *workImg03 = src.clone();
	int slider_pos = 47;

	stor = cvCreateMemStorage(0);
	cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), stor);

	cvThreshold(workImg03, workImg02, slider_pos, 255, CV_THRESH_BINARY);
	//IplImage(workImg02)
	cvFindContours(workImg02, stor, &cont, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
	for (;cont;cont = cont->h_next) {
		int i;
		int count = cont->total;
		CvPoint center;
		CvSize size;
		if (count < 6)
			continue;
		PointArray = (CvPoint*)malloc(count * sizeof(CvPoint));
		PointArray2D32f = (CvPoint2D32f*)malloc(count * sizeof(CvPoint2D32f));
		// Alloc memory for ellipse data.
		box = (CvBox2D32f*)malloc(sizeof(CvBox2D32f));
		cvCvtSeqToArray(cont, PointArray, CV_WHOLE_SEQ);
		for (i = 0; i < count; i++)
		{
			PointArray2D32f[i].x = (float)PointArray[i].x;
			PointArray2D32f[i].y = (float)PointArray[i].y;
		}


		cvFitEllipse(PointArray2D32f, count, box);
		
		//cvDrawContours(&workImg02, cont, CV_RGB(255, 255, 255),CV_RGB(255, 255, 255), 0, 1, 8, cvPoint(0, 0));
		center.x = cvRound(box->center.x);
		center.y = cvRound(box->center.y);
		size.width = cvRound(box->size.width*0.5);
		size.height = cvRound(box->size.height*0.5);
		box->angle = -box->angle;

		if (size.width <= 20 || size.width > 100||size.height > 50)
			continue;
		cout << size.width << " " << size.height << endl;

		// Draw ellipse.
		cvEllipse(workImg01, center, size,
			box->angle, 0, 360,
			CV_RGB(100, 0, 255), 5, CV_AA, 0);
		free(PointArray);
		free(PointArray2D32f);
		free(box);
		cv::Mat save_img(workImg01, 0);

		imwrite(save_name, save_img);
	}
	cvReleaseImage(&src);
	cvReleaseImage(&workImg01);
	cvReleaseImage(&workImg02);
	cvReleaseImage(&workImg03);

}
