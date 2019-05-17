#include "stdafx.h"
#include "ReduceError.h"


CReduceError::CReduceError()
{
	Precision = 1000.0;
	VeinPrecision = 300.0;
}


CReduceError::~CReduceError()
{
}
void CReduceError::readFile(string infile, string outfile) {
	//float Precision;
	fstream fin(infile, ios::in);
	if (!fin.is_open()) {
		cout << " file open ERROR!" << endl;
		return;
	}
	vector<Vec4f> circles;
	vector<Vec3f> this_circle;
	while (!fin.eof()) {
		string img_name;
		fin >> img_name;
		//cout << img_name << endl;
		if (img_name == "")
			continue;
		int pos1 = img_name.find_last_of("_") + 1;
		int length = img_name.size() - pos1;
		img_name = img_name.substr(pos1, length);

		float name = stof(img_name);
		//cout << name << endl;
		int count;
		fin >> count;
		//cout << count << endl;
		int last_circle = circles.size() - 1;
		if (last_circle == -1) {
			//assumed the first image has a right detected position about the artery
			float fl[3];
			fin >> fl[0] >> fl[1] >> fl[2];
			Vec4f vec4f(name, fl[0], fl[1], fl[2]);
			circles.push_back(vec4f);
			continue;
		}
		float center_x = circles[last_circle][2];
		float center_y = circles[last_circle][3];
		if (count == 1) {
			float fl[3];
			fin >> fl[0] >> fl[1] >> fl[2];
			float dis_x = pow(fl[1] - center_x, 2);
			float dis_y = pow(fl[2] - center_y, 2);
			float dis = dis_x + dis_y;
			//cout << count << " " << dis << endl;
			if (dis < Precision) {
				Vec4f the_one(name, fl[0], fl[1], fl[2]);
				circles.push_back(the_one);
			}
			else {
				Vec4f this_one = circles[last_circle];
				this_one[0] = name;
				circles.push_back(this_one);
			}
		}
		else {
			for (int i = 0; i < count; i++) {
				float fl[3];
				fin >> fl[0] >> fl[1] >> fl[2];
				Vec3f vec3f(fl[0], fl[1], fl[2]);
				this_circle.push_back(vec3f);
			}

			vector<float> distance;
			for (int i = 0; i < count; i++) {
				float dis_x = pow(this_circle[i][1] - center_x, 2);
				float dis_y = pow(this_circle[i][2] - center_y, 2);
				float dis = dis_x + dis_y;
				//cout << i + 1 << " " << dis << endl;
				distance.push_back(dis);
			}
			float min_dis = distance[0];
			int index = 0;
			for (int i = 1; i < count; i++) {
				if (distance[i] < min_dis) {
					index = i;
					min_dis = distance[i];
				}
			}
			if (min_dis < Precision) {
				Vec4f the_one(name, this_circle[index][0], this_circle[index][1], this_circle[index][2]);
				circles.push_back(the_one);
			}
			else {
				Vec4f this_one = circles[last_circle];
				this_one[0] = name;
				circles.push_back(this_one);
			}
		}
		//cout << endl;
	}
	fin.close();
	
	//int pos_str = infile.find_last_of("\\");
	//string file_path = infile.substr(0, pos_str + 1);
	//string out_file_name = file_path + outfile;

	//cout << out_file_name << endl;
	fstream fout(outfile, ios::out);
	if (!fout.is_open()) {
		cout << "file opening error" << endl;
		return;
	}
	for (int i = 0; i < circles.size();i++) {
		circles[i][1] = 40.0;
		if (i % 5 == 0) {
			fout << circles[i][0] << endl
				<< circles[i][1] << endl
				<< circles[i][2] << endl
				<< circles[i][3] << endl;
			//<< endl;
		}
	}
	fout.close();

}
void CReduceError::reduceVeinError(string infile,string outfile) {
	fstream fin(infile, ios::in);
	if (!fin.is_open()) {
		cout << " file open ERROR!" << endl;
		return;
	}
	vector<Vec4f> circles;
	vector<Vec3f> this_circle;
	int count = 1;
	while (!fin.eof()) {
		string img_name;
		fin >> img_name;
		//cout << img_name << endl;
		if (img_name == "")
			continue;
		int pos1 = img_name.find_last_of("_") + 1;
		int length = img_name.size() - pos1;
		img_name = img_name.substr(pos1, length);

		float name = stof(img_name);
		// << name;
		//cout << name << endl;

		int last_circle = circles.size() - 1;
		if (last_circle == -1) {
			//assumed the first image has a right detected position about the artery
			float fl[3];
			fin >> fl[0] >> fl[1] >> fl[2];
			Vec4f vec4f(name, fl[0], fl[1], fl[2]);
			circles.push_back(vec4f);
			continue;
		}
		float center_x = circles[last_circle][2];
		float center_y = circles[last_circle][3];

		float fl[3];
		fin >> fl[0] >> fl[1] >> fl[2];
		float dis_x = pow(fl[1] - center_x, 2);
		float dis_y = pow(fl[2] - center_y, 2);
		float dis = dis_x + dis_y;
		//cout << name << " dis = " << dis << endl;
		count++;
		if (dis < VeinPrecision) {
			Vec4f the_one(name, fl[0], fl[1], fl[2]);
			circles.push_back(the_one);
		}
		else {
			Vec4f this_one = circles[last_circle];
			this_one[0] = name;
			circles.push_back(this_one);
		}


		//cout << endl;
	}
	fin.close();

	//int pos_str = infile.find_last_of("\\");
	//string file_path = infile.substr(0, pos_str + 1);
	//string out_file_name = file_path + outfile;

	//cout << out_file_name << endl;
	fstream fout(outfile, ios::out);
	if (!fout.is_open()) {
		cout << "file opening error" << endl;
		return;
	}
	for (int i = 0; i < circles.size();i++) {
		circles[i][1] = 20.0;
		if (i % 5 == 0) {
			fout << circles[i][0] << endl
				<< circles[i][1] << endl
				<< circles[i][2] << endl
				<< circles[i][3] << endl;
		}
		//<< endl;
	}
	fout.close();
	cout << circles.size() << endl;
}
void CReduceError::writeResult(string outfile) {

}