#include "stdafx.h"
#include "veinReconstruction.h"

void labelTheVein() {
	fstream fin("resource\\veinReconstruction\\artery.txt", ios::in);
	if (!fin.is_open()) {
		cout << "Error opening the file!" << endl;
		return;
	}
	string str_name;
	fin >> str_name;
	int num;
	fin >> num;
	float r, x, y;
	fin >> r >> x >> y;

	fin.close();
	float x_v = x + 250.0;
	float y_v = y - 50.0;
	float r_v = r - 10.0;
	int radius = int(r_v);
	Point center(cvRound(x_v), cvRound(y_v));
	string img_name = "resource\\veinReconstruction\\img_4491.jpg";
	string save_name = "resource\\veinReconstruction\\img_4491_vein.jpg";
	Mat src = imread(img_name);
		
	circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
	circle(src, center, radius, Scalar(0, 255, 0), 3, 8, 0);
	imwrite(save_name, src);

}
void labelTheVein(string img_file, string txt_file,string out_path,string out_txt_file) {
	//string file_name = out_txt_file;
	string img_path = img_file;
	vector<string> names;
	glob(img_path + "*.*", names);
	sort(names.begin(), names.end(), cmp);
	
	string saving_path = out_path;
	makeDirectoty(saving_path);
	ofstream fout(out_txt_file, ios::out);
	if (!fout.is_open()) {
		cerr << "Error: " << strerror(errno) << endl;
		return;
	}
	fstream fin(txt_file, ios::in);
	if (!fin.is_open()) {
		cout << "Error opening the file!" << endl;
		return;
	}
	for (int j = 0; j < names.size(); j++) {
		string image_name = names[j];
		string name = image_name.substr(image_name.find_last_of("\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		string save_name = saving_path + name + ".jpg";
		
		float fl[4];
		for (int i = 0; i < 4; i++) {
			fin >> fl[i];
			if (fl[i] == NULL)
				break;
		}
		//float r_v = fl[1] - 10.0;
		float r_v = 20.0;
		float x_v = fl[2] + 250.0;
		float y_v = fl[3] - 50.0;
		Mat src = imread(image_name);
		int radius = int(r_v);
		Point center(cvRound(x_v), cvRound(y_v));
		circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		circle(src, center, radius, Scalar(0, 255, 0), 3, 8, 0);
		imwrite(save_name, src);
		fout << fl[0] << endl
			<< radius << endl
			<< x_v << endl
			<< y_v << endl;

	}
	fout.close();
	fin.close();
}
void getVeinSecquence() {

}