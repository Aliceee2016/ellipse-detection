#include "stdafx.h"
#include "common.h"
string time_day() {
	string res;
	time_t nowtime;
	nowtime = time(NULL);

	tm *ltm = localtime(&nowtime);
	int year = 1900 + ltm->tm_year;
	int mon = 1 + ltm->tm_mon;
	int day = ltm->tm_mday;


	res = to_string(year) + "-" + to_string(mon) + "-" + to_string(day);
	return res;
}
string time_now() {
	string res;
	time_t nowtime;
	nowtime = time(NULL);
	
	tm *ltm = localtime(&nowtime);
	int year = 1900 + ltm->tm_year;
	int mon = 1 + ltm->tm_mon;
	int day = ltm->tm_mday;
	int hour = ltm->tm_hour;
	int min = ltm->tm_min;
	int sec = ltm->tm_sec;

	res = to_string(year) + "-" + to_string(mon) + "-" + to_string(day) + "_" + to_string(hour) + "-" + to_string(min) + "-" + to_string(sec);
	return res;

	//cout << 1900 + ltm->tm_year << endl;
	//cout << 1 + ltm->tm_mon << endl;
	//cout << ltm->tm_mday << endl;
	//cout << ltm->tm_hour << ":";
	//cout << ltm->tm_min << ":";
	//cout << ltm->tm_sec << endl;
}



void processVideos(string in_folder, string out_folder) {
	fstream fout(in_folder + "video_info.txt", ios::out);
	if (!fout.is_open()) {
		cout << "File opening error!" << endl;
		return;
	}

	vector<string> names;
	glob(in_folder + "*.*", names);
	for (int j = 0; j < names.size(); j++) {
		string video_name = names[j];

		string name = video_name.substr(video_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		fout << name << endl;
		string resultFolder = out_folder + name + "\\";
		makeDirectoty(resultFolder);
		cout << "processing " << name << endl;
		videoToImages(5, 01, video_name, resultFolder);
	}
	fout.close();
}
void videoToImages(int period, int file_count, string videoFileName, string resultFileName) {

	CvCapture *capture = cvCreateFileCapture(videoFileName.c_str());
	if (capture == NULL) {
		cout << "Error initialize the capture! " << endl;
		return;
	}
	IplImage *frame = new IplImage();
	//Mat frame;
	string imgName;
	while (1) {


		for (int i = 0; i < period; i++) {
			frame = cvQueryFrame(capture);
			if (!frame) {
				cout << "finish!" << endl;
				return;
			}
		}
		cout << file_count << endl;

		//int x1 = 530;
		//int y1 = 186;
		//int x2 = 863;
		//int y2 = 687;
		//Rect rect(x1, y1, x2, y2);
		Mat imageFrame = cvarrToMat(frame, true);
		//Mat image_roi = imageFrame(rect);

		imgName = "img_" + to_string(file_count) + ".jpg";
		file_count++;
		//cout << imgName << endl;
		string fileName = resultFileName + imgName;

		imwrite(fileName, imageFrame);
		//cout << fileName << endl;
		//waitKey();
		//cvSaveImage(fileName.c_str(), frame);
	}
	delete frame;
	cvReleaseCapture(&capture);
	return;

}
void drawRectangle(string in_folder, string out_folder, int x, int y, int width, int height) {
	
	Rect select;
	select.x = x;
	select.y = y;
	select.width = width;
	select.height = height;
	vector<string> names;
	glob(in_folder + "*.*", names);
	string saving_path = out_folder;
	makeDirectoty(saving_path);
	
	string info_path = saving_path + "info\\";
	makeDirectoty(info_path);
	string infotxtPath = info_path + "info.txt";
	fstream fout(infotxtPath, ios::out);
	if (!fout.is_open()) {
		cout << "file open ERROR" << endl;
		return;
	}
	fout << "x " << x << endl
		<< "y " << y << endl
		<< "width " << width << endl
		<< "height " << height << endl;
	fout.close();

	///cout << names.size() << endl;
	for (int j = 0; j < names.size(); j++) {
		string image_name = names.at(j);
		cout << " processing " << image_name << endl;
		string name = image_name.substr(image_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		string save_name = saving_path + name + "_x_" + to_string(x) + "_y_" + to_string(y) + "_width_" + to_string(width) + "_height_" + to_string(height)+".jpg";
		//string save_name = saving_path + name + ".jpg";
		Mat src = imread(image_name);
		Mat workImg = src.clone();
		if (workImg.empty()) {
			cout << "fail to load Img" << endl;
			return;
		}

		//Mat dst;

		//Mat dstImage = workImg(select);
		rectangle(workImg, select, Scalar(0, 0, 255), 3, 8, 0);

		imwrite(save_name, workImg);


	}
}
void cutImage(string in_folder, string out_folder, int x, int y, int width, int height) {
	Rect select;
	select.x = x;
	select.y = y;
	select.width = width;
	select.height = height;
	vector<string> names;
	glob(in_folder + "*.*", names);
	string saving_path = out_folder;
	makeDirectoty(saving_path);
	for (int j = 0; j < names.size(); j++) {
		string image_name = names.at(j);
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

		//Mat dst;

		Mat dstImage = workImg(select);


		imwrite(save_name, dstImage);
	}
}
void applyClahe(string in_folder, string out_folder) {

	vector<string> names;
	glob(in_folder + "*.*", names);
	string saving_path = out_folder;
	makeDirectoty(saving_path);

	for (int j = 0; j < names.size(); j++) {
		string image_name = names.at(j);

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

		Mat clahe_img;
		cv::cvtColor(workImg, clahe_img, CV_BGR2Lab);
		vector<Mat> channels(3);
		cv::split(clahe_img, channels);

		Ptr<CLAHE> clahe = createCLAHE();
		//Ptr<HE> ahe = createHE();
		clahe->setClipLimit(4.);
		Mat dst;
		clahe->apply(channels[0], dst);
		dst.copyTo(channels[0]);

		clahe->apply(channels[1], dst);
		dst.copyTo(channels[1]);

		clahe->apply(channels[2], dst);
		dst.copyTo(channels[2]);

		cv::merge(channels, clahe_img);

		Mat image_clahe;
		cv::cvtColor(clahe_img, image_clahe, CV_Lab2BGR);
		//imshow("image clahe", image_clahe);
		cv::imwrite(save_name, image_clahe);


	}


}
void houghCircleTxt(string in_path, string out_path, double edgePercent, double param2, int maxRadius, int minRadius, int minDist) {
	Mat medianImg;
	Mat gaussImg;
	Mat grayImg;
	Mat houghCirclesImg;
	//string file_str[5] = { "2019-02-25_12-05-26_3","2019-02-25_12-08-24","2019-02-25_12-12-23","2019-02-25_12-17-49","2019-02-25_12-23-39" };
	string file_name = "info.txt";


	//string img_path = res_path + file_str[i] + "\\";
	string img_path = in_path;


	vector<string> names;

	glob(img_path + "*.*", names);
	sort(names.begin(), names.end(), cmp);
	//string saving_path = out_path + file_str[i]+"_param1" +to_string(int(param1))+"_param2" + to_string(int(param2))+ "\\" ;
	string saving_path = out_path;
	makeDirectoty(saving_path);

	ofstream fout(saving_path + file_name, ios::out);

	if (!fout.is_open()) {
		cerr << "Error: " << strerror(errno) << endl;
		return;
	}
	//return;
	for (int j = 0; j < names.size(); j++) {
		string image_name = names.at(j);
		cout << "processing " << image_name << endl;

		string name = image_name.substr(image_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		string save_name = saving_path + name + ".jpg";
		Mat src = imread(image_name);

		Mat workImg = src.clone();
		if (workImg.empty()) {
			cout << "fail to load Img" << endl;
			return;
		}

		cvtColor(workImg, grayImg, CV_BGR2GRAY);

		medianBlur(grayImg, medianImg, 3);
		//cv::GaussianBlur(grayImg, gaussImg, cv::Size(5, 5), 1.4);
		int *thresholds = new int[2];
		getThreshold(workImg, thresholds, edgePercent);
		double param1 = thresholds[0];
		vector<Vec3f> pcircles;
		HoughCircles(medianImg, pcircles, CV_HOUGH_GRADIENT, 1.5, minDist, param1, param2, minRadius, maxRadius);
		//if (pcircles.size() != 0) {
		int pos = name.find_last_of('_') + 1;
		string num_name = name.substr(pos, name.size() - pos + 1);
		fout << num_name << endl
			<< pcircles.size() << endl;
			//return;
			//fout << "method : CV_HOUGH_GRADIENT" << endl
			//	<< "dp = " << 1.5 << endl
			//	<< "minDist = " << minDist << endl
			//	<< "param1 = " << param1 << endl
			//	<< "param2 = " << param2 << endl
			//	<< "minRadius = " << minRadius << endl
			//	<< "maxRadius = " << maxRadius << endl
			//	<< "radius" << "  " << "pcircles[i][0]" << "  " << "pcircles[i][1]" << endl;
		//}
		for (size_t i = 0; i < pcircles.size(); i++) {
			int radius = cvRound(pcircles[i][2]);
			Point center(cvRound(pcircles[i][0]), cvRound(pcircles[i][1]));
			if (pcircles.size() != 0) {
				fout << pcircles[i][2] << endl
					<< pcircles[i][0] << endl
					<< pcircles[i][1] << endl;
				circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
				circle(src, center, radius, Scalar(0, 255, 0), 3, 8, 0);
			}
			//fout << endl << endl;
		}
		delete[]thresholds;
		imwrite(save_name, src);
	}
	fout.close();
}
void houghCircle(string in_path, string out_path, double edgePercent, double param2, int maxRadius, int minRadius, int minDist) {
	//int minRadius = 0;
	//double minDist = 40;
	//int maxRadius = 55;
	//double param2 = 100;
	//Mat src;
	Mat medianImg;
	Mat gaussImg;
	Mat grayImg;
	Mat houghCirclesImg;
	//string file_str[5] = { "2019-02-25_12-05-26_3","2019-02-25_12-08-24","2019-02-25_12-12-23","2019-02-25_12-17-49","2019-02-25_12-23-39" };
	string file_name = "hough_circles_info.txt";
	//string img_path = res_path + file_str[i] + "\\";
	string img_path = in_path;
	vector<string> names;
	glob(img_path + "*.*", names);
	sort(names.begin(), names.end(), cmp);
	//string saving_path = out_path + file_str[i]+"_param1" +to_string(int(param1))+"_param2" + to_string(int(param2))+ "\\" ;
	string saving_path = out_path;
	makeDirectoty(saving_path);
	ofstream fout(saving_path + file_name, ios::out);
	if (!fout.is_open()) {
		cerr << "Error: " << strerror(errno) << endl;
		return;
	}
	for (int j = 0; j < names.size(); j++) {
		string image_name = names[j];
		cout << "processing " << image_name << endl;
		//cout << "1" << endl;
		string name = image_name.substr(image_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		string save_name = saving_path + name + ".jpg";
		Mat src = imread(image_name);

		Mat workImg = src.clone();
		if (workImg.empty()) {
			cout << "fail to load Img" << endl;
			return;
		}
		cvtColor(workImg, grayImg, CV_BGR2GRAY);
		medianBlur(grayImg, medianImg, 3);
		//cv::GaussianBlur(grayImg, gaussImg, cv::Size(5, 5), 1.4);
		int *thresholds = new int[2];
		getThreshold(workImg, thresholds, edgePercent);
		double param1 = thresholds[0];
		//cout << thresholds[0] <<" "<< thresholds[1] << endl;
		vector<Vec3f> pcircles;
		//int method, double dp, double minDist,
		//	double param1 = 100, double param2 = 100,d
		//	int minRadius = 0, int maxRadius = 0 
		HoughCircles(medianImg, pcircles, CV_HOUGH_GRADIENT, 1.5, minDist, param1, param2, minRadius, maxRadius);

		if (pcircles.size() != 0) {
			fout << name << endl;
			fout << "method : CV_HOUGH_GRADIENT" << endl
				<< "dp = " << 1.5 << endl
				<< "minDist = " << minDist << endl
				<< "param1 = " << param1 << endl
				<< "param2 = " << param2 << endl
				<< "minRadius = " << minRadius << endl
				<< "maxRadius = " << maxRadius << endl
				<< "radius" << "  " << "pcircles[i][0]" << "  " << "pcircles[i][1]" << endl;

		}

		for (size_t i = 0; i < pcircles.size(); i++) {
			int radius = cvRound(pcircles[i][2]);

			//fout << pcircles[i][2] << endl;
			//if (radius > 45 || radius < 28)
			//	continue;
			Point center(cvRound(pcircles[i][0]), cvRound(pcircles[i][1]));
			//if (pcircles.size() != 0 && radius <= 40 ) {
			if (pcircles.size() != 0) {
				fout << i + 1 << "  " << pcircles[i][2] << "  " << pcircles[i][0] << "  " << pcircles[i][1] << endl;
				circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
				circle(src, center, radius, Scalar(0, 255, 0), 3, 8, 0);

			}
			fout << endl << endl;

		}

		//imshow("result", src);
		delete[]thresholds;
		//cout << "save name : " << save_name << endl;
		imwrite(save_name, src);

		//if (count == 0)
		//	break;
	}
	//if (count == 0)
	//	return;
	fout.close();

}


void detectEllipsedir(string in_path,string out_path, int slider_pos) {
	string file_name = "info.txt";
	string img_path = in_path;


	vector<string> names;

	glob(img_path + "*.*", names);
	sort(names.begin(), names.end(), cmp);
	//string saving_path = out_path + file_str[i]+"_param1" +to_string(int(param1))+"_param2" + to_string(int(param2))+ "\\" ;
	string saving_path = out_path;
	makeDirectoty(saving_path);

	ofstream fout(saving_path + file_name, ios::out);

	if (!fout.is_open()) {
		cerr << "Error: " << strerror(errno) << endl;
		return;
	}
	//return;
	for (int j = 0; j < names.size(); j++) {
		string image_name = names.at(j);
		cout << "processing " << image_name << endl;

		string name = image_name.substr(image_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		string save_name = saving_path + name + ".jpg";
		//Mat src = imread(image_name);




		//cv::GaussianBlur(grayImg, gaussImg, cv::Size(5, 5), 1.4);
		
		
		//if (pcircles.size() != 0) {
		int pos = name.find_last_of('_') + 1;
		string num_name = name.substr(pos, name.size() - pos + 1);

		
		//string image_name = "resource\\test\\img_33.jpg";
		//string save_name = "resource\\test\\img_33_myellipse_47.jpg";
		CvMemStorage* stor;
		CvSeq* cont;
		CvBox2D32f* box;
		CvPoint* PointArray;
		CvPoint2D32f* PointArray2D32f;
		IplImage *src = 0, *workImg01 = 0, *workImg02 = 0, *workImg03 = 0;
		if ((src = cvLoadImage(image_name.c_str(), 0)) == 0)
			return;
		workImg01 = cvCloneImage(src);
		workImg02 = cvCloneImage(src);
		workImg03 = cvCloneImage(src);
		//Mat src = imread(image_name);
		//Mat workImg01 = src.clone();
		//Mat *workImg02 = src.clone();
		//Mat *workImg03 = src.clone();


		
		int *thresholds = new int[2];
		double edgePercent = 0.3;
		getThreshold(src, thresholds, edgePercent);
		double param1 = thresholds[0];
		//int slider_pos = param1;
		//int slider_pos = 57;
		delete[]thresholds;

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

		/*	if (size.width <= 20 || size.width > 100 || size.height > 50)
				continue;*/
			if (size.width <= 10 || size.width > 100 || size.height <= 10 || size.height > 100)
				continue;
			cout << size.width << " " << size.height <<" "<< box->angle << endl;

			// Draw ellipse.
			cvEllipse(workImg01, center, size,
				box->angle, 0, 360,
				CV_RGB(100, 0, 255), 5, CV_AA, 0);
			free(PointArray);
			free(PointArray2D32f);
			free(box);
			cv::Mat save_img(workImg01, 0);

			cv::imwrite(save_name, save_img);
		}
		cvReleaseImage(&src);
		cvReleaseImage(&workImg01);
		cvReleaseImage(&workImg02);
		cvReleaseImage(&workImg03);
		
		
	}
	fout.close();

}


void makeDirectoty(string dirName) {
	if (_access(dirName.c_str(), 0) == -1)
	{
		bool flag = CreateDirectory(dirName.c_str(), NULL);
		if (flag) {
			cout << "Creat " << dirName << " successful!" << endl;
		}
		else {
			cout << "FAIL to creat " << dirName << endl;
		}
	}
	else {
		cout << dirName << " already exists." << endl;
	}

}
void getThreshold(Mat src, int *thresholds, double edgePercent, int aperture_size) {
	CvSize size;
	int flags = aperture_size;
	int lowerThreshold;
	int upperThreshold;
	int* mag_buf[3];
	uchar* map;
	ptrdiff_t mapstep;
	int maxsize;
	//int i, j;
	CvMat mag_row;

	size.width = src.cols;
	size.height = src.rows;

	//Mat1f magGrad(size.height, size.width, 0.f);

	float maxGrad(0);
	float val(0);




	Mat workImg = Mat(src);
	workImg = src.clone();
	//Noise reduction
	cv::GaussianBlur(src, workImg, cv::Size(5, 5), 1.4);

	//calculating gradient magnitudes and directions
	Mat magX = Mat(src.rows, src.cols, CV_32F);
	Mat magY = Mat(src.rows, src.cols, CV_32F);
	Mat magGrad = Mat(src.rows, src.cols, CV_32F);

	cv::Sobel(workImg, magX, CV_32F, 1, 0, aperture_size);
	cv::Sobel(workImg, magY, CV_32F, 0, 1, aperture_size);

	cv::MatIterator_<float>itX = magX.begin<float>();
	cv::MatIterator_<float>itXend = magX.end<float>();
	cv::MatIterator_<float>it_magGrad = magGrad.begin<float>();

	cv::MatIterator_<float>itY = magY.begin<float>();
	cv::MatIterator_<float>itYend = magY.end<float>();

	for (;itX != itXend; ++itX, ++it_magGrad, ++itY) {
		const cv::Point pos = itX.pos();

		val = float(abs(*itX) + abs(*itY));
		*it_magGrad = val;
		maxGrad = (val > maxGrad) ? val : maxGrad;

	}
	//% Normalize for threshold selection
	//normalize(magGrad, magGrad, 0.0, 1.0, NORM_MINMAX);

	//% Determine Hysteresis Thresholds

	//set magic numbers
	const int NUM_BINS = 64;
	const double percent_of_pixels_not_edges = 1.0 - edgePercent;
	const double threshold_ratio = 0.3;

	//compute histogram
	int bin_size = cvFloor(maxGrad / float(NUM_BINS) + 0.5f) + 1;
	if (bin_size < 1) bin_size = 1;
	int bins[NUM_BINS] = { 0 };

	it_magGrad = magGrad.begin<float>();
	cv::MatIterator_<float>it_magGrad_end = magGrad.end<float>();

	for (;it_magGrad != it_magGrad_end; ++it_magGrad) {

		int hgf = int(*it_magGrad);
		bins[hgf / bin_size]++;
	}


	//for (int i = 0; i < size.height; ++i) {
	//	float* _pmag = magGrad.ptr<float>(i);
	//	for (int j = 0; j < size.width; ++j) {
	//		int hgf = int(_pmag[j]);
	//		bins[int(_pmag[j]) / bin_size]++;
	//	}
	//}

	//select the thresholds
	float total(0.f);
	float target = float(size.height * size.width * percent_of_pixels_not_edges);
	int low_thresh, high_thresh(0);
	while (total < target) {
		total += bins[high_thresh];
		high_thresh++;
	}
	high_thresh *= bin_size;
	low_thresh = cvFloor(threshold_ratio * float(high_thresh));

	if (flags & CV_CANNY_L2_GRADIENT) {
		Cv32suf ul, uh;
		ul.f = (float)low_thresh;
		uh.f = (float)high_thresh;

		lowerThreshold = ul.i;
		upperThreshold = uh.i;
	}
	else {
		lowerThreshold = cvFloor(low_thresh);
		upperThreshold = cvFloor(high_thresh);
	}
	thresholds[0] = upperThreshold;
	thresholds[1] = lowerThreshold;
}
Mat MyCanny3(Mat src, int *thresholds, double edgePercent, double aperture_size) {
	CvSize size;
	int flags = aperture_size;
	int lowerThreshold;
	int upperThreshold;
	int* mag_buf[3];
	uchar* map;
	ptrdiff_t mapstep;
	int maxsize;
	//int i, j;
	CvMat mag_row;

	size.width = src.cols;
	size.height = src.rows;

	//Mat1f magGrad(size.height, size.width, 0.f);

	float maxGrad(0);
	float val(0);




	Mat workImg = Mat(src);
	workImg = src.clone();
	//Noise reduction
	cv::GaussianBlur(src, workImg, cv::Size(5, 5), 1.4);

	//calculating gradient magnitudes and directions
	Mat magX = Mat(src.rows, src.cols, CV_32F);
	Mat magY = Mat(src.rows, src.cols, CV_32F);
	Mat magGrad = Mat(src.rows, src.cols, CV_32F);

	cv::Sobel(workImg, magX, CV_32F, 1, 0, aperture_size);
	cv::Sobel(workImg, magY, CV_32F, 0, 1, aperture_size);

	cv::MatIterator_<float>itX = magX.begin<float>();
	cv::MatIterator_<float>itXend = magX.end<float>();
	cv::MatIterator_<float>it_magGrad = magGrad.begin<float>();

	cv::MatIterator_<float>itY = magY.begin<float>();
	cv::MatIterator_<float>itYend = magY.end<float>();

	for (;itX != itXend; ++itX, ++it_magGrad, ++itY) {
		const cv::Point pos = itX.pos();

		val = float(abs(*itX) + abs(*itY));
		*it_magGrad = val;
		maxGrad = (val > maxGrad) ? val : maxGrad;

	}

	//for (int i = 0; i < size.height; ++i) {
	//	float* _pmag = magGrad.ptr<float>(i);
	//	for (int j = 0; j < size.width; ++j) {
	//		val = float(abs(magX.at<float>(j, i) + abs(magY.at <float>(j, i))));
	//		_pmag[j] = val;
	//		maxGrad = (val > maxGrad) ? val : maxGrad;
	//	}
	//}

	//% Normalize for threshold selection
	//normalize(magGrad, magGrad, 0.0, 1.0, NORM_MINMAX);

	//% Determine Hysteresis Thresholds

	//set magic numbers
	const int NUM_BINS = 64;
	const double percent_of_pixels_not_edges = 1 - edgePercent;
	const double threshold_ratio = 0.3;

	//compute histogram
	int bin_size = cvFloor(maxGrad / float(NUM_BINS) + 0.5f) + 1;
	if (bin_size < 1) bin_size = 1;
	int bins[NUM_BINS] = { 0 };

	it_magGrad = magGrad.begin<float>();
	cv::MatIterator_<float>it_magGrad_end = magGrad.end<float>();

	for (;it_magGrad != it_magGrad_end; ++it_magGrad) {

		int hgf = int(*it_magGrad);
		bins[hgf / bin_size]++;
	}


	//for (int i = 0; i < size.height; ++i) {
	//	float* _pmag = magGrad.ptr<float>(i);
	//	for (int j = 0; j < size.width; ++j) {
	//		int hgf = int(_pmag[j]);
	//		bins[int(_pmag[j]) / bin_size]++;
	//	}
	//}

	//select the thresholds
	float total(0.f);
	float target = float(size.height * size.width * percent_of_pixels_not_edges);
	int low_thresh, high_thresh(0);
	while (total < target) {
		total += bins[high_thresh];
		high_thresh++;
	}
	high_thresh *= bin_size;
	low_thresh = cvFloor(threshold_ratio * float(high_thresh));

	if (flags & CV_CANNY_L2_GRADIENT) {
		Cv32suf ul, uh;
		ul.f = (float)low_thresh;
		uh.f = (float)high_thresh;

		lowerThreshold = ul.i;
		upperThreshold = uh.i;
	}
	else {
		lowerThreshold = cvFloor(low_thresh);
		upperThreshold = cvFloor(high_thresh);
	}
	thresholds[0] = upperThreshold;
	thresholds[1] = lowerThreshold;



	Mat direction = Mat(workImg.rows, workImg.cols, CV_32F);
	cv::divide(magY, magX, direction);

	Mat sum = Mat(workImg.rows, workImg.cols, CV_64F);
	Mat prodX = Mat(workImg.rows, workImg.cols, CV_64F);
	Mat prodY = Mat(workImg.rows, workImg.cols, CV_64F);
	cv::multiply(magX, magX, prodX);
	cv::multiply(magY, magY, prodY);
	sum = prodX + prodY;
	cv::sqrt(sum, sum);

	Mat returnImg = Mat(src.rows, src.cols, CV_8U);
	returnImg.setTo(cv::Scalar(0)); //Initialie image to return to zero

									//Initialize iterators
									//returnImg is the image that will have all canny edges. 
									//We've initialized all iterators for the magnitude, direction and return images.
	cv::MatIterator_<float>itMag = sum.begin<float>();
	cv::MatIterator_<float>itDirection = direction.begin<float>();

	cv::MatIterator_<unsigned char>itRet = returnImg.begin<unsigned char>();

	cv::MatIterator_<float>itend = sum.end<float>();

	for (;itMag != itend; ++itDirection, ++itRet, ++itMag) {
		const cv::Point pos = itRet.pos();
		float currentDirection = atan(*itDirection) * 180 / 3.142;
		while (currentDirection < 0) {
			currentDirection += 180;
		}

		*itDirection = currentDirection;

		if (*itMag < upperThreshold) {
			continue;
		}

		bool flag = true;

		if (currentDirection > 112.5 && currentDirection <= 157.5) {
			if (pos.y > 0 && pos.x < workImg.cols - 1 && *itMag <= sum.at<float>(pos.y - 1, pos.x + 1)) {
				flag = false;
			}
			if (pos.y < workImg.rows - 1 && pos.x>0 && *itMag <= sum.at<float>(pos.y + 1, pos.x - 1)) {
				flag = false;
			}
		}
		else if (currentDirection > 67.5 && currentDirection <= 112.5) {
			if (pos.y > 0 && *itMag <= sum.at<float>(pos.y - 1, pos.x)) {
				flag = false;
			}
			if (pos.y < workImg.rows - 1 && *itMag <= sum.at<float>(pos.y + 1, pos.x)) {
				flag = false;
			}
		}
		else if (currentDirection > 22.5 && currentDirection <= 67.5) {
			if (pos.y > 0 && pos.x > 0 && *itMag <= sum.at<float>(pos.y - 1, pos.x - 1)) {
				flag = false;
			}
			if (pos.y < workImg.rows - 1 && pos.x < workImg.cols - 1 && *itMag <= sum.at<float>(pos.y + 1, pos.x + 1)) {
				flag = false;
			}
		}
		else {
			if (pos.x > 0 && *itMag <= sum.at<float>(pos.y, pos.x - 1)) {
				flag = false;
			}
			if (pos.x < workImg.cols - 1 && *itMag <= sum.at<float>(pos.y, pos.x + 1)) {
				flag = false;
			}
		}
		if (flag) {
			*itRet = 255;
		}
	}
	bool imageChanged = true;
	int i = 0;
	while (imageChanged)
	{
		imageChanged = false;
		cout << "Interation " << i << endl;
		i++;
		itMag = sum.begin<float>();
		itDirection = direction.begin<float>();
		itRet = returnImg.begin<unsigned char>();
		itend = sum.end<float>();
		for (;itMag != itend;++itMag, ++itDirection, ++itRet) {
			cv::Point pos = itRet.pos();
			if (pos.x < 2 || pos.x > src.cols - 2 || pos.y > src.rows - 2) {
				continue;
			}
			float currentDirection = *itDirection;
			if (*itRet == 255) {
				*itRet = (unsigned char)64;
				if (currentDirection > 112.5 && currentDirection <= 157.5) {
					if (pos.y > 0 && pos.x > 0) {
						if (lowerThreshold <= sum.at<float>(pos.y - 1, pos.x - 1) &&
							returnImg.at<unsigned char>(pos.y - 1, pos.x - 1) != 64 &&
							direction.at<float>(pos.y - 1, pos.x - 1) > 112.5 &&
							direction.at<float>(pos.y - 1, pos.x - 1) <= 157.5 &&
							sum.at<float>(pos.y - 1, pos.x - 1) > sum.at<float>(pos.y - 2, pos.x) &&
							sum.at<float>(pos.y - 1, pos.x - 1) > sum.at<float>(pos.y, pos.x - 2)) {
							returnImg.ptr<unsigned char>(pos.y - 1, pos.x - 1)[0] = 255;
							imageChanged = true;
						}
					}
					if (pos.y < workImg.rows - 1 && pos.x < workImg.cols - 1) {
						if (lowerThreshold <= sum.at<float>(cv::Point(pos.x + 1, pos.y + 1)) &&
							returnImg.at<unsigned char>(pos.y + 1, pos.x + 1) != 64 &&
							direction.at<float>(pos.y + 1, pos.x + 1) > 112.5 &&
							direction.at<float>(pos.y + 1, pos.x + 1) <= 157.5 &&
							sum.at<float>(pos.y - 1, pos.x - 1) > sum.at<float>(pos.y + 2, pos.x) &&
							sum.at<float>(pos.y - 1, pos.x - 1) > sum.at<float>(pos.y, pos.x + 2)) {
							returnImg.ptr<unsigned char>(pos.y + 1, pos.x + 1)[0] = 255;
							imageChanged = true;
						}
					}
				}
				else if (currentDirection > 67.5 && currentDirection <= 112.5) {
					if (pos.x > 0) {
						if (lowerThreshold <= sum.at<float>(cv::Point(pos.x - 1, pos.y)) &&
							returnImg.at<unsigned char>(pos.y, pos.x - 1) != 64 &&
							direction.at<float>(pos.y, pos.x - 1) > 67.5 &&
							direction.at<float>(pos.y, pos.x - 1) <= 112.5 &&
							sum.at<float>(pos.y, pos.x - 1) > sum.at<float>(pos.y - 1, pos.x - 1) &&
							sum.at<float>(pos.y, pos.x - 1) > sum.at<float>(pos.y + 1, pos.x - 1)) {
							returnImg.ptr<unsigned char>(pos.y, pos.x - 1)[0] = 255;
							imageChanged = true;
						}
					}
					if (pos.x < workImg.cols - 1) {
						if (lowerThreshold <= sum.at<float>(cv::Point(pos.x + 1, pos.y)) &&
							returnImg.at<unsigned char>(pos.y, pos.x + 1) != 64 &&
							direction.at<float>(pos.y, pos.x + 1) > 67.5 &&
							direction.at<float>(pos.y, pos.x + 1) <= 112.5 &&
							sum.at<float>(pos.y, pos.x + 1) > sum.at<float>(pos.y - 1, pos.x + 1) &&
							sum.at<float>(pos.y, pos.x + 1) > sum.at<float>(pos.y + 1, pos.x + 1)) {
							returnImg.ptr<unsigned char>(pos.y, pos.x + 1)[0] = 255;
							imageChanged = true;
						}
					}
				}

				else if (currentDirection > 22.5 && currentDirection <= 67.5) {
					if (pos.y > 0 && pos.x < workImg.cols - 1)
					{
						if (lowerThreshold <= sum.at<float>(cv::Point(pos.x + 1, pos.y - 1)) &&
							returnImg.at<unsigned char>(pos.y - 1, pos.x + 1) != 64 &&
							direction.at<float>(pos.y - 1, pos.x + 1) > 22.5 &&
							direction.at<float>(pos.y - 1, pos.x + 1) <= 67.5 &&
							sum.at<float>(pos.y - 1, pos.x + 1) > sum.at<float>(pos.y - 2, pos.x) &&
							sum.at<float>(pos.y - 1, pos.x + 1) > sum.at<float>(pos.y, pos.x + 2))
						{
							returnImg.ptr<unsigned char>(pos.y - 1, pos.x + 1)[0] = 255;
							imageChanged = true;
						}
					}
					if (pos.y < workImg.rows - 1 && pos.x>0)
					{
						if (lowerThreshold <= sum.at<float>(cv::Point(pos.x - 1, pos.y + 1)) &&
							returnImg.at<unsigned char>(pos.y + 1, pos.x - 1) != 64 &&
							direction.at<float>(pos.y + 1, pos.x - 1) > 22.5 &&
							direction.at<float>(pos.y + 1, pos.x - 1) <= 67.5 &&
							sum.at<float>(pos.y + 1, pos.x - 1) > sum.at<float>(pos.y, pos.x - 2) &&
							sum.at<float>(pos.y + 1, pos.x - 1) > sum.at<float>(pos.y + 2, pos.x))
						{
							returnImg.ptr<unsigned char>(pos.y + 1, pos.x - 1)[0] = 255;
							imageChanged = true;
						}
					}
				}

				else
				{
					if (pos.y > 0)
					{
						if (lowerThreshold <= sum.at<float>(cv::Point(pos.x, pos.y - 1)) &&
							returnImg.at<unsigned char>(pos.y - 1, pos.x) != 64 &&
							(direction.at<float>(pos.y - 1, pos.x) < 22.5 ||
								direction.at<float>(pos.y - 1, pos.x) >= 157.5) &&
							sum.at<float>(pos.y - 1, pos.x) > sum.at<float>(pos.y - 1, pos.x - 1) &&
							sum.at<float>(pos.y - 1, pos.x) > sum.at<float>(pos.y - 1, pos.x + 2))
						{
							returnImg.ptr<unsigned char>(pos.y - 1, pos.x)[0] = 255;
							imageChanged = true;
						}
					}
					if (pos.y < workImg.rows - 1)
					{
						if (lowerThreshold <= sum.at<float>(cv::Point(pos.x, pos.y + 1)) &&
							returnImg.at<unsigned char>(pos.y + 1, pos.x) != 64 &&
							(direction.at<float>(pos.y + 1, pos.x) < 22.5 ||
								direction.at<float>(pos.y + 1, pos.x) >= 157.5) &&
							sum.at<float>(pos.y + 1, pos.x) > sum.at<float>(pos.y + 1, pos.x - 1) &&
							sum.at<float>(pos.y + 1, pos.x) > sum.at<float>(pos.y + 1, pos.x + 1))
						{
							returnImg.ptr<unsigned char>(pos.y + 1, pos.x)[0] = 255;
							imageChanged = true;
						}
					}
				}

			}
		}
	}
	cv::MatIterator_<unsigned char>current = returnImg.begin<unsigned char>();
	cv::MatIterator_<unsigned char>final = returnImg.begin<unsigned char>();

	//cv::MatIterator_<unsigned char>currentDes = des.begin<unsigned char>();
	for (;current != final; ++current) {
		if (*current == 64) {
			*current = 255;
		}
		//currentDes = current;
	}
	//imshow("des.pic", returnImg);
	return returnImg;
}
void houghCircleOSTU(string in_path, string out_path, double edgePercent, double param2, int maxRadius, int minRadius, int minDist) {
	//int minRadius = 0;
	//double minDist = 40;
	//int maxRadius = 55;

	//double param2 = 100;
	//Mat src;
	Mat medianImg;
	Mat gaussImg;
	Mat grayImg;
	Mat houghCirclesImg;
	//string file_str[5] = { "2019-02-25_12-05-26_3","2019-02-25_12-08-24","2019-02-25_12-12-23","2019-02-25_12-17-49","2019-02-25_12-23-39" };
	string file_name = "hough_circles_info.txt";


	//string img_path = res_path + file_str[i] + "\\";
	string img_path = in_path;


	vector<string> names;

	glob(img_path + "*.*", names);
	//string saving_path = out_path + file_str[i]+"_param1" +to_string(int(param1))+"_param2" + to_string(int(param2))+ "\\" ;
	string saving_path = out_path;
	makeDirectoty(saving_path);
	ofstream fout(saving_path + file_name, ios::out);

	if (!fout.is_open()) {
		cerr << "Error: " << strerror(errno) << endl;
		return;
	}

	for (int j = 0; j < names.size(); j++) {
		string image_name = names.at(j);
		cout << "processing " << image_name << endl;


		//cout << "1" << endl;
		string name = image_name.substr(image_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		string save_name = saving_path + name + ".jpg";
		string save_name_gray = saving_path + "gray\\" + name + ".jpg";


		Mat src = imread(image_name);

		Mat workImg = src.clone();
		if (workImg.empty()) {
			cout << "fail to load Img" << endl;
			return;
		}

		cvtColor(workImg, grayImg, CV_BGR2GRAY);
		imwrite(save_name_gray, grayImg);
		//imshow("gray", grayImg);
		//medianBlur(grayImg, medianImg, 3);
		cv::GaussianBlur(grayImg, gaussImg, cv::Size(5, 5), 1.4);



		int *thresholds = new int[2];
		getThreshold(workImg, thresholds, edgePercent);
		double param1 = thresholds[0];
		//cout << thresholds[0] <<" "<< thresholds[1] << endl;

		vector<Vec3f> pcircles;

		//int method, double dp, double minDist,
		//	double param1 = 100, double param2 = 100,d
		//	int minRadius = 0, int maxRadius = 0 


		HoughCircles(gaussImg, pcircles, CV_HOUGH_GRADIENT, 1.5, minDist, param1, param2, minRadius, maxRadius);

		if (pcircles.size() != 0) {
			fout << name << endl;
			fout << "method : CV_HOUGH_GRADIENT" << endl
				<< "dp = " << 1.5 << endl
				<< "minDist = " << minDist << endl
				<< "param1 = " << param1 << endl
				<< "param2 = " << param2 << endl
				<< "minRadius = " << minRadius << endl
				<< "maxRadius = " << maxRadius << endl
				<< "radius" << "  " << "pcircles[i][0]" << "  " << "pcircles[i][1]" << endl;

		}

		for (size_t i = 0; i < pcircles.size(); i++) {
			int radius = cvRound(pcircles[i][2]);

			//fout << pcircles[i][2] << endl;
			//if (radius > 45 || radius < 28)
			//	continue;
			Point center(cvRound(pcircles[i][0]), cvRound(pcircles[i][1]));
			//if (pcircles.size() != 0 && radius <= 40 ) {
			if (pcircles.size() != 0) {
				fout << i + 1 << "  " << pcircles[i][2] << "  " << pcircles[i][0] << "  " << pcircles[i][1] << endl;
				circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
				circle(src, center, radius, Scalar(0, 255, 0), 3, 8, 0);

			}

		}

		//imshow("result", src);
		delete[]thresholds;
		//cout << "save name : " << save_name << endl;

		imwrite(save_name, src);
		fout << endl << endl;
		//if (count == 0)
		//	break;
	}
	//if (count == 0)
	//	return;
	fout.close();
}
void applyPyramid(string in_folder, string out_folder) {
	vector<string> names;
	glob(in_folder + "*.*", names);
	string saving_path = out_folder;
	makeDirectoty(saving_path);
	for (int j = 0; j < names.size(); j++) {
		string image_name = names.at(j);
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
		Mat dstImage;
		
		pyrDown(workImg, dstImage, Size(workImg.cols / 2, workImg.rows / 2));
		//imshow("src", img);
		//imshow("gray", gray);
		//imshow("dst", dst);
		imwrite(save_name, dstImage);

	}
}
void addOffset(string in_folder, string out_folder, double x, double y) {
	fstream fin(in_folder, ios::in);
	if (!fin.is_open()) {
		cout << "file opening error" << endl;
		return;
	}
	fstream fout(out_folder, ios::out);
	if (!fout.is_open()) {
		cout << "file opening error" << endl;
		return;
	}
	while (!fin.eof()) {
		string str[5];
		for (int i = 0; i < 5; i++) {
			fin >> str[i];
		}
		double str3 = stof(str[3]) + x;
		str[3] = to_string(str3);
		for (int i = 0; i < 5; i++) {
			fout << str[i] << endl;
		}
	}
	fin.close();
	fout.close();
}
bool cmp(string a, string b) {
	int pos1 = a.find_last_of("_") + 1;
	int pos2 = a.find_last_of(".");
	int lenght = pos2 - pos1;
	a = a.substr(pos1, lenght);
	int va = stoi(a);
	
	int posb1 = b.find_last_of("_") + 1;
	int posb2 = b.find_last_of(".");
	int lenght2 = posb2 - posb1;
	b = b.substr(posb1, lenght2);
	int vb = stoi(b);
	
	//cout << va <<" "<< vb << endl;
	return va < vb;
}
void drawPoint() {

	Mat img = imread("resource\\test\\pic\\img_1_points.jpg");
	vector<Vec4f> points;
	string filename = "resource\\test\\zvsets_3_all.txt";
	//string filename = "resource\\test\\24points.txt";
	fstream fin(filename, ios::in);
	if (!fin.is_open()) {
		cout << "file open error!" << endl;
		return;
	}


	while (!fin.eof()) {
		float num, x, y, z;
		fin >> num >> x >> y >> z;
		//x = x + x_offset;
		//y = y + y_offset;
		Vec4f point(num,x,y,z);
		points.push_back(point);
	}
	fin.close();
	//img.setTo(255);  
	Point p2(0,0);
	//464 186 115 76 -71.9693
	float x_offset = 464;
	float y_offset = 186;
	Point center(464, 186);
	CvSize size;
	size.width = 115;
	size.height = 76;
	float angle =  -71.9693;
	ellipse(img, Point(0,0), size,
		0, 0, 360,
		Scalar(0, 0, 255), 5, CV_AA, 0);
	circle(img, p2, 10, Scalar(255, 0, 0), -1, 8, 0);
	//circle(img, center, size.width, Scalar(255, 0, 0), 3, 8, 0);
	//circle(img, center, size.height, Scalar(255, 100, 80), 3, 8, 0);
	//				circle(src, center, radius, Scalar(0, 255, 0), 3, 8, 0);
	for (int i = 0; i < points.size();i++) {
		//Point p1(points[i][1]+x_offset, points[i][2] + y_offset);
		Point p1(points[i][1], points[i][2] );
		cout<<i<<"  " << points[i][1] << " " << points[i][2] << endl;
		circle(img, p1, 3, Scalar(0, 0, 255), -1, 8, 0);
	}
	//Point p1(100, 100);      
	//circle(img, p1, 3, Scalar(0, 255, 0), -1, 8, 0);

	imshow("drawBroad", img);
	string save_name = "resource\\test\\pic\\img_ellipse_24.jpg";
	imwrite(save_name, img);
	waitKey(0);

}