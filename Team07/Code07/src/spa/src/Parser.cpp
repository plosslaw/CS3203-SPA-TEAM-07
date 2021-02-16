#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include "ParserLib.h"
#include "ParserSimple.h"

// core functions -------------------------------------------------------------

int Parse (string file) {
	ifstream t(file);
	std::string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
	
	State s(&str);
	try{
		cout << program(s).toSexp() << "\n";
	} catch (ParseException &e) {
		s.excps.push_back(e);
		cerr << prettyPrintException(s) << "\n";
	}
	return 0;
}