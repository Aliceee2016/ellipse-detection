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
	string imageFolder = resourceFolder + "source_img\\2019-02-25_12-05-26";
	string videoFolder = resourceFolder + "video\\";
	string imageFromVideo = resourceFolder + "imgFromVideo\\";
	string outFolder = resourceFolder + "imgWithEdge\\2019-02-25_12-05-26\\";

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