#include <stdio.h>
#include <iostream>
#include <string>
#include <array>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

bool isSmall(const array<int, 2> arr1, const array<int, 2> arr2){
	if (arr1[0] < arr2[0]) return true;
	else if (arr1[0] == arr2[0]){
		if( arr1[1] <= arr2[1]) return true;
	}
	else if (arr1[0] > arr2[0]) return false;
}

bool isSmaller(const array<int, 3> arr1, const array<int, 3> arr2){
	if (arr1[0] < arr2[0]) return true;
	else return false;
}

array<int, 2> get(const int& adid, const int& queryid, const int& posi, const int& dep, vector<array<int,5>>* dataofuser){
	int index2 = 3 * dep + posi;
	array<int, 2> result = { 0, 0 };

	for (unsigned i = 0; i < dataofuser->size(); i++){
		if ((*dataofuser)[i][0] == adid && (*dataofuser)[i][1] == queryid && (*dataofuser)[i][2] == index2){
			result[0] += (*dataofuser)[i][3];
			result[1] += (*dataofuser)[i][4];
		}
	}

	return result;
}


vector < array<int, 2> > clicked(vector<array<int, 5>>*  dataofuser){
	vector < array<int, 2> > ofUser;
	array<int, 2> ptrToOK;
	for (unsigned i = 0; i < dataofuser->size(); i++){
		if ((*dataofuser)[i][3] >= 1){
			ptrToOK[0] = (*dataofuser)[i][0];
			ptrToOK[1] = (*dataofuser)[i][1];
			ofUser.push_back(ptrToOK);
		}	
	}
	sort(ofUser.begin(), ofUser.end(), isSmall);
	auto iter = unique(ofUser.begin(), ofUser.end());
	ofUser.erase(iter, ofUser.end());

	return ofUser;
}


vector<int> impressMore1(vector<array<int, 5>>* u1, vector<array<int, 5>>* u2){
	vector<int> both;

	vector<int> uu1, uu2;
	for (unsigned i = 0; i < (*u1).size(); i++){
		if ((*u1)[i][4] >= 1) {
			uu1.push_back((*u1)[i][0]);
		}
	}
	for (unsigned i = 0; i < (*u2).size(); i++){
		if ((*u2)[i][4] >= 1) {
			uu2.push_back((*u2)[i][0]);
		}
	}
	sort(uu1.begin(), uu1.end());
	sort(uu2.begin(), uu2.end());
	auto i1 = unique(uu1.begin(), uu1.end());
	auto i2 = unique(uu2.begin(), uu2.end());
	uu1.erase(i1, uu1.end());
	uu2.erase(i2, uu2.end());

	int i = 0, j = 0;
	while (i < uu1.size() && j < uu2.size()){
		if (uu1[i] < uu2[j])
			i++;
		else if (uu1[i] > uu2[j])
			j++;
		else if (uu1[i] = uu2[j]){
			both.push_back(uu1[i]);
			i++; j++;
		}
	}

	return both;
}




int main(int argc, char *argv[]){
	int click, impression, adID, advID, queryID, keyID, titleID, descript, depth, pos, userID;
	unsigned long long disURL;
	FILE *fp;
	fp = fopen(argv[1], "r");
	map<int, vector<array<int, 5>> > arr_user;
	vector<array<int, 5>> user_data;
	array<int, 5> other;

	map<int, vector<string> > ad_otherData;
	map < int, vector<array<int,3>> > ad_user;
	array<int,3> ad_data;
	for (unsigned i = 0; fscanf(fp, "%d %d %llu %d %d %d %d %d %d %d %d %d \n", &click, &impression, &disURL, &adID, &advID, &depth, &pos, &queryID, &keyID, &titleID, &descript, &userID) != EOF; i++){
		other[0] = adID;
		other[1] = queryID;
		other[2] = 3*depth + pos;
		other[3] = click;
		other[4] = impression;
		arr_user[userID].push_back(other);

		string other_data = "\t" + to_string(disURL) + " " + to_string(advID) + " " + to_string(keyID) + " " + to_string(titleID) + " " + to_string(descript);
		ad_otherData[adID].push_back(other_data);
		ad_data[0] = userID;
		ad_data[1] = click;
		ad_data[2] = impression;
		ad_user[adID].push_back(ad_data);
		

	}

	//read and load the data into data structure

	string inputline;
	vector<string> output;
	

	while (getline(cin, inputline)){
		istringstream splitline(inputline);
		string determine;
		splitline >> determine;
		if (determine == "quit")	break; 

		else if (determine == "get"){
			int userid, adid, query, posi, dep;
			splitline >> userid >> adid >> query >> posi >> dep;
			vector<array<int, 5>>* ptrUser = &arr_user[userid];
			array<int,2> result_click_impression = get(adid, query, posi, dep, ptrUser);
			string out = "********************\n" + to_string( result_click_impression[0]) + " " + to_string( result_click_impression[1] ) + "\n********************\n";
			cout << out;
		}

		else if (determine == "clicked"){
			int userid;
			splitline >> userid;
			vector<array<int, 5>>* ptrUser = &arr_user[userid];

			vector < array<int, 2> > vec_ptr_user = clicked(ptrUser);

			string out = "********************\n";
			for (unsigned i = 0; i < vec_ptr_user.size(); i++){
				out += to_string(vec_ptr_user[i][0]);
				out += " ";
				out += to_string(vec_ptr_user[i][1]);
				out += "\n";
			}
			out += "********************\n";
			cout << out;
		}

		else if (determine == "impressed"){
			int u1, u2;
			splitline >> u1 >> u2;
			vector<array<int, 5>>* ptrU1 = &arr_user[u1]; 
			vector<array<int, 5>>* ptrU2 = &arr_user[u2];
			vector<int> bigger1 = impressMore1(ptrU1,ptrU2);

			vector<string> result;
			for (int i = 0; i < bigger1.size(); i++){
				result.push_back( to_string( bigger1[i]) );
				vector<string> tmp_result;
				for (int j = 0; j < ad_user[bigger1[i]].size(); j++){
					if (ad_user[bigger1[i]][j][2] >= 1 && (ad_user[bigger1[i]][j][0] == u1 || ad_user[bigger1[i]][j][0] == u2))
						tmp_result.push_back(   ad_otherData[ bigger1[i] ][j]   );
				}
				sort(tmp_result.begin(), tmp_result.end());
				auto iii = unique(tmp_result.begin(), tmp_result.end());
				tmp_result.erase(iii, tmp_result.end());
				for (int k = 0; k < tmp_result.size(); k++){
					result.push_back(tmp_result[k]);
				}

			}
			
			string total ="********************\n";
			for (int i = 0; i < result.size(); i++){
				total = total + result[i] + "\n";
			}
			total += "********************\n";

			cout << total;

		}


		else if (determine == "profit"){
			int ad;
			double sida;
			splitline >> ad >> sida;

			vector<array<int, 3>> tmp_vec;
			for (int i = 0; i < ad_user[ad].size(); i++){
				array<int, 3> arr_tmp;
				arr_tmp[0] = ad_user[ad][i][0];
				arr_tmp[1] = ad_user[ad][i][1];
				arr_tmp[2] = ad_user[ad][i][2];
				tmp_vec.push_back(arr_tmp);
			}
			
			sort(tmp_vec.begin(), tmp_vec.end(), isSmaller);
						
			vector<array<int, 3>> tmp_result;
			tmp_result.push_back(tmp_vec[0]);
			int jjj = 0;
			for (unsigned i = 1; i < tmp_vec.size(); i++){
				if (tmp_result[jjj][0] == tmp_vec[i][0]){
					tmp_result[jjj][1] += tmp_vec[i][1];
					tmp_result[jjj][2] += tmp_vec[i][2];
				}
				else {
					tmp_result.push_back(tmp_vec[i]);
					jjj++;
				}
			}
						
			vector<int> result;
			for (unsigned i = 0; i < tmp_result.size(); i++){
				double compare;
				if (tmp_result[i][2] != 0){
					compare = (tmp_result[i][1] + 0.0) / tmp_result[i][2];
				}
				else compare = 0.0;
				if (compare >= sida){
					result.push_back(tmp_result[i][0]);
				}
			}

			string profitTotal = "********************";
			for (unsigned i = 0; i < result.size(); i++){
				profitTotal = profitTotal + "\n" + to_string(result[i]);
			}
			profitTotal += "\n********************\n";

			cout << profitTotal;
			
		}

	}


	return 0;
}
