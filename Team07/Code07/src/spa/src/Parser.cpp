#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>

//using namespace std;

#include "PKB.h"
#include "TNode.h"

template<typename T>
class ParseState {
		T a;
		int o;
		std::string* str;
	public:
		ParseState<T>(ParseState<T> &p) {
			str = p.str;
			o = p.o;
			a = p.a;
		}
		ParseState(std::string &s) {
			str = &s;
			o = 0;
		}
		char head() {
			return (*str).at(o++);
		}
		bool end() {
			return (*str).length() == o;
		}
};

int Parse (string file) {
	std::ifstream t(file);
	std::string str((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());
	ParseState<char> p(str);
	cout<<p.head()<<"...\n\n\n";
	cout<<str<<"\n\n\n\n";
	return 0;
}