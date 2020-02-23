#include <iostream>
#include <string>
#include <math.h>
#include <stack>
#include <cstdlib>
#include <vector>
#include <iomanip>
using namespace std;

int prec(string ss){
	if (ss == "(") return 1;
	else if (ss == "plus") return 2;
	else if (ss == "minus") return 2;
	else if (ss == "*") return 3;
	else if (ss == "+") return 4;
	else if (ss == "-") return 4;
	else if (ss == "sin") return 1;
	else if (ss == "cos") return 1;
	else if (ss == "exp") return 1;
	else if (ss == "log") return 1;
	else if (ss == "pow") return 1;
	else if (ss == "sqrt") return 1;
	else if (ss == "fabs") return 1;

}



int main(){

	string line;
	while (getline(cin, line)){
		vector<string> postfix;
		stack<string> storeOperator;
		stack<double> storeDecimal;


		for (int i = 0; i < line.size(); i++){

			// whitespace
			if (iswspace(line[i])){}

			//decimal
			else if (isdigit(line[i]) || line[i] == '.' ){
				string number;
				number.push_back(line[i]);
				int j = i + 1;
				while (isdigit(line[j]) || line[j] == '.'){
					number.push_back(line[j]);
					j++;
					i++;
				}
				postfix.push_back(number);
			}

			// parentheses
			else if (line[i] == '(') {
				storeOperator.push("(");
			}
			else if (line[i] == ')'){
				while (storeOperator.top() != "("){
					string oper = storeOperator.top();
					postfix.push_back(oper);
					storeOperator.pop();
				}
				storeOperator.pop();
			}

			// minus or suctraction
			else if (line[i] == '-'){
				if (i == 0){
					storeOperator.push("minus");
				}
				else if (line[i + 1] != '-'){
					int j = i-1;
					while (iswspace(line[j]) && j >0){
						j--;
					}
					if (!isdigit(line[j]) && line[j] != ')'){
						if (storeOperator.empty()){
							storeOperator.push("minus");
						}
						else {
							string top = storeOperator.top();
							while (!storeOperator.empty() && prec(top) < prec("minus") && storeOperator.top() != "("){
								postfix.push_back(top);
								storeOperator.pop();
								if (!storeOperator.empty()) top = storeOperator.top();
							}
							storeOperator.push("minus");
						}	
					}
					else {
						if (storeOperator.empty()){
							storeOperator.push("-");
						}
						else{
							string top = storeOperator.top();
							while (!storeOperator.empty() && prec(top) <= prec("-") && storeOperator.top() != "("){
								postfix.push_back(top);
								storeOperator.pop();
								if (!storeOperator.empty()) top = storeOperator.top();
							}
							storeOperator.push("-");
						}
					}
				}
				else{
					if (storeOperator.empty()){
						storeOperator.push("-");
					}
					else{
						string top = storeOperator.top();
						while (!storeOperator.empty() && prec(top) <= prec("-") && storeOperator.top() != "("){
							postfix.push_back(top);
							storeOperator.pop();
							if (!storeOperator.empty()) top = storeOperator.top();
						}
						storeOperator.push("-");
					}
				}
			}

			// plus or addition
			else if (line[i] == '+'){
				if (i == 0){
					storeOperator.push("plus");
				}
				else if (line[i + 1] != '+'){
					int j = i - 1;
					while (iswspace(line[j]) && j >0){
						j--;
					}
					if (!isdigit(line[j]) && line[j] != ')'){
						if (storeOperator.empty()){
							storeOperator.push("plus");
						}
						else {
							string top = storeOperator.top();
							while (!storeOperator.empty() && prec(top) < prec("plus") && storeOperator.top() != "("){
								postfix.push_back(top);
								storeOperator.pop();
								if (!storeOperator.empty()) top = storeOperator.top();
							}
							storeOperator.push("plus");
						}
					}
					else {
						if (storeOperator.empty()){
							storeOperator.push("+");
						}
						else{
							string top = storeOperator.top();
							while (!storeOperator.empty() && prec(top) <= prec("+") && storeOperator.top() != "("){
								postfix.push_back(top);
								storeOperator.pop();
								if (!storeOperator.empty()) top = storeOperator.top();
							}
							storeOperator.push("+");
						}
					}
				}
				else{
					if (storeOperator.empty()){
						storeOperator.push("+");
					}
					else{
						string top = storeOperator.top();
						while (!storeOperator.empty() && prec(top) <= prec("+") && storeOperator.top() != "("){
							postfix.push_back(top);
							storeOperator.pop();
							if (!storeOperator.empty()) top = storeOperator.top();
						}
						storeOperator.push("+");
					}
				}
			}

			// multiplicative
			else if (line[i] == '*'){
				if (storeOperator.empty()){
					storeOperator.push("*");
				}
				else{
					string top = storeOperator.top();
					while (!storeOperator.empty() && prec(top) <= prec("*") && storeOperator.top() != "("){
						postfix.push_back(top);
						storeOperator.pop();
						if (!storeOperator.empty()) top = storeOperator.top();
					}
					storeOperator.push("*");
				}
			}

			// function sqrt
			else if (line[i] == 's' && line[i + 1] == 'q'){
				if (storeOperator.empty()){
					storeOperator.push("sqrt");
				}
				else{
					string top = storeOperator.top();
					while (!storeOperator.empty() && prec(top) <= prec("sqrt") && storeOperator.top() != "("){
						postfix.push_back(top);
						storeOperator.pop();
						if (!storeOperator.empty()) top = storeOperator.top();
					}
					storeOperator.push("sqrt");
				}
				i += 3;
			}

			// function log
			else if (line[i] == 'l' && line[i + 1] == 'o'){
				if (storeOperator.empty()){
					storeOperator.push("log");
				}
				else{
					string top = storeOperator.top();
					while (!storeOperator.empty() && prec(top) <= prec("log") && storeOperator.top() != "("){
						postfix.push_back(top);
						storeOperator.pop();
						if (!storeOperator.empty()) top = storeOperator.top();
					}
					storeOperator.push("log");
				}
				i += 2;
			}

			// function exponential
			else if (line[i] == 'e' && line[i + 1] == 'x'){
				if (storeOperator.empty()){
					storeOperator.push("exp");
				}
				else{
					string top = storeOperator.top();
					while (!storeOperator.empty() && prec(top) <= prec("exp") && storeOperator.top() != "("){
						postfix.push_back(top);
						storeOperator.pop();
						if (!storeOperator.empty()) top = storeOperator.top();
					}
					storeOperator.push("exp");
				}
				i += 2;
			}

			// function power
			else if (line[i] == 'p' && line[i + 1] == 'o'){
				if (storeOperator.empty()){
					storeOperator.push("pow");
				}
				else{
					string top = storeOperator.top();
					while (!storeOperator.empty() && prec(top) <= prec("pow") && storeOperator.top() != "("){
						postfix.push_back(top);
						storeOperator.pop();
						if (!storeOperator.empty()) top = storeOperator.top();
					}
					storeOperator.push("pow");
				}
				i += 2;
			}

			// function sin
			else if (line[i] == 's' && line[i + 1] == 'i'){
				if (storeOperator.empty()){
					storeOperator.push("sin");
				}
				else{
					string top = storeOperator.top();
					while (!storeOperator.empty() && prec(top) <= prec("sin") && storeOperator.top() != "("){
						postfix.push_back(top);
						storeOperator.pop();
						if (!storeOperator.empty()) top = storeOperator.top();
					}
					storeOperator.push("sin");
				}
				i += 2;
			}

			// function cos
			else if (line[i] == 'c' && line[i + 1] == 'o'){
				if (storeOperator.empty()){
					storeOperator.push("cos");
				}
				else{
					string top = storeOperator.top();
					while (!storeOperator.empty() && prec(top) <= prec("cos") && storeOperator.top() != "("){
						postfix.push_back(top);
						storeOperator.pop();
						if (!storeOperator.empty()) top = storeOperator.top();
					}
					storeOperator.push("cos");
				}
				i += 2;
			}

			// function fabs
			else if (line[i] == 'f' && line[i + 1] == 'a'){
				if (storeOperator.empty()){
					storeOperator.push("fabs");
				}
				else{
					string top = storeOperator.top();
					while (!storeOperator.empty() && prec(top) <= prec("fabs") && storeOperator.top() != "("){
						postfix.push_back(top);
						storeOperator.pop();
						if (!storeOperator.empty()) top = storeOperator.top();
					}
					storeOperator.push("fabs");
				}
				i += 3;
			}

		}

		while (!storeOperator.empty()){
			postfix.push_back(storeOperator.top());
			storeOperator.pop();
		}

		cout << "Postfix Exp: ";
		for (int i = 0; i < postfix.size(); i++){
			if (isdigit(postfix[i][0]) || postfix[i][0] == '.' ){
				double toInt = atof(postfix[i].c_str());
				cout << fixed << setprecision(6) <<toInt << " ";
			}
			else if (postfix[i] == "plus"){
				cout << "+" << " ";
			}
			else if (postfix[i] == "minus"){
				cout << "-" << " ";
			}
			else cout << postfix[i] << " ";
		}

		for (unsigned i = 0; i < postfix.size(); i++){
			if (isdigit(postfix[i][0]) || postfix[i][0] == '.'){
				double toInt = atof(postfix[i].c_str());
				storeDecimal.push(toInt);
			}
			else if (postfix[i] == "plus"){
				double tmp = storeDecimal.top() * 1;
				storeDecimal.pop();
				storeDecimal.push(tmp);
			}
			else if (postfix[i] == "minus"){
				double tmp = storeDecimal.top() * (-1);
				storeDecimal.pop();
				storeDecimal.push(tmp);
			}
			else if (postfix[i] == "sin"){
				double tmp = sin(storeDecimal.top() );
				storeDecimal.pop();
				storeDecimal.push(tmp);
			}
			else if (postfix[i] == "cos"){
				double tmp = cos(storeDecimal.top());
				storeDecimal.pop();
				storeDecimal.push(tmp);
			}
			else if (postfix[i] == "sqrt"){
				double tmp = sqrt(storeDecimal.top());
				storeDecimal.pop();
				storeDecimal.push(tmp);
			}
			else if (postfix[i] == "log"){
				double tmp = log(storeDecimal.top());
				storeDecimal.pop();
				storeDecimal.push(tmp);
			}
			else if (postfix[i] == "exp"){
				double tmp = exp(storeDecimal.top());
				storeDecimal.pop();
				storeDecimal.push(tmp);
			}
			else if (postfix[i] == "fabs"){
				double tmp = fabs(storeDecimal.top());
				storeDecimal.pop();
				storeDecimal.push(tmp);
			}
			else if (postfix[i] == "+"){
				double a = storeDecimal.top();
				storeDecimal.pop();
				double b = storeDecimal.top();
				storeDecimal.pop();
				double tmp = b + a;
				storeDecimal.push(tmp);
			}
			else if (postfix[i] == "-"){
				double a = storeDecimal.top();
				storeDecimal.pop();
				double b = storeDecimal.top();
				storeDecimal.pop();
				double tmp = b - a;
				storeDecimal.push(tmp);
			}
			else if (postfix[i] == "*"){
				double a = storeDecimal.top();
				storeDecimal.pop();
				double b = storeDecimal.top();
				storeDecimal.pop();
				double tmp = b * a;
				storeDecimal.push(tmp);
			}
			else if (postfix[i] == "pow"){
				double a = storeDecimal.top();
				storeDecimal.pop();
				double b = storeDecimal.top();
				storeDecimal.pop();
				double tmp = pow(b,a);
				storeDecimal.push(tmp);
			}
			
		}

		cout << "\nRESULT: " << fixed << setprecision(6) << storeDecimal.top() << endl;
	}

	return 0;
}