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
		if(s.i != (*s.source).size()) {
			throw ParseException(s.i, s.i, "", "unconsumed characters");
		}
	} catch (ParseException &e) {
		s.excps.push_back(e);
		cerr << prettyPrintException(s) << "\n";
	}
	cout << s.toString() << "\n";
	return 0;
}