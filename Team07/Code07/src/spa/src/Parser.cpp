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
	string str1 = "1 * 2 / 3";
	State s(&str1);
	
	try{
		term(s);
	} catch (ParseException &e) {
		s.excps.push_back(e);
		cout << prettyPrintException(s) << "\n";
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