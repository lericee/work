#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <time.h>

using namespace std;


struct OneDatum {
public:
	OneDatum() = default;
	int yesno;
	array< double, 1025> dense = { };

};


void decisionTree( vector<OneDatum>& vo , double epsilon) {
	
	double conf;
	int tmp_a=0, tmp_b=0;
	for (unsigned u = 0; u < vo.size(); u++){
		if (vo[u].yesno == 1) tmp_a++;
		else tmp_b++;
	}
	conf = 1 - pow((tmp_a + 0.0) / (tmp_a + tmp_b), 2) - pow((tmp_b + 0.0) / (tmp_a + tmp_b), 2);
	

	vector<array<double, 1025>> tmpvvv;
	for (unsigned i = 0; i < vo.size(); i++){
		tmpvvv.push_back(vo[i].dense);
	}
	sort(tmpvvv.begin(), tmpvvv.end());
	auto  iter = unique(tmpvvv.begin(), tmpvvv.end());
	tmpvvv.erase(iter, tmpvvv.end());


	if (conf <= epsilon){
		if (tmp_a > tmp_b) cout << "\treturn +1;" << endl;
		else if (tmp_b > tmp_a) cout << "\treturn -1;" << endl;
		else{
			srand(time(NULL));
			int ran = rand() % 2;
			if (ran == 0) cout << "\treturn +1;" << endl;
			else cout << "\treturn -1;" << endl;
		}

	}

	else if (  tmpvvv.size() == 1 ){
		if (tmp_a > tmp_b) cout << "\treturn +1;" << endl;
		else if (tmp_b > tmp_a) cout << "\treturn -1;" << endl;
		else{
			srand(time(NULL));
			int ran = rand() % 2;
			if (ran == 0) cout << "\treturn +1;" << endl;
			else cout << "\treturn -1;" << endl;
		}
	}

	else {
		double min_conf = 10;
		double cut_value;
		int cut_index;
		int conf_a, conf_b, conf_c, conf_d;

		for (unsigned i = 0; i < vo[0].dense.size(); i++){
			vector<double> vd;
			for (unsigned j = 0; j < vo.size(); j++){
				vd.push_back(vo[j].dense[i]);
			}
			sort(vd.begin(), vd.end());
			auto it = unique(vd.begin(), vd.end());
			vd.erase(it, vd.end());

			if (vd.size() > 1){
				for (int k = 0; k < vd.size()-1; k++){
					double threshold;
					threshold = (vd[k] + vd[k + 1]) / 2; 
					int a = 0, b = 0, c = 0, d = 0;
					for (unsigned l = 0; l < vo.size(); l++){
						if (vo[l].dense[i] > threshold){
							if (vo[l].yesno == 1) a++;
							else b++;
						}
						else {
							if (vo[l].yesno == 1) c++;
							else d++;
						}
					}
					double total_confusion = (a + b + 0.0) / (a + b + c + d)*(1 - pow(a / (a + b + 0.0), 2) - pow(b / (a + b + 0.0), 2)) + (c + d + 0.0) / (a + b + c + d)*(1 - pow(c / (c + d + 0.0), 2) - pow(d / (c + d + 0.0), 2));
					if (total_confusion < min_conf){
						min_conf = total_confusion;
						cut_value = threshold;
						cut_index = i;
						conf_a = a;
						conf_b = b;
						conf_c = c;
						conf_d = d;
					}
				}
			}
		}

		vector<OneDatum> vecLeft, vecRight;


		for (unsigned i = 0; i < vo.size(); i++){
			if (vo[i].dense[cut_index] > cut_value) { vecLeft.push_back(vo[i]); }
			else { vecRight.push_back(vo[i]); }
		}

		cout << "\tif (attr[" << cut_index << "] > " << cut_value << "){" << "\n\t";
		decisionTree(vecLeft, epsilon);
		cout << "\t}" << endl;
		cout << "\telse {\n\t";
		decisionTree(vecRight, epsilon);
		cout << "\t}" << endl;

	}

}


int main(int argc, char** argv ){

	ifstream fin;
	fin.open(argv[1]);
	string ep = argv[2];
	double epsilon = atof(ep.c_str());

	vector < OneDatum > vcTrain;
	string line;
	while (getline(fin, line)){
		istringstream lys(line);		
		int yes_no;
		lys >> yes_no;
		OneDatum aLine;
		aLine.yesno = yes_no;
		int index;
		while (lys >> index){
			char comma;
			double value;
			lys >> comma >> value;
			aLine.dense[index] = value;
		}
		vcTrain.push_back(aLine);
	}

	
	cout << "int tree_predict(double *attr){" << endl;
	decisionTree(vcTrain, epsilon);
	cout << "}" << endl;



	return 0;
}