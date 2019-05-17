#include "stdafx.h"
#include "ObjLoader.h"

CObjLoader::CObjLoader()
{

}
CObjLoader::CObjLoader(string filename)
{
	string line;
	fstream fin;
	fin.open(filename, ios::in);
	if (!fin.is_open()) {
		cout << "Error opening the obj file" << endl;
		return;
	}
	while (!fin.eof()) {
		//getline(fin, line);
		string str[4];
		fin >> str[0] >> str[1] >> str[2] >> str[3];
		vector<string> parameters;
		//string tailMark = " ";
		//string ans = "";
		for (int i = 0; i < 4; i++) {
			//cout << str[0] <<" "<< str[1]<<" " << str[2] <<" "<< str[3]<<endl;
			parameters.push_back(str[i]);
		}
		//line = line + tailMark;
		//for (int i = 0; i < line.length(); i++) {
		//	char ch = line[i];
		//	if (ch != ' ')
		//		ans += ch;
		//	else {
		//		//cout << ans << endl;
		//		parameters.push_back(ans);
		//		ans = "";
		//	}
		//}
		if (parameters.size() != 4) {
			cout << parameters.size() << endl;
			cout << "The size is not correct!" << endl;
			for (int i = 0; i < parameters.size(); i++) {
				cout << parameters.at(i) << endl;
			}
			return;

		}
		
		 
			
			
		else {
			if (parameters[0] == "v") {
				vector<GLfloat> Point;
				for (int i = 1; i < 4; i++) {
					GLfloat xyz = atof(parameters[i].c_str())/50.0;
					Point.push_back(xyz);
				}
				vSets.push_back(Point);
			}
			else if (parameters[0] == "f") {
				vector<GLint> vIndexSets;
				for (int i = 1; i < 4; i++) {
					string x = parameters[i];
					string ans = "";
					for (int j = 0; j < x.length(); j++) {
						char ch = x[j];
						if (ch != '/') {
							ans += ch;
						}
						else
							break;
					}
					GLint index = atof(ans.c_str());
					index = index--;
					vIndexSets.push_back(index);
				}
				fSets.push_back(vIndexSets);
			}
		}
	}
	fin.close();

}


CObjLoader::~CObjLoader()
{
}
void CObjLoader::Draw() {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < fSets.size();i++) {
		GLfloat VN[3];
		GLfloat SV1[3];
		GLfloat SV2[3];
		GLfloat SV3[3];
		if ((fSets[i]).size() != 3) {
			cout << "the size of fSets is not correct!" << endl;
			return;
		}
		else {
			GLint firstVertexIndex = (fSets[i])[0];
			GLint secondVertexIndex = (fSets[i])[1];
			GLint thirdVertexIndex = (fSets[i])[2];
			//cout << firstVertexIndex << " " << secondVertexIndex << " " << thirdVertexIndex << endl;
			for (int k = 0; k < 3; k++) {
				SV1[k] = (vSets[firstVertexIndex])[k];
				SV2[k] = (vSets[secondVertexIndex])[k];
				SV3[k] = (vSets[thirdVertexIndex])[k];

				//cout << k << endl;
				//cout << SV1[k] << endl;
				//cout << SV2[k] << endl;
				//cout << SV3[k] << endl;
			}
			
			
			GLfloat vec1[3], vec2[3], vec3[3];
			//(x2-x1,y2-y1,z2-z1)
			//(x3-x2,y3-y2,z3-z2)
			//for (int k = 0; k < 3; k++) {
			//	vec1[k] = SV1[k] - SV2[k];
			//	vec2[k] = SV1[k] - SV3[k];
			//	
			//}
			//(x2-x1,y2-y1,z2-z1)
			vec1[0] = SV1[0] - SV2[0];
			vec1[1] = SV1[1] - SV2[1];
			vec1[2] = SV1[2] - SV2[2];

			//(x3-x2,y3-y2,z3-z2)
			vec2[0] = SV1[0] - SV3[0];
			vec2[1] = SV1[1] - SV3[1];
			vec2[2] = SV1[2] - SV3[2];

			//(x3-x1,y3-y1,z3-z1)
			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));
			VN[0] = vec3[0] / D;
			VN[1] = vec3[1] / D;
			VN[2] = vec3[2] / D;
			
			glNormal3f(VN[0], VN[1], VN[2]);

			glVertex3f(SV1[0], SV1[1], SV1[2]);
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);
		}
	}
	glEnd();
}
void CObjLoader::convertToTriangle(string filename) {
	fstream fin(filename, ios::in);
	if (!fin.is_open()) {
		cout << "error open " << filename << endl;
		return;
	}
	string name = filename.substr(0, filename.find_last_of("."));
	cout << name << endl;
	string fileOut = name + "_triangle.txt";
	cout << fileOut << endl;
	fstream fout(fileOut, ios::out);
	while (!fin.eof()) {
		string str[5];
		for (int i = 0; i < 5;i++) {
			fin >> str[i];
			if (str[i] == "f") {
				if (i == 0)
					continue;
				else {
					cout << "error" << endl;
					return;
				}
			}
		}
		fout << str[0] << " " << str[1] << " " << str[2] << " " << str[3] << " " << endl
			<< str[0] << " " << str[3] << " " << str[4] << " " << str[1] << " " << endl;

	}
	fin.close();
	fout.close();
}
void CObjLoader::processing() {
	string filename = "resource\\mesh\\mesh_netural.txt";
	fstream fin(filename, ios::in);
	if (!fin.is_open()) {
		cout << "error opening " << filename << endl;
		return;
	}
	string res = "resource\\mesh\\mesh_netural_triangle_second.obj";
	fstream fout(res, ios::out);
	if (!fout.is_open()) {
		cout << "error opening " << res << endl;
		return;
	}
	while (!fin.eof()) {
		string line;
		getline(fin, line);
		if (line[0] == 'v') {
			//for(int i = 0; i < line.length() - 1; i++)
			//	fout << line[i];
			//fout << endl;
			fout << line << endl;
		}
		else
			break;
	}
	fin.close();
	filename = "resource\\mesh\\last_triangle.txt";
	fin.open(filename, ios::in);
	while (!fin.eof()) {
		string line;
		getline(fin, line);
		fout << line << endl;
	}
	fin.close();
	fout.close();


}
