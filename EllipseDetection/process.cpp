#include "stdafx.h"
#include "process.h"


void processing3_26() {
	string in_file = "resource\\imageFeb\\imgFromVideoROI\\2019-02-25_12-08-24\\";
	string out_file = "resource\\imageFeb\\PrepareforReconstruction\\2019-02-25_12-08-24_with_square_2\\";
	makeDirectoty(out_file);
	int x = 200;
	int y = 0;
	int width = 600;
	int height = 360;
	cutImage(in_file, out_file, x, y, width, height);
	string videoFileName = "resource\\video\\rawdatafeburary2019\\2019-02-25_12-08-24.mp4";
	string resultFileName = "resource\\imageFeb\\PrepareforReconstruction\\2019-02-25_12-08-24\\";
	//makeDirectoty(resultFileName);
	videoToImages(1, 01, videoFileName, resultFileName);
	string roi_filePath_in = "resource\\imageFeb\\PrepareforReconstruction\\2019-02-25_12-08-24_manually_select\\origin\\";
	string roi_filePath_out = "resource\\imageFeb\\PrepareforReconstruction\\2019-02-25_12-08-24_manually_select\\roi\\";
	//makeDirectoty(roi_filePath_out);
	//Feb image ROI 530, 186, 863, 689
	//cutImage(roi_filePath_in, roi_filePath_out, 530, 186, 863, 689);
	string clahe_filePath_in = "resource\\imageFeb\\PrepareforReconstruction\\2019-02-25_12-08-24_manually_select\\roi\\";
	string clahe_filePath_out = "resource\\imageFeb\\PrepareforReconstruction\\2019-02-25_12-08-24_manually_select\\clahe\\";
	//makeDirectoty(clahe_filePath_out);
	applyClahe(clahe_filePath_in, clahe_filePath_out);

	string select_square_out = "resource\\imageFeb\\PrepareforReconstruction\\2019-02-25_12-08-24_manually_select\\square\\";

	//square for 2019-02-25_12-08-24 150 0 700 460
	//int x = 150;
	//int y = 0;
	//int width = 700;
	//int height = 460;

	//makeDirectoty(clahe_filePath_out);
	cutImage(clahe_filePath_out, select_square_out, x, y, width, height);
	string hough_in = "resource\\imageFeb\\PrepareforReconstruction\\data\\";
	string hough_out = "resource\\imageFeb\\PrepareforReconstruction\\data_hough\\";
	//makeDirectoty(hough_out);

	double edge_percent = 0.7;
	double para2 = 55;
	int maxRadius = 55;
	int minDist = 500;
	int minRadius = 30;
	string out = hough_out + "edge_" + to_string(int(edge_percent * 100)) + "%_param2_" + to_string(int(para2)) + "_maxRadius_" + to_string(maxRadius) + "_minDist_" + to_string(minDist) + "\\";
	houghCircleTxt(hough_in, out, edge_percent, para2,  maxRadius,  minRadius,  minDist);

	string fileName = "resource\\imageFeb\\PrepareforReconstruction\\data_txt\\info.txt";
	string fileName2 = "resource\\imageFeb\\PrepareforReconstruction\\data_txt\\info_addOffset.txt";
	double x1 = 150.0;
	double y1 = 0.0;
	addOffset(fileName, fileName2, x1, y1);

	//string filePath = "resource\\imageFeb\\PrepareforReconstruction\\2019-02-25_12-08-24_manually_select\\hough_trans\\edge_70%_param2_55_maxRadius_55_minDist_500\\hough_circles_info.txt";
	//fstream fin(filePath, ios::in);
	//if (!fin.is_open()) {
	//	cout << "file opening error!" << endl;
	//	return;
	//}
	//string filePath2 = "resource\\imageFeb\\PrepareforReconstruction\\testData.txt";
	//fstream fout(filePath2, ios::out);
	//if (!fout.is_open()) {
	//	cout << "file opening error!" << endl;
	//	return;
	//}
	//string line;
	//getline(fin, line);
	//while (line != "img_4466") {
	//	getline(fin, line);
	//}
	//while (!fin.eof() && line != "img_4589") {
	//	fout << line << endl;
	//	getline(fin, line);
	//}
	//fin.close();
	//fout.close();
}


void processing4_4() {
	string in_file = "resource\\imageMarch\\ROI_CLAHE\\2019-03-18_12-37-15\\";
	string out_file = "resource\\result\\";
	//makeDirectoty(out_file);
	string day = time_day();
	string out2 = out_file + day + "\\";
	//makeDirectoty(out2);
	string out3 = out2 + "circle_detection\\";
	//makeDirectoty(out3);
	string out4 = out3 + "2019-03-18_12-37-15\\";
	//makeDirectoty(out4);
	//string thistime_now = time_now();
	//string out_file_path = out4 + thistime_now + "\\";

	//string a = "img_1879789.jpg";
	//string b = "img_2.jpg";
	//cout << cmp(a, b) << endl;
	//double edgePercent, double param2, int maxRadius, int minRadius, int minDist
	double edge_percent = 0.7;
	double param2 = 70;
	int maxRadius = 55;
	int minRadius = 30;
	int minDist = 100;
	string out_file_path = out4 + "edge_" + to_string(int(edge_percent * 100)) + "%_param2_" + to_string(int(param2)) + "_maxRadius_" + to_string(maxRadius) + "_minDist_" + to_string(minDist) + "\\";
	//makeDirectoty(out_file_path);
	string out_file_square = "resource\\result\\2019-4-4\\square\\2019-03-18_12-37-15\\";
	//string time_now_this = time_now();
	int x = 200;
	int y = 70;
	int width = 600;
	int height = 350;

	out_file_square = out_file_square + "x_" + to_string(x) + "_y_" + to_string(y) + "_width_" + to_string(width) + "_height_" + to_string(height) + "\\";
	makeDirectoty(out_file_square);
	drawRectangle(in_file, out_file_square, x, y, width, height);
	
	//houghCircle(in_file, out_file_path, edge_percent, param2, maxRadius, minRadius, minDist);
}
void processingTwoImage() {
	string in_file = "resource\\imageMarch\\ROI_CLAHE\\twoImage\\";
	string out_file = "resource\\imageMarch\\hough\\twoImage\\";
	int x1 = 319;//445 for 2019-03-18_12-57-24_1 //319 for normal image march
	int x2 = 964;
	int x3 = 445;

	int y = 191;

	int width = 1290;//1290,645,1035
	int width3 = 1035;

	int height = 690;
	string str[7] = { "2019-03-18_12-41-21", "2019-03-18_12-40-23","2019-03-18_12-43-39","2019-03-18_12-54-16","2019-03-18_12-56-20","2019-03-18_12-57-24","2019-03-18_13-03-00" };
	
	//string fin = "resource\\imageMarch\\findROI\\test\\";
	//string fout = "resource\\imageMarch\\findROI\\res_2\\";
	//
	//makeDirectoty(fout);
	//drawRectangle(fin, fout, x1, y, width, height);

	string in_hough = "resource\\imageMarch\\ROI_CLAHE\\oneImage\\neck\\2019-03-18_12-57-24_1\\";
	string out_hough = "resource\\imageMarch\\hough\\oneImage\\2019-03-18_12-57-24_1\\";
	makeDirectoty(out_hough);
	//for (int i = width; i >700 ; i -= 5) {
	//	drawRectangle(fin, fout, x3, y, i, height);
	//}
	//cutImage(fin, fout, x3, y, width3, height);
	//applyClahe(fin, fout);
	//for (int i = 1; i < 7; i++) {
	//	string left = "left_half\\";
	//	string right = "right_half\\";
	//	
	//	string str_in = in_file + str[i] + "\\";
	//	string str_out = out_file + str[i] + "\\";
	//	makeDirectoty(str_out);
	//	string str1_in = str_in + left;

	//		

	//	//cutImage(in_file1, str2, x1, y, width, height);
	//	//applyClahe(str1_in, str1_out);
		double edge_percent = 0.9;
		double param2 = 95;

		int minRadius = 65;
		int maxRadius = 100;
		
		int minDist = 150;
		string out_file_path = out_hough + "txt_edge_" + to_string(int(edge_percent * 100)) + "%_param2_" + to_string(int(param2)) + "_minRadius_" + to_string(minRadius) + "_maxRadius_" + to_string(maxRadius) +  "_minDist_" + to_string(minDist) + "\\";
		makeDirectoty(out_file_path);
	//	string str1_out = out_file_path + left;
	//	makeDirectoty(str1_out);
		houghCircleTxt(in_hough, out_file_path, edge_percent, param2, maxRadius, minRadius, minDist);
		


	//	string str2_in = str_in + right;
	//	string str2_out = out_file_path + right;
	//	makeDirectoty(str2_out);
	//		
	//	//cutImage(in_file1, str3, x2, y, width, height);
	//	//applyClahe(str2_in, str2_out);
	//	houghCircle(str2_in, str2_out, edge_percent, param2, maxRadius, minRadius, minDist);

	//}


	//for (x = x+5,y = y +5;x < 480, y < 300;x += 5, y += 5) {
	//	//string out_file1 = out_file + "_x_" + to_string(x) + "_y_" + to_string(y) + "_width_" + to_string(width) + "_height_" + to_string(height) + "\\";
	//	//makeDirectoty(out_file1);
	//	drawRectangle(in_file, out_file, x, y, width, height);
	//}
	//for ( height =  500; height < 800; height += 5) {
	//	//string out_file1 = out_file + "_x_" + to_string(x) + "_y_" + to_string(y) + "_width_" + to_string(width) + "_height_" + to_string(height) + "\\";
	//	//makeDirectoty(out_file1);
	//	drawRectangle(in_file, out_file, x, y, width, height);
	//}
	//drawRectangle(in_file, out_file, x, y, 645, height);
	//drawRectangle(in_file, out_file, 964, y, 645, height);
	//cutImage(in_file, out_file, x, y, width, height);

}

//resource\imageFeb\PrepareforReconstruction\data_hough\edge_70%_param2_55_maxRadius_55_minDist_500
string ArteryReduceErrorAndGenerateMesh(string in_file,string out_file) {
	CReduceError reducer;
	CGenerateObj genObj;
	
	//string in_path = "resource\\imageFeb\\PrepareforReconstruction\\reduceError\\";
	//string reducer_infile = "resource\\imageFeb\\PrepareforReconstruction\\reduceError\\info_testReduceError.txt";
	



	//string veinTxtFile = "resource\\veinReconstruction\\vein_3\\res_vien_obj_select_300.txt";
	//string infileName = in_path + outfile1;
	//string infileName = "resource\\imageFeb\\PrepareforReconstruction\\data_txt\\info_addOffset.txt";
	string img_file = "";


	string str = time_day();
	string outPath = "resource\\result\\" + str + "\\";
	makeDirectoty(outPath);
	string res_now = outPath + time_now() + "\\";
	makeDirectoty(res_now);

	string reducer_infile = "resource\\imageFeb\\PrepareforReconstruction\\reduceError\\info_testReduceError.txt";
	string reducer_outfile = res_now + "after_reduceError.txt";
	
	//string reducer_infileV = "resource\\veinReconstruction\\vein_3\\info_vein.txt";
	//string reducer_outfileV = res_now + "after_reduceError_vien.txt";
	
	string getObj_outfile = res_now + "obj\\";
	//string getObj_outfileV = res_now + "obj_vien\\";
	
	reducer.readFile(reducer_infile, reducer_outfile);
	genObj.getObj(reducer_outfile, getObj_outfile, 37);


	//void labelTheVein(string img_file, string txt_file, string out_path, string out_txt_file);
	//labelTheVein( img_file, reducer_outfile, img_file_labelVien, reducer_infileV);
	//reducer.reduceVeinError(reducer_infileV, reducer_outfileV);
	//genObj.getObj(reducer_outfileV, getObj_outfileV, 37);
	return reducer_outfile;
}
void VeinReduceErrorAndGenerateMesh(string in_file,string vein_reducer_outfile) {
	CReduceError reducer;
	CGenerateObj genObj;

	string img_file = "";
	string img_file_labelVien = "";

	string str = time_day();
	string outPath = "resource\\result\\" + str + "\\";
	makeDirectoty(outPath);
	string res_now = outPath + time_now() + "\\";
	makeDirectoty(res_now);

	string reducer_infileV = "resource\\veinReconstruction\\vein_3\\info_vein.txt";
	string reducer_outfileV = res_now + "after_reduceError_vien.txt";

	string getObj_outfileV = res_now + "obj_vien\\";




	//void labelTheVein(string img_file, string txt_file, string out_path, string out_txt_file);
	labelTheVein(img_file, vein_reducer_outfile, img_file_labelVien, reducer_infileV);
	reducer.reduceVeinError(reducer_infileV, reducer_outfileV);
	genObj.getObj(reducer_outfileV, getObj_outfileV, 37);
}
void wholeProcessing(string video) {
	int period = 10;
	int file_count = 1;
	string resultFileName = "";
	videoToImages(period, file_count, video, resultFileName);
	string draw_rectangle_file = "";
	int x = 45;
	int y = 45;
	int width = 50;
	int height = 50;
	drawRectangle(resultFileName, draw_rectangle_file, x, y, width, height);
	string cut_image_file = "";
	cutImage(resultFileName, cut_image_file, x, y, width, height);
	string apply_clahe_flie = "";
	applyClahe(cut_image_file, apply_clahe_flie);
	string houcircle_file = "";
	double edgePercent = 0.7;
	double param2 = 55;
	int maxRadius = 55; 
	int minRadius = 30; 
	int minDist = 500;
	houghCircleTxt(apply_clahe_flie, houcircle_file, edgePercent, param2, maxRadius, minRadius, minDist);
	//void ArteryReduceErrorAndGenerateMesh(string in_file, string out_file);
	string artery_out_meshfile = "";

	string aretry_circle = ArteryReduceErrorAndGenerateMesh(houcircle_file, artery_out_meshfile);
	string vein_reducer_outfile = "";
	VeinReduceErrorAndGenerateMesh(aretry_circle,  vein_reducer_outfile);
}

