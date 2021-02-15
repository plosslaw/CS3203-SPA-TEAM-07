#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include "ParserLib.h"
#include "ParserSimple.h"

using namespace std;


// core functions -------------------------------------------------------------

int Parse (string file) {
	ifstream t(file);
	string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
	
	State s(&str);
	try{
		cout << program(s).toSexp() << "\n";
	} catch (ParseException &e) {
		s.excps.push_back(e);
		cerr << prettyPrintException(s) << "\n";
	}
	cout << s.toString() << "\n";
	//TODO figure out error case
	return 0;
}