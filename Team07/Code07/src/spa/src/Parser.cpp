#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>

//using namespace std;


int Parse (std::string file) {
	std::ifstream t(file);
	std::string str((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());
	std::cout << str <<"\n\n\n\n";
	return 0;
}