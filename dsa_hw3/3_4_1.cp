#include <iostream> 
#include <string>
#include <stack>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <stdio.h>
using namespace std;

int prec(string ss){
	if (ss == "+") return 4;
	else if (ss == "-") return 4;
	else if (ss == "plus") return 2;
	else if (ss == "minus") return 2;
	else if (ss == "!") return 2;
	else if (ss == "~") return 2;
	else if (ss == ">>") return 5;
	else if (ss == "<<") return 5;
	else if (ss == "&") return 8;
	else if (ss == "^") return 9;
	else if (ss == "|") return 10;
	else if (ss == "&&") return 11;
	else if (ss == "||") return 12;
	else if (ss == "(") return 1;
	else if (ss == "*") return 3;
	else if (ss == "/") return 3;
	else if (ss == "%") return 3;
}




int main(){

	string tmpline;
	while (getline(cin,tmpline)){
		stack<int> storeInt;
		stack<string> storeOperator;
		string strToInt;
		vector<string> vs;
		string line; 
		for (auto e : tmpline){
			if (!iswspace(e)){
				line.push_back(e);
			}
		}
		
		for (int i = 0; i < line.size(); i++){

			if (isdigit(line[i])){
				strToInt.push_back(line[i]);
				if (!isdigit(line[i + 1])){
					vs.push_back(strToInt);
					strToInt = "";
				}
			}

			else if (line[i] == '(') {
				storeOperator.push("(");
			}
			else if (line[i] == ')'){
				while (storeOperator.top() != "("){
					string oper = storeOperator.top();
					vs.push_back(oper);
					storeOperator.pop();
				}
				storeOperator.pop();
			}

			else if (line[i] == '+') {
				if (i == 0){
					storeOperator.push("plus");
				}
				else if (!isdigit(line[i - 1]) && line[i - 1] != ')'){
					if (storeOperator.empty()){
						storeOperator.push("plus");
					}
					else {
						string top = storeOperator.top();
						while (!storeOperator.empty() && prec(top) < prec("plus") && storeOperator.top() != "("){
							vs.push_back(top);
							storeOperator.pop();
							if (!storeOperator.empty()) top = storeOperator.top();
						}
						storeOperator.push("plus");
					}
				}
				else  {
					if (storeOperator.empty()){
						storeOperator.push("+");
					}
					else{
						string top = storeOperator.top();
						while (!storeOperator.empty() && prec(top) <= prec("+") && storeOperator.top() != "("){
							vs.push_back(top);
							storeOperator.pop();
							if (!storeOperator.empty()) top = storeOperator.top();
						}
						storeOperator.push("+");
					}

				}
			}

			else if (line[i] == '-') {
				if (i == 0){
					storeOperator.push("minus");
				}
				else if (!isdigit(line[i - 1]) && line[i - 1] != ')'){
					if (storeOperator.empty()){
						storeOperator.push("minus");
					}
					else {
						string top = storeOperator.top();
						while (!storeOperator.empty() && prec(top) < prec("minus") && storeOperator.top() != "("){
							vs.push_back(top);
							storeOperator.pop();
							if (!storeOperator.empty()) top = storeOperator.top();
						}
						storeOperator.push("minus");
					}
				}
				else  {
					if (storeOperator.empty()){
						storeOperator.push("-");
					}
					else{
						string top = storeOperator.top();
						while (!storeOperator.empty() && prec(top) <= prec("-") && storeOperator.top() != "("){
							vs.push_back(top);
							storeOperator.pop();
							if (!storeOperator.empty()) top = storeOperator.top();
						}
						storeOperator.push("-");
					}

				}
			}

			else if (line[i] == '~' || line[i] == '!'){
				if (line[i] == '~') {
					if (storeOperator.empty()){
						storeOperator.push("~");
					}
					else {
						string top = storeOperator.top();
						while (!storeOperator.empty() && prec(top) < prec("~") && storeOperator.top() != "("){
							vs.push_back(top);
							storeOperator.pop();
							if (!storeOperator.empty()) top = storeOperator.top();
						}
						storeOperator.push("~");
					}
				}
				else {
					if (storeOperator.empty()){
						storeOperator.push("!");
					}
					else {
						string top = storeOperator.top();
						while (!storeOperator.empty() && prec(top) < prec("!") && storeOperator.top() != "("){
							vs.push_back(top);
							storeOperator.pop();
							if (!storeOperator.empty()) top = storeOperator.top();
						}
						storeOperator.push("!");
					}
				}
			}

			else if (line[i] == '<' || line[i] == '>'){
				if (storeOperator.empty()){
					if (line[i] == '<'){
						string tmpop = "<<";
						storeOperator.push(tmpop);
					}
					else{
						string tmpop = ">>";
						storeOperator.push(tmpop);
					}
					i++;
				}
				else{
					string top = storeOperator.top();
					while (!storeOperator.empty() && prec(top) <= prec("<<") && storeOperator.top() != "("){
						vs.push_back(top);
						storeOperator.pop();
						if (!storeOperator.empty()) top = storeOperator.top();
					}
					if (line[i] == '<') storeOperator.push("<<");
					else if (line[i] == '>') storeOperator.push(">>");
					i++;
				}
			}


			else if (line[i] == '*' || line[i] == '/' || line[i] == '%'){
				if (storeOperator.empty()){
					if (line[i] == '*') storeOperator.push("*");
					else if (line[i] == '/') storeOperator.push("/");
					else if (line[i] == '%') storeOperator.push("%");
				}
				else {
					string top = storeOperator.top();
					while (!storeOperator.empty() && prec(top) <= prec("*") && storeOperator.top() != "("){
						vs.push_back(top);
						storeOperator.pop();
						if (!storeOperator.empty()) top = storeOperator.top();
					}
					if (line[i] == '*') storeOperator.push("*");
					else if (line[i] == '/') storeOperator.push("/");
					else if (line[i] == '%') storeOperator.push("%");
				}


			}
			else if (line[i] == '^'){
				if (storeOperator.empty()){
					storeOperator.push("^");
				}
				else {
					string top = storeOperator.top();
					while (!storeOperator.empty() && prec(top) <= prec("^") && storeOperator.top() != "("){
						vs.push_back(top);
						storeOperator.pop();
						if (!storeOperator.empty()) top = storeOperator.top();
					}
					storeOperator.push("^");
				}
			}

			else if (line[i] == '&'){
				if (line[i + 1] == '&'){
					if (storeOperator.empty()){
						storeOperator.push("&&");
					}
					else {
						string top = storeOperator.top();
						while (!storeOperator.empty() && prec(top) <= prec("&&") && storeOperator.top() != "("){
							vs.push_back(top);
							storeOperator.pop();
							if (!storeOperator.empty()) top = storeOperator.top();
						}
						storeOperator.push("&&");
					}
					i++;
				}
				else{
					if (storeOperator.empty()){
						storeOperator.push("&");
					}
					else {
						string top = storeOperator.top();
						while (!storeOperator.empty() && prec(top) <= prec("&") && storeOperator.top() != "("){
							vs.push_back(top);
							storeOperator.pop();
							if (!storeOperator.empty()) top = storeOperator.top();
						}
						storeOperator.push("&");
					}
				}
			}

			else if (line[i] == '|'){
				if (line[i + 1] == '|'){
					if (storeOperator.empty()){
						storeOperator.push("||");
					}
					else {
						string top = storeOperator.top();
						while (!storeOperator.empty() && prec(top) <= prec("||") && storeOperator.top() != "("){
							vs.push_back(top);
							storeOperator.pop();
							if (!storeOperator.empty()) top = storeOperator.top();
						}
						storeOperator.push("||");
					}
					i++;
				}
				else{
					if (storeOperator.empty()){
						storeOperator.push("|");
					}
					else {
						string top = storeOperator.top();
						while (!storeOperator.empty() && prec(top) <= prec("|") && storeOperator.top() != "("){
							vs.push_back(top);
							storeOperator.pop();
							if (!storeOperator.empty()) top = storeOperator.top();
						}
						storeOperator.push("|");
					}
				}
			}

		}

		while (!storeOperator.empty()){
			vs.push_back(storeOperator.top());
			storeOperator.pop();
		}

		cout << "Postfix Exp: ";
		for (int i = 0; i < vs.size(); i++){
			if (isdigit(vs[i][0])){
				int toInt = atoi(vs[i].c_str());
				cout << toInt << " ";
			}
			else if (vs[i] == "plus"){
				cout << "+" << " ";
			}
			else if (vs[i] == "minus"){
				cout << "-" << " ";
			}
			else cout << vs[i] << " ";
		}

		for (int i = 0; i < vs.size(); i++){
			if (isdigit(vs[i][0])){
				int toInt = atoi(vs[i].c_str());
				storeInt.push(toInt);
			}
			else if (vs[i] == "plus"){
				int tmp = storeInt.top() * 1;
				storeInt.pop();
				storeInt.push(tmp);
			}
			else if (vs[i] == "minus"){
				int tmp = storeInt.top() * (-1);
				storeInt.pop();
				storeInt.push(tmp);
			}
			else if (vs[i] == "!"){
				int tmp = !storeInt.top();
				storeInt.pop();
				storeInt.push(tmp);
			}
			else if (vs[i] == "~"){
				int tmp = ~storeInt.top();
				storeInt.pop();
				storeInt.push(tmp);
			}
			else if (vs[i] == "+"){
				int a = storeInt.top();
				storeInt.pop();
				int b = storeInt.top();
				storeInt.pop();
				int tmp = a + b;
				storeInt.push(tmp);
			}
			else if (vs[i] == "-"){
				int a = storeInt.top();
				storeInt.pop();
				int b = storeInt.top();
				storeInt.pop();
				int tmp = b - a;
				storeInt.push(tmp);
			}
			else if (vs[i] == "/"){
				int a = storeInt.top();
				storeInt.pop();
				int b = storeInt.top();
				storeInt.pop();
				int tmp = b / a;
				storeInt.push(tmp);
			}
			else if (vs[i] == "%"){
				int a = storeInt.top();
				storeInt.pop();
				int b = storeInt.top();
				storeInt.pop();
				int tmp = b % a;
				storeInt.push(tmp);
			}
			else if (vs[i] == "*"){
				int a = storeInt.top();
				storeInt.pop();
				int b = storeInt.top();
				storeInt.pop();
				int tmp = b * a;
				storeInt.push(tmp);
			}
			else if (vs[i] == "&&"){
				int a = storeInt.top();
				storeInt.pop();
				int b = storeInt.top();
				storeInt.pop();
				int tmp = b && a;
				storeInt.push(tmp);
			}
			else if (vs[i] == "^"){
				int a = storeInt.top();
				storeInt.pop();
				int b = storeInt.top();
				storeInt.pop();
				int tmp = b ^ a;
				storeInt.push(tmp);
			}
			else if (vs[i] == "||"){
				int a = storeInt.top();
				storeInt.pop();
				int b = storeInt.top();
				storeInt.pop();
				int tmp = b || a;
				storeInt.push(tmp);
			}
			else if (vs[i] == "&"){
				int a = storeInt.top();
				storeInt.pop();
				int b = storeInt.top();
				storeInt.pop();
				int tmp = b & a;
				storeInt.push(tmp);
			}
			else if (vs[i] == "|"){
				int a = storeInt.top();
				storeInt.pop();
				int b = storeInt.top();
				storeInt.pop();
				int tmp = b | a;
				storeInt.push(tmp);
			}
			else if (vs[i] == "<<"){
				int a = storeInt.top();
				storeInt.pop();
				int b = storeInt.top();
				storeInt.pop();
				int tmp = b << a;
				storeInt.push(tmp);
			}
			else if (vs[i] == ">>"){
				int a = storeInt.top();
				storeInt.pop();
				int b = storeInt.top();
				storeInt.pop();
				int tmp = b >> a;
				storeInt.push(tmp);
			}
		}

		cout << "\nRESULT: " << storeInt.top() << endl;

	}

	return 0;
}