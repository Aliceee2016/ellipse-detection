#include "stdafx.h"
#include "GenerateObj.h"


CGenerateObj::CGenerateObj()
{
	//5 frames per frame
	ip_num = 1;//insert slices
	numParts_of_circle = 24;
	//slice_dis = 5.0 / (ip_num + 1.0);
	slice_dis = 5;
	x_offset = 450;
	y_offset = 150;
	beginZ = 0;
	string logo_name = "log\\" + time_now() + "_log.txt";
	flog.open(logo_name, ios::out);

}
CGenerateObj::CGenerateObj( int num) {
	ip_num = 4;
	numParts_of_circle = num;
	slice_dis = 5;
	x_offset = 400;
	y_offset = 150;
	beginZ = 0;

}
CGenerateObj::CGenerateObj(string in_file, string out_file, int num, float beginZ) {
	numParts_of_circle = num;
	slice_dis = 10;
	x_offset = 400;
	y_offset = 150;
	beginZ = 0;
	ip_num = 4;

}
void CGenerateObj::saveInfo(string outfile) {
	
	fstream fout(outfile, ios::out);
	if (!fout.is_open()) {
		cout << "file opening error!" << endl;
		return;
	}
	fout << "numParts_of_circle" << endl

		<< numParts_of_circle << endl
		<< "number of circles" << endl
		<< numOfCircles << endl
		<< "begin_pos_of_Z" << endl
		<< beginZ << endl
		<< "slice_distance" << endl
		<< slice_dis << endl
		<< "x_offset" << endl
		<< x_offset << endl
		<< "y_offset" << endl
		<< y_offset << endl
		<< "smoothing method:" << endl
		<< smoothMethod << endl
		<< "ip_num" << endl
		<< ip_num << endl;
	fout.close();
}
void CGenerateObj::getObj(string infileName, string outPath, int smoothMethod) {
	//makeDirectoty(outPath);
	this->smoothMethod = smoothMethod;
	
	string filePath = outPath ;
	makeDirectoty(filePath);
	string vSetsFilename = filePath + "vetex.txt";
	string fSetsFilename = filePath + "fsets.txt";
	string meshfileName = filePath + "mesh.txt";
	string objfileName = filePath + "mesh.obj";
	string infofilename = filePath + "info.txt";

	getvSets( infileName, vSetsFilename, smoothMethod);
	getfSets(fSetsFilename);
	getMesh(meshfileName, fSetsFilename, vSetsFilename);
	getMesh(objfileName, fSetsFilename, vSetsFilename);
	saveInfo(infofilename);

}
void CGenerateObj::getDynamicObj(string infileName, string outPath, int smoothMethod) {
	this->smoothMethod = smoothMethod;
	string filePath = outPath ;
	makeDirectoty(filePath);
	
	string vSetsFilename = filePath + "vetex.txt";
	string fSetsFilename = filePath + "fsets.txt";
	string meshfileName = filePath + "mesh.txt";
	string objfileName = filePath + "mesh.obj";
	string infofilename = filePath + "info.txt";
	for (int i = 0;i < 25; i += 5) {
		string vSetsFilename = filePath + "vetex_" + to_string(i) + ".txt";
		string fSetsFilename = filePath + "fsets_" + to_string(i) + ".txt";
		string meshfileName = filePath + "mesh_" + to_string(i) + ".txt";
		string objfileName = filePath + "mesh_" + to_string(i) + ".obj";
		string infofilename = filePath + "info_" + to_string(i) + ".txt";
		readDynamicFile(infileName, vSetsFilename, smoothMethod, i);
		getfSets(fSetsFilename);
		getMesh(meshfileName, fSetsFilename, vSetsFilename);
		getMesh(objfileName, fSetsFilename, vSetsFilename);
		saveInfo(infofilename);

	}
	//readDynamicFile(infileName, vSetsFilename, smoothMethod);
	
}
void CGenerateObj::getvSets(string infileName, string outfileName, int smoothMethod) {
	//switch (smoothMethod)
	//{
	//case 0:
	//	readFile(infileName, outfileName);
	//	break;
	//default:
	//	readFile(infileName, outfileName, smoothMethod);
	//	break;
	//}
	readFile(infileName, outfileName, smoothMethod);
	
}
void CGenerateObj::readDynamicFile(string infileName, string outfileName, int smoothMethod, int r_offset) {
	vector<Vec4f> circles;
	vector<Vec4f> points;
	vector<float> radius;
	vector<float> center_x;
	vector<float> center_y;

	float fl[4];

	//float beginZ = 1.0;
	//float countZ = beginZ;
	fstream fin(infileName, ios::in);
	fstream fout(outfileName, ios::out);


	if (!fin.is_open()) {
		cout << "file open error" << endl;
		return;
	}
	if (!fout.is_open()) {
		cout << "file open error" << endl;
		return;
	}

	while (!fin.eof()) {
		string str[5];
		for (int i = 0; i < 4; i++) {
			fin >> str[i];
			
		}



		for (int i = 0; i < 3; i++) {
			if (str[i + 1] == "")
				continue;
			fl[i] = str2float(str[1 + i]);
		}
		//move the vessels to another location

		//fl[3] = countZ;
		//Vec4f circle(fl[0], fl[1], fl[2], fl[3]);
		radius.push_back(fl[0]);
		center_x.push_back(fl[1]);
		center_y.push_back(fl[2]);

		//circles.push_back(circle);


	}

	int N = radius.size();
	real_1d_array x_spline;
	x_spline.setlength(N);
	for (int i = 0; i < N; i++) {
		x_spline[i] = i + 1;
	}
	switch (smoothMethod) {
	case 3:
		//cout << "case 3" << endl;
		linearSmooth3(radius, N);
		linearSmooth3(center_x, N);
		linearSmooth3(center_y, N);
		break;
	case 5:
		linearSmooth5(radius, N);
		linearSmooth5(center_x, N);
		linearSmooth5(center_y, N);
		break;
	case 7:
		linearSmooth7(radius, N);
		linearSmooth7(center_x, N);
		linearSmooth7(center_y, N);
		break;
	case 25:
		quadraicSmooth5(radius, N);
		quadraicSmooth5(center_x, N);
		quadraicSmooth5(center_y, N);
		break;
	case 27:
		quadraicSmooth7(radius, N);
		quadraicSmooth7(center_x, N);
		quadraicSmooth7(center_y, N);
		break;
	case 35:
		cubicSmooth5(radius, N);
		cubicSmooth5(center_x, N);
		cubicSmooth5(center_y, N);
		break;
	case 37:
		cubicSmooth7(radius, N);
		cubicSmooth7(center_x, N);
		cubicSmooth7(center_y, N);
		break;
	case 41:
		cubicSpline(x_spline, radius, N, ip_num);
		cubicSpline(x_spline, center_x, N, ip_num);
		cubicSpline(x_spline, center_y, N, ip_num);
	default:
		break;
	}
	cubicSpline(x_spline, radius, N, ip_num);
	cubicSpline(x_spline, center_x, N, ip_num);
	cubicSpline(x_spline, center_y, N, ip_num);
	//cout << "radius.size = " << radius.size() << endl;
	float j = beginZ;
	for (int i = 0; i < radius.size(); i++) {
		j += slice_dis;
		radius[i] = radius[i] + r_offset;
		center_x[i] -= x_offset;
		center_y[i] -= y_offset;
		Vec4f circle(radius[i], center_x[i], center_y[i], j);
		circles.push_back(circle);

	}
	
	getPointAtCircle(circles, points, numParts_of_circle);
	for (int i = 0; i < points.size();i++) {
		//fout << "v " << points[i][0] << " " << points[i][1] << " " << points[i][2] << " " << points[i][3] << endl;
		fout << "v " << points[i][1] << " " << points[i][2] << " " << points[i][3] << endl;

	}

	numOfCircles = circles.size();
	//cout << numOfCircles << endl;

	fin.close();
	fout.close();
}
void CGenerateObj::readFile(string infileName, string outfileName, int smoothMethod) {
	vector<Vec4f> circles;
	vector<Vec4f> points;
	vector<float> radius;
	vector<float> center_x;
	vector<float> center_y;
	
	float fl[4];

	//float beginZ = 1.0;
	//float countZ = beginZ;
	fstream fin(infileName, ios::in);
	fstream fout(outfileName, ios::out);


	if (!fin.is_open()) {
		cout << "file open error" << endl;
		return;
	}
	if (!fout.is_open()) {
		cout << "file open error" << endl;
		return;
	}

	while (!fin.eof()) {
		string str[5];
		for (int i = 0; i < 4; i++) {
			fin >> str[i];
		}
		


		for (int i = 0; i < 3; i++) {
			if (str[i + 1] == "")
				break;
			fl[i] = str2float(str[1 + i]);
		}
		//move the vessels to another location

		//fl[3] = countZ;
		//Vec4f circle(fl[0], fl[1], fl[2], fl[3]);
		radius.push_back(fl[0]);
		center_x.push_back(fl[1]);
		center_y.push_back(fl[2]);
		
		//circles.push_back(circle);
		

	}
	//cout << radius.size() << endl;
	//for (int i = 0; i < radius.size(); i++) {
	//	cout << radius[i]<<" " << center_x[i] << " " << center_y[i] << endl;
	//}

	int N = radius.size();
	real_1d_array x_spline;
	x_spline.setlength(N);
	for (int i = 0; i < N; i++) {
		x_spline[i] = i + 1;
	}
	switch (smoothMethod) {
	case 3:
		//cout << "case 3" << endl;
		linearSmooth3(radius, N);
		linearSmooth3(center_x, N);
		linearSmooth3(center_y, N);
		break;
	case 5:
		linearSmooth5(radius, N);
		linearSmooth5(center_x, N);
		linearSmooth5(center_y, N);
		break;
	case 7:
		linearSmooth7(radius, N);
		linearSmooth7(center_x, N);
		linearSmooth7(center_y, N);
		break;
	case 25:
		quadraicSmooth5(radius, N);
		quadraicSmooth5(center_x, N);
		quadraicSmooth5(center_y, N);
		break;
	case 27:
		quadraicSmooth7(radius, N);
		quadraicSmooth7(center_x, N);
		quadraicSmooth7(center_y, N);
		break;
	case 35:
		cubicSmooth5(radius, N);
		cubicSmooth5(center_x, N);
		cubicSmooth5(center_y, N);
		break;
	case 37:
		cubicSmooth7(radius, N);
		cubicSmooth7(center_x, N);
		cubicSmooth7(center_y, N);
		break;
	case 41:
		cubicSpline(x_spline, radius, N, ip_num);
		cubicSpline(x_spline, center_x, N, ip_num);
		cubicSpline(x_spline, center_y, N, ip_num);
	default:
		break;
	}
	cubicSpline(x_spline, radius, N, ip_num);
	cubicSpline(x_spline, center_x, N, ip_num);
	cubicSpline(x_spline, center_y, N, ip_num);

	//cout << "afer filtering" << endl;
	//for (int i = 0; i < radius.size(); i++) {
	//	cout << radius[i] << " " << center_x[i] << " " << center_y[i] << endl;
	//}
	//cout << "radius.size = " << radius.size() << endl;
	float j = beginZ;
	for (int i = 0; i < radius.size(); i++) {
		j += slice_dis;
		center_x[i] -= x_offset;
		center_y[i] -= y_offset;
		Vec4f circle(radius[i], center_x[i], center_y[i], j);
		circles.push_back(circle);

	}

	getPointAtCircle(circles, points, numParts_of_circle);
	for (int i = 0; i < points.size();i++) {
		//fout << "v " << points[i][0] << " " << points[i][1] << " " << points[i][2] << " " << points[i][3] << endl;
		fout << "v " << points[i][1] << " " << points[i][2] << " " << points[i][3] << endl;

	}

	numOfCircles = circles.size();
	//cout << numOfCircles << endl;

	fin.close();
	fout.close();


}
void CGenerateObj::getPointAtCircle(vector<Vec4f> circles, vector<Vec4f> &points, int number) {
	//cout << "number  = " << number << endl;
	number = numParts_of_circle;
	float angle = 360.0/number;
	float radian = angle_to_radian(angle, 0, 0);
	int count = 1;
	//cout << circles.size() << endl;
	for (int i = 0; i < circles.size(); i++) {
		float radius = circles[i][0];
		float xpos = circles[i][1];
		float ypos = circles[i][2];
		float zpos = circles[i][3];
		for (int j = 0; j < number; j++) {
			float xp = xpos + radius *cos(radian*j);
			float yp = ypos + radius *sin(radian*j);
			//if (abs(xp) < MINVAL) {
			//	xp = 0;
			//}
			//if (abs(yp) < MINVAL) {
			//	yp = 0;
			//}
			Vec4f ppoint(float(count), xp, yp, zpos);
			count++;
			points.push_back(ppoint);

			count++;
			
		}
	}
}
void CGenerateObj::getfSets(string outfileName) {
	fstream fout(outfileName, ios::out);
	if (!fout.is_open()) {
		cout << "file open error" << endl;
		return;
	}
	
	int num_of_points = numOfCircles * numParts_of_circle;
	//cout << "num_of_points = " << num_of_points << endl;
	int count = (numOfCircles - 1) * numParts_of_circle;
	int num = numParts_of_circle;
	for (int i = 1; i <= count; i++) {
		if (i % numParts_of_circle == 0) {
			fout << "f " << i << " " << i - (numParts_of_circle - 1) << " " << i + 1 << endl;
			fout << "f " << i + 1  << " " << i + numParts_of_circle << " " << i << endl;
		}
		else {
			fout << "f " << i << " " << i + 1 << " " << i + 1 + num << endl;
			fout << "f " << i + 1 + num << " " << i + num << " " << i << endl;
		}
		
	}

	fout.close();
}

//get Vsets file
//void CGenerateObj::readFile(string infileName, string outfileName) {
//	vector<Vec4f> circles;
//	vector<Vec4f> points;
//	//float beginZ = 1.0;
//	float countZ = beginZ;
//	fstream fin(infileName, ios::in);
//	fstream fout(outfileName, ios::out);
//
//
//	if (!fin.is_open()) {
//		cout << "file open error" << endl;
//		return;
//	}
//	if (!fout.is_open()) {
//		cout << "file open error" << endl;
//		return;
//	}
//
//	while (!fin.eof()) {
//		string str[5];
//		for (int i = 0; i < 5; i++) {
//			fin >> str[i];
//		}
//		float fl[4];
//		
//		
//		for (int i = 0; i < 3; i++) {
//			fl[i] = str2float(str[2 + i]);
//		}
//		//move the vessels to another location
//
//		fl[3] = countZ;
//		Vec4f circle(fl[0], fl[1], fl[2], fl[3]);
//		
//		circles.push_back(circle);
//		countZ += slice_dis;
//
//	}
//	//for (int i = 0; i < circles.size();i++) {
//	//	cout << circles[i][0] << " " << circles[i][1] << " " << circles[i][2] << " " << circles[i][3] << endl;
//	//}
//	//int numParts_of_circle = 24;
//	getPointAtCircle(circles, points, numParts_of_circle);
//	for (int i = 0; i < points.size();i++) {
//		//fout << "v " << points[i][0] << " " << points[i][1] << " " << points[i][2] << " " << points[i][3] << endl;
//		fout << "v " << points[i][1] << " " << points[i][2] << " " << points[i][3] << endl;
//
//	}
//	//int numCircle = circles.size();
//	//cout << circles.size() << endl;
//	numOfCircles = circles.size();
//	
//
//	fin.close();
//	fout.close();
//
//}

void CGenerateObj::process() {
	Vec4f cicle(1, 0, 0, 1);
	vector<Vec4f> circles;
	circles.push_back(cicle);
	vector<Vec4f> points;
	int number = 8;
	getPointAtCircle(circles, points, number);
	//cout << points.size() << endl;
	for (int i = 0; i < points.size();i++) {
		cout << points[i][0] <<" "<< points[i][1]<<" " << points[i][2]<<" " << points[i][3] << endl;
	}
}
void CGenerateObj::getMesh(string meshFile, string fsetsFile, string vextexFile) {
	fstream fin(vextexFile, ios::in);
	ofstream fout(meshFile, ios::out);


	if (!fin.is_open()) {
		cout << "file open error" << endl;
		return;
	}
	if (!fout.is_open()) {
		cout << "file open error" << endl;
		return;
	}
	while (!fin.eof()) {
		string line;
		getline(fin, line);
		//cout << line << endl;
		fout << line << endl;
	}
	fin.close();
	//fout.close();

	

	fstream finF(fsetsFile, ios::in);
	//fstream foutF(meshFile, ios::app);
	if (!finF.is_open()) {
		cout << "file open error" << endl;
		return;
	}
	while (!finF.eof()) {
		string line;
		getline(finF, line);
		fout << line << endl;
	}

	finF.close();
	fout.close();
	
}
CGenerateObj::~CGenerateObj()
{
	flog.close();
}
float angle_to_radian(float degree, float min, float second)
{
	float flag = (degree < 0) ? -1.0 : 1.0;
	if (degree<0)
	{
		degree = degree * (-1.0);
	}
	float angle = degree + min / 60 + second / 3600;
	float result = flag * (angle * float(CV_PI) )/ 180;
	return result;
	//cout<<result<<endl;
}
float str2float(string num) {
	float res;
	stringstream stream(num);
	stream >> res;
	return res;
}
//Three-point linear smoothing
void CGenerateObj::linearSmooth3(vector<float> &x, int N) {
	vector<float> res;
	if (N < 3) {
		for (int i = 0; i < N; i++) {
			res.push_back(x[i]);
		}
	}
	else {
		float tmp = (5.0 * x[0] + 2.0 * x[1] - x[2]) / 6.0;
		res.push_back(tmp);
		for (int i = 1;i < N - 1;i++) {
			tmp = (x[i - 1] + x[i] + x[i + 1]) / 3.0;
			res.push_back(tmp);
		}
		tmp = (5.0 * x[N - 1] + 2.0 * x[N - 2] - x[N - 3]) / 6.0;
		res.push_back(tmp);
	}
	for (int i = 0; i < x.size(); i++) {
		x[i] = res[i];
	}

	//cout << "At function" << endl;
	//for (int i = 0; i < x.size();i++) {
	//	cout << res[i] << endl;
	//	//cout << circles[i][0] << " " << circles[i][1] << " " << circles[i][2] << " " << circles[i][3] << endl;
	//}
}
void CGenerateObj::linearSmooth5(vector<float> &x, int N) {
	vector<float> res;
	if (N < 5) {
		for (int i = 0; i < N; i++) {
			res.push_back(x[i]);
		}
	}
	else {
		float tmp = (3.0 * x[0] + 2.0 * x[1] + x[2] - x[4]) / 5.0;
		res.push_back(tmp);
		tmp = (4.0 * x[0] + 3.0 * x[1] + 2.0 * x[2] + x[3]) / 10.0;
		res.push_back(tmp);
		for (int i = 2; i < N - 2; i++) {
			tmp = (x[i - 2] + x[i - 1] + x[i] + x[i + 1] + x[i + 2]) / 5.0;
			res.push_back(tmp);
		}
		tmp = (4.0 * x[N - 1] + 3.0 * x[N - 2] + 2.0 * x[N - 3] + x[N - 4]) / 10.0;
		res.push_back(tmp);
		tmp = (3.0 * x[N - 1] + 2.0 * x[N - 2] + x[N - 3] - x[N - 5]) / 5.0;
		res.push_back(tmp);
		
	}
	for (int i = 0; i < x.size(); i++) {
		x[i] = res[i];
	}
}
void CGenerateObj::linearSmooth7(vector<float> &x, int N) {
	vector<float> res;
	if (N < 7) {
		for (int i = 0; i < N; i++) {
			res.push_back(x[i]);
		}
	}
	else {
		float tmp = (13.0 * x[0] + 10 * x[1] + 7.0 * x[2] + 4.0 * x[3] + x[4] - 2.0 * x[5] - 5.0 * x[6]) / 28.0;
		res.push_back(tmp);
		tmp = (5.0 * x[0] + 4.0 * x[1] + 3.0 * x[2] + 2.0 * x[3] + x[4] - x[5]) / 14.0;
		res.push_back(tmp);
		tmp = (7.0 * x[0] + 6.0 * x[1] + 5.0 * x[2] + 4.0 * x[3] + 3.0 * x[4] + 2.0 * x[5] + 1.0 * x[6]) / 28.0;
		res.push_back(tmp);
		for (int i = 3; i < N - 3; i++) {
			float sum = 0.0;
			for (int j = i - 3; j < i + 4; j++) {
				sum += x[j];
			}
			tmp = sum / 7.0;
			res.push_back(tmp);
		}
		tmp = (7.0 * x[N - 1] + 6.0 * x[N - 2] + 5.0 * x[N - 3] + 4.0 * x[N - 4] + 3.0 * x[N - 5] + 2.0 * x[N - 6] + x[N - 7]) / 28.0;
		res.push_back(tmp);
		tmp = (5.0 * x[N - 1] + 4.0 * x[N - 2] + 3.0 * x[N - 3] + 2.0 * x[N - 4] + x[N - 5] - x[N - 7]) / 14.0;
		res.push_back(tmp);
		tmp = (13.0 * x[N - 1] + 10.0 * x[N - 2] + 7.0 * x[N - 3] + 4.0 * x[N - 4] + x[N - 5] - 2 * x[N - 6] - 5 * x[N - 7]) / 28.0;
		res.push_back(tmp);
	}
	for (int i = 0; i < x.size(); i++) {
		x[i] = res[i];
	}
}

void CGenerateObj::quadraicSmooth5(vector<float> &x, int N) {
	vector<float> res;
	float tmp;
	if (N < 5) {
		for (int i = 0; i < N; i++) {
			res.push_back(x[i]);
		}
	}
	else {
		tmp = (31.0 * x[0] + 9.0 * x[1] - 3.0 * x[2] - 5.0 * x[3] + 3.0 * x[4]) / 35.0;
		res.push_back(tmp);
		float tmp = (9.0 * x[0] + 13.0 * x[1] + 12 * x[2] + 6.0 * x[3] - 5.0 * x[4]) / 35.0;
		res.push_back(tmp);
		for (int i = 2; i < N - 2;i++) {
			tmp = (-3.0 * (x[i - 2] + x[i + 2]) + 12.0 * (x[i - 1] + x[i + 1] ) + 17.0 * x[i]) / 35.0;
			res.push_back(tmp);
		}
	}
	tmp = (9.0 * x[N - 1] + 13.0 * x[N - 2] + 12.0 * x[N - 3] + 6.0 * x[N - 4] - 5.0 * x[N - 5]) / 35.0;
	res.push_back(tmp);
	tmp = (31.0 * x[N - 1] + 9.0 * x[N - 2] - 3.0 * x[N - 3] - 5.0 * x[N - 4] + 3.0 * x[N - 5]) / 35.0;
	res.push_back(tmp);
	for (int i = 0; i < x.size(); i++) {
		x[i] = res[i];
	}
}
void CGenerateObj::quadraicSmooth7(vector<float> &in, int N) {
	vector<float> res;
	float tmp;
	if (N < 7) {
		for (int i = 0; i < N; i++) {
			res.push_back(in[i]);
		}
	}
	else {
		tmp = (32.0 * in[0] + 15.0 * in[1] + 3.0 * in[2] - 4.0 * in[3] - 6.0 * in[4] - 3.0 * in[5] + 5.0 * in[6]) / 42.0;
		res.push_back(tmp);
		tmp = (5.0 * in[0] + 4.0 * in[1] + 3.0 * in[2] + 2.0 * in[3] + in[4] - in[6]) / 14.0;
		res.push_back(tmp);
		tmp = (1.0 * in[0] + 3.0 * in[1] + 4.0 * in[2] + 4.0 * in[3] + 3.0 * in[4] + 1.0 * in[5] - 2.0 * in[6]) / 14.0;
		res.push_back(tmp);
		for (int i = 3; i <= N - 4; i++)
		{
			tmp = (-2.0 * (in[i - 3] + in[i + 3]) + 3.0 * (in[i - 2] + in[i + 2]) + 6.0 * (in[i - 1] + in[i + 1]) + 7.0 * in[i]) / 21.0;
			res.push_back(tmp);
		}
		tmp = (1.0 * in[N - 1] + 3.0 * in[N - 2] + 4.0 * in[N - 3] + 4.0 * in[N - 4] + 3.0 * in[N - 5] + 1.0 * in[N - 6] - 2.0 * in[N - 7]) / 14.0;
		res.push_back(tmp);
		tmp = (5.0 * in[N - 1] + 4.0 * in[N - 2] + 3.0 * in[N - 3] + 2.0 * in[N - 4] + in[N - 5] - in[N - 7]) / 14.0;
		res.push_back(tmp);
		tmp = (32.0 * in[N - 1] + 15.0 * in[N - 2] + 3.0 * in[N - 3] - 4.0 * in[N - 4] - 6.0 * in[N - 5] - 3.0 * in[N - 6] + 5.0 * in[N - 7]) / 42.0;
		res.push_back(tmp);

	}
	for (int i = 0; i < in.size(); i++) {
		in[i] = res[i];
	}
}
void CGenerateObj::cubicSmooth5(vector<float> &in, int N) {
	vector<float> res;
	float tmp;
	if (N < 5) {
		for (int i = 0; i < N; i++) {
			res.push_back(in[i]);
		}
	}
	else {
		tmp = (69.0 * in[0] + 4.0 * in[1] - 6.0 * in[2] + 4.0 * in[3] - in[4]) / 70.0;
		res.push_back(tmp);
		tmp = (2.0 * in[0] + 27.0 * in[1] + 12.0 * in[2] - 8.0 * in[3] + 2.0 * in[4]) / 35.0;
		res.push_back(tmp);
		for (int i = 2; i <= N - 3; i++)
		{
			tmp = (-3.0 * (in[i - 2] + in[i + 2]) + 12.0 * (in[i - 1] + in[i + 1]) + 17.0 * in[i]) / 35.0;
			res.push_back(tmp);
		}
		tmp = (2.0 * in[N - 5] - 8.0 * in[N - 4] + 12.0 * in[N - 3] + 27.0 * in[N - 2] + 2.0 * in[N - 1]) / 35.0;
		res.push_back(tmp);
		tmp = (-in[N - 5] + 4.0 * in[N - 4] - 6.0 * in[N - 3] + 4.0 * in[N - 2] + 69.0 * in[N - 1]) / 70.0;
		res.push_back(tmp);

	}
	for (int i = 0; i < in.size(); i++) {
		in[i] = res[i];
	}
}
void CGenerateObj::cubicSmooth7(vector<float> &in, int N) {
	vector<float> res;
	float tmp;
	if (N < 7) {
		for (int i = 0; i < N; i++) {
			res.push_back(in[i]);
		}
	}
	else {
		tmp = (39.0 * in[0] + 8.0 * in[1] - 4.0 * in[2] - 4.0 * in[3] +
			1.0 * in[4] + 4.0 * in[5] - 2.0 * in[6]) / 42.0;
		res.push_back(tmp);
		tmp = (8.0 * in[0] + 19.0 * in[1] + 16.0 * in[2] + 6.0 * in[3] -
			4.0 * in[4] - 7.0* in[5] + 4.0 * in[6]) / 42.0;
		res.push_back(tmp);
		tmp = (-4.0 * in[0] + 16.0 * in[1] + 19.0 * in[2] + 12.0 * in[3] +
			2.0 * in[4] - 4.0 * in[5] + 1.0 * in[6]) / 42.0;
		res.push_back(tmp);
		for (int i = 3; i <= N - 4; i++)
		{
			tmp = (-2.0 * (in[i - 3] + in[i + 3]) +
				3.0 * (in[i - 2] + in[i + 2]) +
				6.0 * (in[i - 1] + in[i + 1]) + 7.0 * in[i]) / 21.0;
			res.push_back(tmp);
		}
		tmp = (-4.0 * in[N - 1] + 16.0 * in[N - 2] + 19.0 * in[N - 3] +
			12.0 * in[N - 4] + 2.0 * in[N - 5] - 4.0 * in[N - 6] + 1.0 * in[N - 7]) / 42.0;
		res.push_back(tmp);
		tmp = (8.0 * in[N - 1] + 19.0 * in[N - 2] + 16.0 * in[N - 3] +
			6.0 * in[N - 4] - 4.0 * in[N - 5] - 7.0 * in[N - 6] + 4.0 * in[N - 7]) / 42.0;
		res.push_back(tmp);
		tmp = (39.0 * in[N - 1] + 8.0 * in[N - 2] - 4.0 * in[N - 3] -
			4.0 * in[N - 4] + 1.0 * in[N - 5] + 4.0 * in[N - 6] - 2.0 * in[N - 7]) / 42.0;
		res.push_back(tmp);
	}
	for (int i = 0; i < in.size(); i++) {
		in[i] = res[i];
	}
}
void CGenerateObj::cubicSplineForEllipse(real_1d_array x, vector<float> &in, int N, int dis) {
	real_1d_array y;
	spline1dinterpolant s;
	float v;
	y.setlength(N);
	for (int i = 0; i < N; i++) {
		y[i] = in[i];

	}
	spline1dbuildcubic(x, y, s);

	double j = x[0];
	double count = x[N - 1];
	in.clear();
	//int count = (N - 1)*ip_num + N;
	for (int i = 0; i < count;i++) {
		v = spline1dcalc(s, j);
		j += dis;
		//cout << "here" << endl;
		//cout << v << endl;
		in.push_back(v);
	}
	//cout << "j = " << j << endl;
	//numOfCircles = 5 * N 
}
void CGenerateObj::cubicSpline(real_1d_array x,vector<float> &in, int N, int ip_num) {
	real_1d_array y;
	spline1dinterpolant s;
	//float v;
	y.setlength(N);
	for (int i = 0; i < N; i++) {
		y[i] = in[i];
		
	}
	// Test exact boundary conditions: build S(x), calculare S(0.25)
	// (almost same as original function)
	//
	spline1dbuildcubic(x, y, s);
	//v = spline1dcalc(s, t);
	//printf("%.4f\n", double(v)); 
	//t = -0.5;
	//in.clear();
	//cout << "at spline line" << endl;
	//cout << " N = " << N << endl;
	double j = x[0];
	double dis = 1.0 / (ip_num + 1.0);
	in.clear();
	int count = (N - 1)*ip_num + N;
	for (int i = 0; i < count;i ++ ) {
		float v = spline1dcalc(s, j);
		j += dis;
		//cout << "here" << endl;
		//cout << v << endl;
		in.push_back(v);
	}
	//cout << "j = " << j << endl;
	//numOfCircles = 5 * N 

}
void CGenerateObj::getVsetsEllipse(string infilename, string outfilename, int divide, int &num_of_points, int &number_of_ellipse) {
	int num = divide;
	vector<Vec6f> ellipses;
	vector<Vec4f> points;
	vector<float> x_spline;
	vector<float> a_radius;
	vector<float> b_radius;
	vector<float> center_x;
	vector<float> center_y;
	vector<float> ellipse_angle;
	fstream fin(infilename, ios::in);
	fstream fout(outfilename, ios::out);
	if (!fin.is_open()) {
		cout << "File open error!" << endl;
		return;
	}
	if (!fout.is_open()) {
		cout << "File open error!" << endl;
		return;
	}
	while (!fin.eof()) {
		string str;
		fin >> str;
		if (str == "")
			break;
		str = str.substr(str.find('_')+1, str.length() - str.find('_') + 1);
		//cout<<"pic name    " << str << endl;
		float name = str2float(str);
		//cout << "pic name    " << name << endl;
		float x, y, a, b, alpha;
		fin >> x >> y >> a >> b >> alpha;
		x_spline.push_back(name);
		a_radius.push_back(a);
		b_radius.push_back(b);
		center_x.push_back(x);
		center_y.push_back(y);
		ellipse_angle.push_back(alpha);
		cout<<"ellipse   " << name<<" " << x <<" "<< y <<" " << a <<" " << b << " "<<alpha <<endl;
		//Vec6f ellipse(name, x, y, a, b, alpha);
		//ellipses.push_back(ellipse);
	}

	
	cout <<"BEFORE Number of ellipses = "<< center_x.size() << endl;
	int N = a_radius.size();
	real_1d_array x_spline_r1a;
	x_spline_r1a.setlength(N);
	for (int i = 0; i < N; i++) {
		x_spline_r1a[i] = x_spline[i];
	}
	int dis = 1;
	cubicSplineForEllipse(x_spline_r1a, a_radius, N, dis);
	cubicSplineForEllipse(x_spline_r1a, b_radius, N, dis);

	cubicSplineForEllipse(x_spline_r1a, center_x, N, dis);
	cubicSplineForEllipse(x_spline_r1a, center_y, N, dis);

	cubicSplineForEllipse(x_spline_r1a, ellipse_angle, N, dis);
	N = a_radius.size();

	cubicSmooth7(a_radius, N);
	cubicSmooth7(b_radius, N);
	cubicSmooth7(center_x, N);
	cubicSmooth7(center_y, N);
	cubicSmooth7(ellipse_angle, N);


	int ellipse_num = a_radius.size();
	float start_num = x_spline[0];

	

	for (int i = 0; i < a_radius.size();i++) {
		Vec6f ellipse(start_num, center_x[i], center_y[i], a_radius[i], b_radius[i], ellipse_angle[i]);
		start_num += dis;
		ellipses.push_back(ellipse);
		cout << ellipses[i][0] << " " << ellipses[i][1] << " " << ellipses[i][2] << " " << ellipses[i][3] << " " << ellipses[i][4] << " " << ellipses[i][5] << endl;
	}
	number_of_ellipse = ellipses.size();
	cout << "AFTER number_of_ellipse  = " << number_of_ellipse << endl;

	num_of_points = getPointsAtEllipse(ellipses, points, num);
	for (int i = 0;i < points.size();i++) {
		fout << "v " << points[i][1] << " " << points[i][2] << " " << points[i][3] << endl;
		//fout<< points[i][0]<<" "<< points[i][1] << " " << points[i][2] << " " << points[i][3] << endl;
		//cout << "v  " << points[i][0] << " " << points[i][1] << " " << points[i][2] << " " << points[i][3] << endl;

	}

	fin.close();
	fout.close();
}
int CGenerateObj::getPointsAtEllipse(vector<Vec6f> ellipses, vector<Vec4f> &points, int num) {
	cout << "Run at get points at ellipse." << endl;
	//int sum_points = num * 2 + 2;
	int slide_distance_ellipse = 20;
	int add_points = 1;
	for (int i = 0; i < ellipses.size(); i++) {
		float name, x_center, y_center, a, b, alpha;
		name = ellipses[i][0];
		x_center = ellipses[i][1];
		y_center = ellipses[i][2];
		float a_axis = ellipses[i][3];
		float b_axis = ellipses[i][4];

		a = a_axis ;
		b = b_axis ;
		//angle_to_radian
		//alpha = angle_to_radian(ellipses[i][5],0,0);
		float angle = -ellipses[i][5]+90;
		alpha = angle_to_radian(angle, 0, 0);
		//cout << "@  " << x_center << " " << y_center << " " << a_axis << " " << b_axis << " " << angle << endl;

		//cout << "ellipses[i][5] = " << ellipses[i][5] << endl;
		//cout << name << " " << x_center << " " << y_center << " " << a << " " << b << " " << alpha << endl;
		
		float steplenght = 2*a / float(num);
		int count_point = 1;
		
		float x_pos = -a;
		float bpow2 = pow(b, 2);
		float apow2 = pow(a, 2);
		float parab_a = bpow2 / apow2;
		vector<Vec4f> ypositivePoints;
		vector<Vec4f> ynegtivePoints;

		Vec4f p(count_point, -a, 0, float(i + 1)*slide_distance_ellipse);
		ypositivePoints.push_back(p);
		
		int x_pos_copy = x_pos;
		
		float small_step = steplenght / (add_points + 1);
		for (int u = 0;u < add_points;u++) {
			if (u == 0) {
				x_pos_copy = x_pos_copy + small_step / 2.0;
			}
			else {
				x_pos_copy = x_pos_copy + small_step;
			}
			float ypow2 = bpow2 - parab_a * pow(x_pos_copy, 2);
			float y_pos = sqrt(ypow2);
			float y_pos_neg = -y_pos;
			Vec4f point(u, x_pos_copy, y_pos, float(i + 1)*slide_distance_ellipse);
			ypositivePoints.push_back(point);

			Vec4f point_neg(u, x_pos_copy, -y_pos, float(i + 1)*slide_distance_ellipse);
			ynegtivePoints.push_back(point_neg);

		}
	
		int j = 2;
		for (;j < num + 1;j++) {
			x_pos = x_pos + steplenght;
			float ypow2 = bpow2 - parab_a * pow(x_pos, 2);
			float y_pos = sqrt(ypow2);
			float y_pos_neg = -y_pos;

			Vec4f point(j, x_pos, y_pos, float(i + 1)*slide_distance_ellipse);
			ypositivePoints.push_back(point);

			Vec4f point_neg(j, x_pos, -y_pos, float(i + 1)*slide_distance_ellipse);
			ynegtivePoints.push_back(point_neg);
		}
		x_pos_copy = a;


		for (int u = 0;u < add_points;u++) {
			if (u == 0) {
				x_pos_copy = x_pos_copy - small_step/2.0 ;
			}
			else {
				x_pos_copy = x_pos_copy - small_step;
			}		
			float ypow2 = bpow2 - parab_a * pow(x_pos_copy, 2);
			float y_pos = sqrt(ypow2);
			float y_pos_neg = -y_pos;
			Vec4f point(u, x_pos_copy, y_pos, float(i + 1)*slide_distance_ellipse);
			ypositivePoints.push_back(point);

			Vec4f point_neg(u, x_pos_copy, -y_pos, float(i + 1)*slide_distance_ellipse);
			ynegtivePoints.push_back(point_neg);
		}
		
		Vec4f p_a(j, a, 0, float(i + 1)*slide_distance_ellipse);
		ypositivePoints.push_back(p_a);
		j++;
		int start_num = j;
		for (int k = ynegtivePoints.size() - 1; k >= 0; k--) {
			//ynegtivePoints[k][0] = start_num;
			//start_num++;
			ypositivePoints.push_back(ynegtivePoints[k]);
		}

		//if (i == 0) {
		//	fstream fout_firsst("resource\\test\\o_ellipse.txt", ios::out);
		//	cout << "postion of x and y "<< endl;
		//	for (int k = 0; k < points.size();k++) {
		//		//cout<< "@   "<<k<<"  "<< points[k][1] << " " << points[k][2] << endl;
		//		//fout_firsst << points[k][1] << " " << points[k][2] << endl;
		//		fout_firsst << points[k][0] << " " << points[k][1] << " " << points[k][2] << " " << points[k][3] << endl;
		//	}
		//	fout_firsst.close();
		//	cout << "END" << endl;
		//}
		
		for (int k = 0; k < ypositivePoints.size();k++) {
			float x_postion = ypositivePoints[k][1];
			float y_postion = ypositivePoints[k][2];
			ypositivePoints[k][0] = k + 1;
			ypositivePoints[k][1] = x_postion*cos(alpha) + y_postion*sin(alpha) + x_center;
			ypositivePoints[k][2] = x_postion*sin(alpha) - y_postion*cos(alpha) + y_center;
			//cout<<"k    " << ypositivePoints[k][0] << " " << ypositivePoints[k][1] << " " << ypositivePoints[k][2] << " " << ypositivePoints[k][3] << endl;

		}
		for (int k = 0; k < ypositivePoints.size();k++) {
			points.push_back(ypositivePoints[k]);
			//cout << "k    " << ypositivePoints[k][0] << " " << ypositivePoints[k][1] << " " << ypositivePoints[k][2] << " " << ypositivePoints[k][3] << endl;

		}
		
		//
		//if (i == 1)
		//	break;
	}
	int count = num * 2 + add_points * 4;
	return count;
}
void CGenerateObj::getMeshEllipse(string infilename, string outfilename) {

}
void CGenerateObj::getMeshEllipse() {
	string time_tod = time_now();
	string dirpath = "resource\\test\\" + time_tod + "\\";
	makeDirectoty(dirpath);
	string infile = "resource\\test\\ellipse_info_test.txt";

	string fsetsTxtfile = dirpath + "fsets.txt";
	//getVsetsEllipse(string infilename, string outfilename)
	string vsetsfile = dirpath + "vsets.txt";
	string meshFile = dirpath + "mesh.txt";
	string meshFileObj = dirpath + "mesh.obj";
	int num = 12;
	int number_of_ellipse = 0;
	int number_of_points = 0;
	getVsetsEllipse(infile, vsetsfile,num, number_of_points,number_of_ellipse);
	//cout << "number_of_ellipse  = " << number_of_ellipse << endl;
	//cout << "number_of_points  = " << number_of_points << endl;
	getfSets(fsetsTxtfile, number_of_points, number_of_ellipse);
	getMesh(meshFile, fsetsTxtfile, vsetsfile);
	getMesh(meshFileObj, fsetsTxtfile, vsetsfile);
}
void CGenerateObj::getfSets(string outfileName, int number_of_points, int number_of_ellipse){
	fstream fout(outfileName, ios::out);
	if (!fout.is_open()) {
		cout << "file open error" << endl;
		return;
	}

	int num_of_points = number_of_ellipse * number_of_points;
	//cout << "num_of_points = " << num_of_points << endl;
	int count = (number_of_ellipse - 1) * number_of_points;
	int num = number_of_points;
	for (int i = 1; i <= count; i++) {
		if (i % number_of_points == 0) {
			fout << "f " << i << " " << i - (number_of_points - 1) << " " << i + 1 << endl;
			fout << "f " << i + 1 << " " << i + number_of_points << " " << i << endl;
		}
		else {
			fout << "f " << i << " " << i + 1 << " " << i + 1 + num << endl;
			fout << "f " << i + 1 + num << " " << i + num << " " << i << endl;
		}

	}

	fout.close();
}