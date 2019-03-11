#include "stdafx.h"
#include "common.h"

void videoToImages(int period, int file_count, string videoFileName, string resultFileName) {

	CvCapture *capture = cvCreateFileCapture(videoFileName.c_str());
	if (capture == NULL) {
		cout << "Error initialize the capture! " << endl;
		return ;
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

		int x1 = 530;
		int y1 = 186;
		int x2 = 863;
		int y2 = 687;
		Rect rect(x1, y1, x2, y2);
		Mat imageFrame = cvarrToMat(frame, true);
		Mat image_roi = imageFrame(rect);
		
		imgName = "img_" + to_string(file_count)+".jpg";
		file_count++;
		//cout << imgName << endl;
		string fileName = resultFileName + imgName;
		
		imwrite(fileName, image_roi);
		//cout << fileName << endl;
		//waitKey();
		//cvSaveImage(fileName.c_str(), frame);
	}
	delete frame;
	cvReleaseCapture(&capture);
	return;

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

Mat MyCanny3(Mat src, int *thresholds,double edgePercent, double aperture_size) {
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
					if (pos.y>0 && pos.x<workImg.cols - 1)
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
					if (pos.y<workImg.rows - 1 && pos.x>0)
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
					if (pos.y>0)
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
					if (pos.y<workImg.rows - 1)
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

void processVideos() {
	string resourceFolder = "resource\\";
	string imageFolder = resourceFolder + "source_img\\";
	string videoFolder = resourceFolder + "video\\";
	//string imageFromVideo = resourceFolder + "imgFromVideo\\";
	string imageFromVideo = resourceFolder + "imgFromVideoROI\\";

	vector<string> names;

	glob(videoFolder + "*.*", names);

	for (const auto & video_name : names) {
		//cout << video_name << endl;
		//string name = video_name.substr(0, name.find_last_of("."));
		string name = video_name.substr(video_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		//cout << name << endl << endl;
		string resultFolder = imageFromVideo + name + "\\";
		makeDirectoty(resultFolder);

		videoToImages(5, 01, video_name, resultFolder);

	}
	//makeDirectoty(dirName);
	cout << "Finished." << endl;
}


void edgeForImage(double edgePercent) {
	string resourceFolder = "resource\\";
	//string imageFolder = resourceFolder + "source_img\\2019-02-25_12-05-26";
	//2019-02-25_12-08-24
	string imageFolder = resourceFolder + "source_img\\2019-02-25_12-08-24";
	string videoFolder = resourceFolder + "video\\";
	string imageFromVideo = resourceFolder + "imgFromVideo\\";
	//string outFolder = resourceFolder + "imgWithEdge\\2019-02-25_12-05-26\\";
	string outFolder = resourceFolder + "imgWithEdge\\2019-02-25_12-08-24\\";

	outFolder = outFolder + "edgePercent" + to_string(int(edgePercent*100)) +  "%" + "\\";
	cout << outFolder << endl;
	makeDirectoty(outFolder);
	//vector<string> folder_names;
	//cout << imageFolder << endl;
	//
	////glob(imageFolder, folder_names);
	////cout << folder_names[0] << endl;
	//int i = 0;
	//for (const auto & folder : folder_names) {
	//	cout << folder << endl;
	//	cout << " 1 " << endl;
	//	Mat src = cv::imread(folder);
	//	i++;
	//	imshow("test", src);
	//}
	//cout << i << endl;
	vector<string> names;

	glob(imageFolder + "\\" + "*.*", names);
	std::cout << "start" << endl;
	int *thresholds = new int[2];
	int upperThreshold;
	int lowerThreshold;
	
	for (const auto& image_name : names) {
		//int upperThreshold = 150;
		//int lowerThreshold = 100;

		string name = image_name.substr(image_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));

		//string thres = to_string(upperThreshold) + "_" + to_string(lowerThreshold);
		//name = name + thres;

		Mat src_image = imread(image_name);
		//Mat des_image = MyCanny(src_image, upperThreshold, lowerThreshold);
		
		Mat des_image = MyCanny3(src_image,thresholds,edgePercent);

		upperThreshold = thresholds[0];
		lowerThreshold = thresholds[1];
		string thres = to_string(lowerThreshold) + "_" + to_string(upperThreshold);
		name = name +"_"+ thres;

		//imshow("des.pic", des_image);
		//waitKey();
		std::cout << "finished one picture" << endl;
		imwrite(outFolder + name + ".png", des_image);

		//IplImage *theimg = cvLoadImage(image_name.c_str(), 1);
		//IplImage *img_nor = NormalizeImage( theimg);
		//cvShowImage("normalized img", img_nor);
		//waitKey();
	}
	delete[]thresholds;
	std::cout << "end" << endl;


}

void cutImage(int x1, int y1, int x2, int y2) {
	Mat img = imread("resource\\test\\img_5.jpg");
	//cout << img.rows 
	cout << "success" << endl;
	string fileName = "resource\\test\\info.txt";
	//out.open("Hello.txt", ios::in | ios::out | ios::binary)
	fstream fout;
	if (_access("fileName", 0)) {
		fout.open(fileName, ios::app);
		
	}
	else {
		cout << "file create" << endl;
		fout.open(fileName, ios::out);
		fout << img.cols << " " << img.rows << endl;
		fout << "x1  " << "y1  " << "x2  " << "y2  " << endl;
		
	}
	if (!fout.is_open()) {
		cout << "fail to create the txt file!" << endl;
		return;
	}
	
	//Rect rect(720, 610,300,320);
	//
	//cout << img.cols << " " << img.rows << endl;
	//fout << "sdsafds" << endl;
	Rect rect(x1, y1, x2, y2);
	
	fout << x1 <<" " << y1 <<"  " << x2 << "  "<< y2 << endl;
	Mat image_roi = img(rect);
	//imshow("cutted image",image_roi);
	//waitKey();
	string  image_name = "resource\\test\\find2_y2\\test_" + to_string(x1) + "_" + to_string(y1) + "_" + to_string(x2) + "_" + to_string(y2) + ".jpg";
	imwrite(image_name, image_roi);
	fout.close();


}
void findROI() {
	//Mat img = imread("resource\\test\\img_5.jpg");
	//cout << img.rows << " "<<img.cols << endl;
	cout << "start" << endl;
	int x1 = 0;
	int y1 = 0;
	int x2 = 863;
	int y2 = 687;
	int i = 0;
	//for (; x1 < 530, y1 < 190; x1 += 1, y1 += 1, i++) {
	//	cout << "move "<< i << endl;
	//	cutImage(x1, y1, x2, y2);
	//}

	//for (; x1 <= 530; x1 += 1,  i++) {
	//	cout << "move " << i << endl;
	//	cutImage(x1, y1, x2, y2);
	//}

	//for (; y1 <= 190;  y1 += 1, i++) {
	//	cout << "move " << i << endl;
	//	cutImage(x1, y1, x2, y2);
	//}

	//for (;x2 < 1920 - 530, y2 < 1080 - 186;x2 += 10, y2 += 10, i++) {
	//	cout << "move " << i << endl;
	//	cutImage(x1, y1, x2, y2);
	//}

	//cutImage(530, 186, 800, 690);

	for (;y2 >= 660 ;y2 -= 1, i++) {
		cout << "move " << i << endl;
		cutImage(x1, y1, x2, y2);
	}


	cout << "end" << endl;
}

void applyHoughCircle() {
	int max_radius = 55;
	double param1 = 30;
	double param2 = 100;
	//Mat src;
	Mat medianImg;
	Mat grayImg;
	Mat houghCirclesImg;
	string res_path = "resource\\testSquareRes\\2019-02-25_12-08-24_ROI\\";
	string out_path = "resource\\testSquareRes\\";
	//string file_str[5] = { "2019-02-25_12-05-26_3","2019-02-25_12-08-24","2019-02-25_12-12-23","2019-02-25_12-17-49","2019-02-25_12-23-39" };
	string file_name = "hough_circles_info.txt";

	for (int i = 0; i < 1; i++) {

		//string img_path = res_path + file_str[i] + "\\";
		string img_path = res_path;


		vector<string> names;

		glob(img_path + "*.*", names);
		//string saving_path = out_path + file_str[i]+"_param1" +to_string(int(param1))+"_param2" + to_string(int(param2))+ "\\" ;
		string saving_path = out_path + "2019-02-25_12-08-24_houghCircle_param1_" + to_string(int(param1)) + "_param2_" + to_string(int(param2)) + "\\";
		makeDirectoty(saving_path);
		int count = 0;
		fstream fout;
		fout.open(saving_path + file_name,  ios::out );
		if (!fout.is_open()) {
			cout << "File opening error!" << endl;
			return;
		}
		fout << "radius" << "  " << "pcircles[i][0]" << "  " << "pcircles[i][1]" << endl;

		//int count2 = 0;
		for (const auto & image_name : names) {
			cout << "processing " << image_name << endl;
			
			//cout << "1" << endl;
			string name = image_name.substr(image_name.find_last_of("\\") + 1);
			name = name.substr(0, name.find_last_of("."));
			string save_name = saving_path + name + ".jpg";
			

			Mat src = imread(image_name);

			//fout << "img_1205" << endl;
			//Mat src = imread("resource\\applyPyramid\\2019-02-25_12-05-26_1\\img_1205.jpg");
			Mat workImg = src.clone();
			if (src.empty()) {
				cout << "fail to load Img" << endl;
				return;
			}


			cvtColor(workImg, grayImg, CV_BGR2GRAY);
			medianBlur(grayImg, medianImg, 3);

			vector<Vec3f> pcircles;
			HoughCircles(medianImg, pcircles, CV_HOUGH_GRADIENT, 1.5, 20, param1, param2, 0, max_radius);
			//HoughCircles(medianImg, pcircles, CV_HOUGH_GRADIENT, 1.5, 84, 100.0, 100.0, 0, max_radius);
			//HoughCircles(medianImg, pcircles, CV_HOUGH_GRADIENT, 1.5, 84);
			cout << pcircles.size() << endl;
			if (pcircles.size() != 0) 
				fout << name << endl;
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
			imwrite(save_name, src);
			//if (count == 0)
			//	break;
		}
		//if (count == 0)
		//	return;
		fout.close();
	}
	

}
void testHough() {
	Mat src;
	Mat medianImg;
	Mat grayImg;
	Mat houghCirclesImg;
	string img_path = "resource\\imgFromVideoROI\\";
	string out_path = "resource\\applyHough\\";
	string resourceFolder = "resource\\";
	//string imageFolder = resourceFolder + "source_img\\2019-02-25_12-05-26";
	//2019-02-25_12-08-24
	//string imageFolder = resourceFolder + "source_img\\2019-02-25_12-08-24";
	string videoFolder = resourceFolder + "lastVideo\\";


	vector<string> names;
	//glob(img_path + "\\" + "*.*", names);
	//vector<string> names;

	glob(videoFolder + "*.*", names);

	for (const auto & video_name : names) {
		//cout << video_name << endl;
		//string name = video_name.substr(0, name.find_last_of("."));
		string name = video_name.substr(video_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		//cout << name << endl << endl;
		string resultFolder = out_path + name + "\\";
		makeDirectoty(resultFolder);

		string imgPath = img_path + name + "\\";

		//cout << "reading path " << imgPath << endl;
		

		vector<string> img_names;
		glob(imgPath + "*.*", img_names);

		for (const auto & image_name : img_names) {
			cout << "processing " << image_name << endl;
			//cout << "1" << endl;
			string _name = image_name.substr(image_name.find_last_of("\\") + 1);
			_name = _name.substr(0, _name.find_last_of("."));


		//	int count = 0;
		//	int min_dist = 10;
		//	fstream fout;
		//	//cout << "IMAGE_NAME  "<< imgPath << endl;
		//	fout.open(out_path + name + "\\" + "info.txt", ios::app);
		//	cout << out_path + name + "\\" + "info.txt" << endl;
		//	if (!fout.is_open()) {
		//		cout << "fail to open the txt file!" << endl;
		//		return;
		//	}


			src = imread(image_name);
			if (src.empty()) {
				cout << "fail to load Img" << endl;
				return;
			}


			cvtColor(src, grayImg, CV_BGR2GRAY);
			medianBlur(grayImg, medianImg, 3);

		//	
		//	for (;min_dist < 500;min_dist += 10) {
		//		vector<Vec3f> pcircles;

		//		Mat src_copy = src.clone();
		//		//imshow("src_copy", src_copy);
		//		//imshow("src", src);
		//		waitKey();
		//		HoughCircles(medianImg, pcircles, CV_HOUGH_GRADIENT, min_dist, 100);
		//		cout << pcircles.size() << endl;
		//		for (size_t i = 0; i < pcircles.size(); i++) {
		//			Point center(cvRound(pcircles[i][0]), cvRound(pcircles[i][1]));
		//			int radius = cvRound(pcircles[i][2]);
		//			circle(src_copy, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		//			circle(src_copy, center, radius, Scalar(0, 255, 0), 3, 8, 0);
		//		}

		//		//namedWindow("circles", 0);
		//		//imshow("circles", src);
		//		string saving_path = out_path + "\\" + name + "\\" + _name +"_"+to_string(min_dist) +"_"+ ".jpg";
		//		//cout << "saving path " << saving_path << endl;
		//		fout << min_dist << "  " << pcircles.size() << endl;
		//		imwrite(saving_path, src_copy);

		//	}

			vector<Vec3f> pcircles;

			HoughCircles(medianImg, pcircles, CV_HOUGH_GRADIENT, 1.5, 84);
			//cout << pcircles.size() << endl;
			for (size_t i = 0; i < pcircles.size(); i++) {
				Point center(cvRound(pcircles[i][0]), cvRound(pcircles[i][1]));
				int radius = cvRound(pcircles[i][2]);
				circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
				circle(src, center, radius, Scalar(0, 255, 0), 3, 8, 0);
			}

			//namedWindow("circles", 0);
			//imshow("circles", src);
			string saving_path = out_path + "\\" + name + "\\" + _name + ".jpg";
			//cout << "saving path " << saving_path << endl;
			imwrite(saving_path, src);

			//fout.close();
			//if (count == 0)
			//	return;

		}

	}


	
	//for (const auto & image_name : names) {
	//	string name = image_name.substr(image_name.find_last_of("\\") + 1);
	//	name = name.substr(0, name.find_last_of("."));
	//	src = imread(image_name);
	//	if (src.empty()) {
	//		cout << "fail to load Img" << endl;
	//		return;
	//	}

	//	//namedWindow("Input_Img");
	//	//imshow("Input_Img", src);
	//	cvtColor(src, grayImg, CV_BGR2GRAY);
	//	medianBlur(grayImg, medianImg, 3);
	//	//imshow("medianImg", medianImg);

	//	
	//	//imshow("grayImg", grayImg);
	//	vector<Vec3f> pcircles;

	//	HoughCircles(medianImg, pcircles, CV_HOUGH_GRADIENT, 1.5, 10);
	//	//cout << pcircles.size() << endl;
	//	for (size_t i = 0; i < pcircles.size(); i++) {
	//		Point center(cvRound(pcircles[i][0]), cvRound(pcircles[i][1]));
	//		int radius = cvRound(pcircles[i][2]);
	//		circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
	//		circle(src, center, radius, Scalar(0, 255, 0), 3, 8, 0);
	//	}

	//	//namedWindow("circles", 0);
	//	//imshow("circles", src);
	//	imwrite(out_path+"\\" + name +".jpg", src);

	//	//waitKey();
	//}
}

void test_min_dist() {
	Mat medianImg;
	Mat grayImg;
	int count = 0;
	int min_dist = 10;
	fstream fout;
	//cout << "IMAGE_NAME  "<< imgPath << endl;
	fout.open("resource\\test_min_dist_res\\info.txt", ios::app);
	//cout << out_path + name + "\\" + "info.txt" << endl;
	if (!fout.is_open()) {
		cout << "fail to open the txt file!" << endl;
		return;
	}


	Mat src = imread("resource\\test_min_dist\\img_283.jpg");
	if (src.empty()) {
		cout << "fail to load Img" << endl;
		return;
	}


	cvtColor(src, grayImg, CV_BGR2GRAY);
	medianBlur(grayImg, medianImg, 3);


	for (;min_dist < 500;min_dist += 1) {
		vector<Vec3f> pcircles;

		Mat src_copy = src.clone();
		//imshow("src_copy", src_copy);
		//imshow("src", src);
		waitKey();
		HoughCircles(medianImg, pcircles, CV_HOUGH_GRADIENT, min_dist, 100);
		cout << pcircles.size() << endl;
		for (size_t i = 0; i < pcircles.size(); i++) {
			Point center(cvRound(pcircles[i][0]), cvRound(pcircles[i][1]));
			int radius = cvRound(pcircles[i][2]);
			circle(src_copy, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			circle(src_copy, center, radius, Scalar(0, 255, 0), 3, 8, 0);
		}

		//namedWindow("circles", 0);
		//imshow("circles", src);
		string saving_path = "resource\\test_min_dist_res\\img_283_" + to_string(min_dist) + "_" + ".jpg";
		//cout << "saving path " << saving_path << endl;
		fout << min_dist << "  " << pcircles.size() << endl;
		imwrite(saving_path, src_copy);
	}
}


void applyClahe() {
	string res_path = "resource\\imgFromVideoROI\\";
	string out_path = "resource\\applyClahe\\";
	clock_t t0 = clock();
	string file_str[5] = {"2019-02-25_12-05-26","2019-02-25_12-08-24","2019-02-25_12-12-23","2019-02-25_12-17-49","2019-02-25_12-23-39" };
	for (int i = 0;i < 5;i++) {
		string img_path = res_path + file_str[i] + "\\";
		vector<string> names;

		glob(img_path + "*.*", names);
		string saving_path = out_path + file_str[i] + "\\";
		makeDirectoty(saving_path);

		for (const auto & image_name : names) {
			clock_t t1 = clock();
			string name = image_name.substr(image_name.find_last_of("\\") + 1);
			name = name.substr(0, name.find_last_of("."));
			string save_name = saving_path + name + ".jpg";

			Mat img = imread(image_name);
			if (!img.data) {
				cout << "read image error" << endl;
			}
			//imshow("source", img);

			Mat clahe_img;
			cv::cvtColor(img, clahe_img, CV_BGR2Lab);
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
			clock_t t2 = clock();
			clock_t t3 = (t2 - t1)*1.0 / CLOCKS_PER_SEC * 1000;
			cout << "using time : " << t3 << endl;
		
		}

	}
	clock_t t4 = clock();
	clock_t t5 = (t4 - t0)*1.0 / CLOCKS_PER_SEC * 1000;
	cout << "using time for all picture : " << t5 << endl;
	//Mat img = imread("resource\\");
	//if (!img.data) {
	//	cout << "read image error" << endl;
	//}
	//imshow("source", img);
	//Mat clahe_img;
	//cvtColor(img, clahe_img, CV_BGR2Lab);
	//vector<Mat> channels(3);
	//split(clahe_img, channels);

	//Ptr<CLAHE> clahe = createCLAHE();
	//clahe->setClipLimit(4.);
	//Mat dst;
	//clahe->apply(channels[0], dst);
	//dst.copyTo(channels[0]);

	//clahe->apply(channels[1], dst);
	//dst.copyTo(channels[1]);

	//clahe->apply(channels[2], dst);
	//dst.copyTo(channels[2]);

	//merge(channels, clahe_img);

	//Mat image_clahe;
	//cvtColor(clahe_img, image_clahe, CV_Lab2BGR);
	//imshow("image clahe", image_clahe);
	//imwrite("resource\\clahe_result\\img.jpg", image_clahe);
	//waitKey();

}

void applyHoughLine() {
	string res_path = "resource\\applyClahe\\";
	string out_path = "resource\\applyHoughLine\\";
	clock_t t0 = clock();
	string file_str[5] = { "2019-02-25_12-05-26","2019-02-25_12-08-24","2019-02-25_12-12-23","2019-02-25_12-17-49","2019-02-25_12-23-39" };
	for (int i = 0;i < 5;i++) {
		string img_path = res_path + file_str[i] + "\\";
		vector<string> names;

		glob(img_path + "*.*", names);
		string saving_path = out_path + file_str[i] + "\\";
		makeDirectoty(saving_path);

		for (const auto & image_name : names) {
			clock_t t1 = clock();
			string name = image_name.substr(image_name.find_last_of("\\") + 1);
			name = name.substr(0, name.find_last_of("."));
			string save_name = saving_path + name + ".jpg";

			Mat img = imread(image_name);
			Mat workImg = img.clone();
			//workImg = 
			if (!workImg.data) {
				cout << "read image error" << endl;
			}
			Mat CannyImg;
			//Mat Image = imread(img_name);
			Canny(workImg, CannyImg, 140, 250, 3);
			//imshow("CannyImg", CannyImg);

			//Mat DstImg;
			//cvtColor(workImg, DstImg, CV_GRAY2BGR);

			vector<Vec4i> Lines;
			HoughLinesP(CannyImg, Lines, 1, CV_PI / 360, 170, 30, 15);
			for (size_t i = 0; i < Lines.size(); i++)
			{
				line(img, Point(Lines[i][0], Lines[i][1]), Point(Lines[i][2], Lines[i][3]), Scalar(0, 0, 255), 2, 8);
			}
			//imshow("HoughLines_Detect", DstImg);
			imwrite(save_name, img);
			clock_t t2 = clock();
			cout << "using time : " << (t2 - t1)*1.0 / CLOCKS_PER_SEC << endl;
		}
	}
	clock_t t3 = clock();
	cout << "using time for all pic : " << (t3 - t0)*1.0 / CLOCKS_PER_SEC << endl;
	//string img_name;
	
	
	//Canny(Image, CannyImg, 140, 250, 3);
	//imshow("CannyImg", CannyImg);

	//Mat DstImg;
	//cvtColor(Image, DstImg, CV_GRAY2BGR);

	//vector<Vec4i> Lines;
	//HoughLinesP(CannyImg, Lines, 1, CV_PI / 360, 170, 30, 15);
	//for (size_t i = 0; i < Lines.size(); i++)
	//{
	//	line(DstImg, Point(Lines[i][0], Lines[i][1]), Point(Lines[i][2], Lines[i][3]), Scalar(0, 0, 255), 2, 8);
	//}
	//imshow("HoughLines_Detect", DstImg);
	//imwrite("HoughLines_Detect.jpg", DstImg);
}

void applyPyramid() {
	string res_path = "resource\\applyClahe\\";
	//string res_path = "resource\\applyPyramid\\";
	string out_path = "resource\\applyPyramid\\";
	clock_t t0 = clock();
	string file_str[5] = { "2019-02-25_12-05-26","2019-02-25_12-08-24","2019-02-25_12-12-23","2019-02-25_12-17-49","2019-02-25_12-23-39" };
	for (int i = 0;i < 1;i++) {
		string img_path = res_path + file_str[i] + "\\";
		vector<string> names;

		glob(img_path + "*.*", names);
		string saving_path = out_path + file_str[i] + "\\";
		makeDirectoty(saving_path);
		int count = 0;
		for (const auto & image_name : names) {
			clock_t t1 = clock();
			string name = image_name.substr(image_name.find_last_of("\\") + 1);
			name = name.substr(0, name.find_last_of("."));
			string save_name = saving_path + name + ".jpg";

			

			Mat img = imread(image_name);
			Mat workImg = img.clone();


			//cout << image_name << endl;
			//cout << workImg.cols << "   " << workImg.rows << endl;
			//workImg = 
			if (!workImg.data) {
				cout << "read image error" << endl;
			}
			Mat dstImage;

			
			pyrDown(workImg, dstImage, Size(workImg.cols / 2, workImg.rows / 2));

			//cout << workImg.cols << "   " << workImg.rows << endl;
			//cout << dstImage.cols  << "   " << dstImage.rows << endl;
			//if (count == 0)
			//	return;
			
			//imwrite(save_name, dstImage);
			imwrite("resource\\applyPyramid\\2019-02-25_12-05-26\\"+name+".jpg", dstImage);
			clock_t t2 = clock();
			cout << "using time : " << (t2 - t1)*1.0 / CLOCKS_PER_SEC << endl;
		}
	}
	clock_t t3 = clock();
	cout << "using time for all pic : " << (t3 - t0)*1.0 / CLOCKS_PER_SEC << endl;





	//string image_name;
	//Mat img = imread(image_name);
	//Mat workImg = img.clone();
	////workImg = 
	//if (!workImg.data) {
	//	cout << "read image error" << endl;
	//}
	//Mat dstImage;
	//pyrDown(workImg, dstImage, Size(workImg.cols / 2, workImg.rows / 2));
	//imwrite(save_name, img);

}

void findSquare() {
	string res_path = "resource\\test\\testSquare\\";

	string out_path = "resource\\testSquareRes\\";
	clock_t t0 = clock();
	string file_str[5] = { "2019-02-25_12-08-24","2019-02-25_12-05-26","2019-02-25_12-12-23","2019-02-25_12-17-49","2019-02-25_12-23-39" };
	for (int i = 0;i < 1;i++) {
		string img_path = res_path + file_str[i] + "\\";
		vector<string> names;

		glob(img_path + "*.*", names);
		string saving_path = out_path + file_str[i] + "\\";

		string saving_path1 = out_path + file_str[i] + "_ROI\\";
		string saving_path2 = out_path + file_str[i] + "_square\\";

		makeDirectoty(saving_path1);
		makeDirectoty(saving_path2);
		int count = 0;
		for (const auto & image_name : names) {
			clock_t t1 = clock();
			string name = image_name.substr(image_name.find_last_of("\\") + 1);
			name = name.substr(0, name.find_last_of("."));
			string save_name1 = saving_path1 + name + ".jpg";
			string save_name2 = saving_path2 + name + ".jpg";



			Mat img = imread(image_name);
			Mat workImg = img.clone();
			if (!workImg.data) {
				cout << "read image error" << endl;
			}
			//Mat dstImage;

			Rect select;
			select.x = 300;
			select.y = 30;
			select.width = 400;
			select.height = 250;

			rectangle(workImg, select, Scalar(0, 0, 255), 3, 8, 0);

			Mat dstImage = img(select);
			//imshow("cutted image",image_roi);
			//waitKey();
			//string  image_name = "resource\\test\\find2_y2\\test_" + to_string(x1) + "_" + to_string(y1) + "_" + to_string(x2) + "_" + to_string(y2) + ".jpg";
			imwrite(save_name1, dstImage);
			
			imwrite(save_name2, workImg);
			clock_t t2 = clock();
			cout << "using time : " << (t2 - t1)*1.0 / CLOCKS_PER_SEC << endl;
		}
	}
	clock_t t3 = clock();
	cout << "using time for all pic : " << (t3 - t0)*1.0 / CLOCKS_PER_SEC << endl;



}