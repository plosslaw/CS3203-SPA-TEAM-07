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
	string str((istreambuf_iterator<char>(t)),
                 istreambuf_iterator<char>());
	//cout << str <<"\n\n\n\n";
	
	// TESTING PARSE hello world :
	string str1 = "7 - 24 / 8 * 4 + 6 >= 12";
	State s(&str1);
	
	try{
		cout << rel_expr(s).toSexp() << "\n";
		if(s.i != (*s.source).size()) {
			throw ParseException(s.i, s.i, "", "unconsumed characters");
		}
	} catch (ParseException &e) {
		s.excps.push_back(e);
		cerr << prettyPrintException(s) << "\n";
	}
	cout << s.toString() << "\n";
	// ------
	return 0;
}

/*
void repeatHi(State &s) {
	State so(s);
	bool consumed = false;
	try {
		while(true) {
			hi(s);
			so.assign(s);
			consumed = true;
		}
	} catch (ParseException &e){
		if(!consumed) {
			s.excps.push_back(e);
			throw ParseException(so.i, s.i, "repeatHi", "");
		}
		s.assign(so);
	}
}*/