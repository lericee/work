#include "md5.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <array>
#include <sstream>
#include <algorithm>

using namespace std;

void print(int n, int * a,vector<pair<int,int*>>& record) {
    int* num=new int[n+1]; 
    for (int i=1; i <= n; i++) {
        num[i]=a[i];
    } 
    record.push_back({n,num});
}
void integerPartition(int n, int * a, int level,vector<pair<int,int*>>& record){
    int first; 
    int i; 
    if (n < 1) return ;
    if(n>a[level-1]){    
        a[level] = n;
        print(level, a, record);
    }
    first = (level == 0) ? 1 : a[level-1];
    for(i = first; i <= n / 2; i++){
        if(level==0){
            a[level] = i; 
            integerPartition(n-i, a, level+1, record);
        }
        if(i>a[level-1]){
            a[level] = i; 
            integerPartition(n-i, a, level+1, record);
        }
    }
}

struct Transfer{
	array<string,2> history;
	unsigned int number;
	int time;
	bool dead = false;
};


struct Account{
	string Id;
	string password;
	int createTime;
	unsigned int money = 0;
	vector<Transfer> record;

};

bool transferCmp(const Transfer a, const Transfer b){
	return a.time < b.time;
}

bool wildcmp(const string a, const string b){
	return a < b;
}

bool match(const char *first, const char * second)
{
	if (*first == '\0' && *second == '\0')
		return true;

	if (*first == '*' && *(first + 1) != '\0' && *second == '\0')
		return false;
	if (*first == '?' && *second == '\0')
		return false;

	if (*first == '?' || *first == *second)
		return match(first + 1, second + 1);

	if (*first == '*')
		return match(first + 1, second) || match(first, second + 1);
	return false;
}

vector<string> printRecommend_create(const string s, char ch[62], array<map<string, Account>, 101>& ID){
	//if (outputNum >= 10) return outputNum;
	//else {
	vector<string> vi;
		string tmp = s;
		for (unsigned i = 0; i < s.size(); i++){
			if (tmp[i] == '*') {
				for (unsigned k = 0; k < 62; k++){
					tmp[i] = ch[k];
					vector<string> vv;
					vv = printRecommend_create(tmp, ch, ID);
					for (unsigned j = 0; j < vv.size(); j++){
						vi.push_back(vv[j]);
					}
				}
			}
		}
		auto it = ID[tmp.size()].find(tmp);
		if (it == ID[tmp.size()].end()){
			vi.push_back(tmp);
			//outputNum++;
		//}
		//return outputNum;
	}
		return vi;

}

int score(const string a, const string b){
	int diff = (a.size() > b.size()) ?  a.size()-b.size(): b.size()-a.size();
	int one=0;
	for (int i = 1; i <= diff; i++){
		one += i;
	}
	int two=0;
	int L = min(a.size(), b.size());
	for (int j = 0; j < L; j++){
		if (a[j] != b[j]){
			two += (L-j);
		}
	}
	return (one + two);
}

bool trsCmp(pair<string,int>a, pair<string,int> b){
	if (a.second == b.second){
		return a.first < b.first;
	}
	return a.second < b.second;
}


int main(){

	array<map<string, Account>, 101> ID;
	int timeCount = 0;
	string line, word;
	string tempID;
	char Char[62] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
	int mum[100] = { 0 }; //紀錄長度部分的s可能score值（1, 3, 6, 10.....)
	for (int i = 1; i<100; i++){//這部分可以放在main 一開始
		mum[i] = (1 + i)*(i)*0.5;
	}
	int lineNum = 0;
	while (getline(cin, line)){
		istringstream command(line);
		command >> word;
		//lineNum++; if (lineNum>200000)break;
		// function diverge
		if (word == "login"){
			string id, pswd;
			command >> id >> pswd;
			int size = id.size();
			std::map<string, Account>::iterator it = ID[size].find(id);
			if (it == ID[id.size()].end()){ cout << "ID " << id << " not found" << endl; }
			else if ( (*it).second.password ==  md5( pswd )){
				tempID = id;
				cout << "success" << endl;
			}
			else { cout << "wrong password" << endl; }

		}
		else if (word == "create"){
			string id, pswd;
			command >> id >> pswd;
			int size = id.size();
			auto it = ID[size].find(id);
			if (it != ID[size].end()){ cout << "ID " << id << " exists, ";
				
			//
			int output_num = 0;//紀錄output 數量
			int len = id.length();
			int max_score = (1 + 100 - len)*(100 - len)*0.5 + (1 + len)*len*0.5;
			vector<string> outputed;
			for (int score = 1; score <= max_score; score++){//一個一個score去找可能的組合 每一個分為兩項 （長度）＋（char差異）
				vector<string> combination;///////最重要部分！！！
				////用來儲存當前score 所有可能組合 ex id=abc score=1 則組合為 {ab, abc*, ab*};
				////我這裡先用vector 你們可以自己決定想要的容器 但要在 有 註明////放入combination 地方進行更改
				int max_first_part_pos = 0;//知道（長度）部分最大到多少
				for (int k = 0; k <= 99; k++){
					if (mum[k]>score){
						max_first_part_pos = k - 1;
						break;
					}
				}
				for (int k = 0; k <= max_first_part_pos; k++){//根據（長度）部分不同數值 去找 對應（char差異）可能數值
					int first_part = mum[k];
					int second_part = score - first_part;
					bool canShort = false;
					if (k != 0 && k<len) canShort = true;
					vector<pair<int, int*>> record;
					int* me = new int[2];
					me[1] = second_part;
					record.push_back({ 1, me });
					int * a = new int[second_part];
					integerPartition(second_part, a, 0, record);//用來找整數加發分解存在record
					for (size_t s = 0; s<record.size(); s++){
						//加長
						string model = id;
						for (int i = 1; i <= k; i++){
							model.push_back('*');
						}
						if (record[s].second[record[s].first] <= len){
							for (int i = 1; i <= record[s].first; i++){
								model[len - record[s].second[i]] = '*';
							}
							combination.push_back(model);/////////////////放入combination
						}
						//縮短
						if (canShort){
							model = id;
							for (int i = 1; i <= k; i++){
								model.pop_back();
							}
							int short_len = len - k;
							if (record[s].second[record[s].first] <= short_len){
								for (int i = 1; i <= record[s].first; i++){
									model[short_len - record[s].second[i]] = '*';
								}
								combination.push_back(model);//////////////放入combination
							}

						}

					}
					delete[]a;
					for (size_t s = 0; s<record.size(); s++){//此部分可以偷偷刪掉 只是就是會浪費記憶體空間 但上傳因為空間大為省時間可以註解掉～～
						delete[]record[s].second;
					}
					record.clear();
				}//evey first part
				sort(combination.begin(), combination.end());
				vector<string> result;
				for (unsigned u = 0; u < combination.size(); u++){
					vector<string> vecFor = printRecommend_create(combination[u], Char, ID);
					for (unsigned z = 0; z < vecFor.size(); z++){
						result.push_back(vecFor[z]);
					}
					/*int cnt = 0;
					for (unsigned g = 0; g < combination[u].size();g++){
						if (combination[u][g] == '*') { cnt++; }
					}
					if (cnt >0){   }
					
					cout <<","<< combination[u] ;
					output_num++;
					*/
				}
				sort(result.begin(), result.end());
				auto e = unique(result.begin(), result.end());
				result.erase(e, result.end());
				
				for (unsigned y = 0; y < result.size(); y++){
					bool checkOK = true;
					for (unsigned z = 0; z < outputed.size(); z++){ if (outputed[z] == result[y]) { checkOK = false; break; } }
					if (checkOK){
						cout << result[y];
						output_num++;
						outputed.push_back(result[y]);
					}
					if (output_num >= 10) break;
					cout << ",";
				}
				
				///////////////// 此部分可以由你們自己決定如何根據可能組合 依字典順序＋是否此id已經創建 輸出10個可能id 若已經十個 就直接結束 不足進行下一個score ////////////////////////
							
				combination.clear();
				if (output_num >= 10) {
					cout << endl; break;
				}

			}//one score end

			//

			}
			else {
				ID[size][id].Id = id;
				ID[size][id].password = md5(pswd);
				ID[size][id].createTime = timeCount;
				timeCount++;
				cout << "success" << endl;
			}
		}
		else if (word == "delete"){
			string id, pswd;
			command >> id >> pswd;
			int size = id.size();
			auto it = ID[size].find(id);
			if (it == ID[size].end()){ cout << "ID " << id << " not found" << endl; }
			else if ((*it).second.password == md5(pswd) ){ 
				for (unsigned i = 0; i < (*it).second.record.size(); i++){
					if ((*it).second.record[i].history[1] != id){
						auto itt = ID[(*it).second.record[i].history[1].size()].find((*it).second.record[i].history[1]);
						if (itt != ID[(*it).second.record[i].history[1].size()].end()){
							for (unsigned j = 0; j < (*itt).second.record.size(); j++){
								if ((*itt).second.record[j].history[1] == id){ (*itt).second.record[j].dead = true; }
							}
						}
					}
				}
				
				
				ID[size].erase(it);
				cout << "success" << endl; 
			}
			else {
				cout << "wrong password" << endl;
			}

		}
		else if (word == "merge"){
			string id1, pswd1, id2, pswd2;
			command >> id1 >> pswd1 >> id2 >> pswd2;
			auto it1 = ID[id1.size()].find(id1);
			auto it2 = ID[id2.size()].find(id2);
			if (it1 == ID[id1.size()].end()){ cout << "ID " << id1 << " not found" << endl; }
			else if (it2 == ID[id2.size()].end()){ cout << "ID " << id2 << " not found" << endl; }
			else if ((*it1).second.password != md5(pswd1) ){
				cout << "wrong password1" << endl;
			}
			else if ((*it2).second.password != md5(pswd2)){
				cout << "wrong password2" << endl;
			}
			else {
				(*it1).second.money += (*it2).second.money;
				/*for (unsigned i = 0; i < (*it1).second.record.size(); i++){
					if ((*it1).second.record[i].history[1] == id2 && (*it1).second.record[i].time > (*it2).second.createTime){
						(*it1).second.record[i].history[1] = id1;
					}
				}*/
				for (unsigned i = 0; i < (*it2).second.record.size(); i++){
					if ((*it2).second.record[i].history[1] == id2 && !(*it2).second.record[i].dead ){
						(*it2).second.record[i].history[1] = id1;
					}
					else if ((*it2).second.record[i].history[1] != id2){
						auto it3 = ID[(*it2).second.record[i].history[1].size()].find((*it2).second.record[i].history[1]);
						if (it3 != ID[(*it2).second.record[i].history[1].size()].end() ){
							for (unsigned j = 0; j < (*it3).second.record.size(); j++){
								if ((*it3).second.record[j].history[1] == id2 && !(*it3).second.record[j].dead){
									(*it3).second.record[j].history[1] = id1;
								}
							}
						}
					}
					else {}
					(*it1).second.record.push_back((*it2).second.record[i]);
				}
				stable_sort((*it1).second.record.begin(), (*it1).second.record.end(), transferCmp);
				ID[id2.size()].erase(it2);
	  	                cout << "success, " << id1 << " has " << (*it1).second.money << " dollars" << endl;
				
			}
		}
		//last successful login
		else if (word == "deposit"){
			int dep;
			command >> dep;
			ID[tempID.size()][tempID].money += dep;
			cout << "success, " << ID[tempID.size()][tempID].money << " dollars in current account" << endl;
		}
		else if (word == "withdraw"){
			int wit;
			command >> wit;
			if (ID[tempID.size()][tempID].money < wit){
				cout << "fail, " << ID[tempID.size()][tempID].money << " dollars only in current account" << endl;
			}
			else {
				ID[tempID.size()][tempID].money -= wit;
				cout << "success, " << ID[tempID.size()][tempID].money << " dollars left in current account" << endl;
			}
		}
		else if (word == "transfer"){
			string id;
			int num;
			command >> id >> num;
			auto it = ID[id.size()].find(id);
			auto itt = ID[tempID.size()].find(tempID);
			if (it == ID[id.size()].end()){ cout << "ID " << id << " not found, "; 
			
				vector<pair<string,int>> vs;
				for (unsigned i = 0; i <= 100; i++){
					auto it = ID[i].begin();
					auto ite = ID[i].end();
					while (it != ite){
						string tmp = it->first;
						int scr = score(id, tmp);
						pair<string, int> a = { tmp, scr };
						vs.push_back(a);
						it++;
					}
				}
				sort(vs.begin(), vs.end(), trsCmp);
				for (unsigned j = 0; j < vs.size(); j++){
					cout << vs[j].first;
					if (j >= 9 || j==vs.size()-1) break;
					cout << ",";
				}
				cout << endl;
			
			}
			else if ((*itt).second.money < num){ cout << "fail, " << (*itt).second.money << " dollars only in current account" << endl; }
			else {
				(*itt).second.money -= num;
				(*it).second.money += num;
				Transfer t1,t2;
				t1.number = num;
				t2.number = num;
				t1.time = timeCount;
				t2.time = timeCount;
				timeCount++;
				t2.history[0] = "From";
				t2.history[1] = tempID;
				(*it).second.record.push_back(t2);
				t1.history[0] = "To";
				t1.history[1] = id;
				(*itt).second.record.push_back(t1);
				cout << "success, " << (*itt).second.money << " dollars left in current account" << endl;
			}
		}
		else if (word == "find"){
			string wildcardID;
			command >> wildcardID;
			vector<string> vs;
			const char* a1 = wildcardID.c_str();
			for (unsigned i = 1; i <= 100; i++){
				auto it = ID[i].begin(); 
				auto itend = ID[i].end();
				while (it != itend){
					const char* a2 = (*it).second.Id.c_str();
					if (match(a1, a2)){
						vs.push_back((*it).second.Id);
					}
					it++;
				}
			}
			sort(vs.begin(), vs.end() , wildcmp);	
			if (vs.size()> 0){
				for (unsigned i = 0; i < vs.size(); i++){
					//if (vs[i] != tempID){
						if (i != vs.size() - 1){
							cout << vs[i] << ",";
						}
						else {
							cout << vs[i];
						}
					//}
				}
			}
			cout << endl;


		}
		else if (word == "search"){
			string id;
			command >> id;
			int count = 0;
			auto it = ID[tempID.size()].find(tempID);
			for (unsigned i = 0; i < (*it).second.record.size(); i++){
				if ((*it).second.record[i].history[1] == id){
					count++;
					cout << (*it).second.record[i].history[0] << " " <<
						(*it).second.record[i].history[1] << " " <<
						(*it).second.record[i].number << endl;
				}
			}
			if (count == 0){
				cout << "no record" << endl;
			}
		}
	
	
	
	}


	return 0;
}
