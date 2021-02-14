#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include "ParserLib.h"

using namespace std;


// core functions -------------------------------------------------------------
void helloOrHihi(State &s);

int Parse (string file) {
	ifstream t(file);
	string str((istreambuf_iterator<char>(t)),
                 istreambuf_iterator<char>());
	//cout << str <<"\n\n\n\n";
	
	// TESTING PARSE hello world :
	string str1 = "hello worlasd";
	State s(&str1);
	
	try{
		helloOrHihi(s);
	} catch (ParseException &e) {
		s.excps.push_back(e);
		cout << prettyPrintException(s) << "\n";
	}
	cout << s.toString() << "\n";
	// ------
	return 0;
}

void hi(State &s) {
	State so(s);
	try {
		stringMatch(s, "hi");
	} catch(ParseException &e) {
		s.excps.push_back(e);
		throw ParseException(so.i, s.i, "hi", "");
	}
}

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
}

void helloWorld(State &s) {
	State so(s);
	try {
		stringMatch(s, "hello");
		whitespace(s);
		stringMatch(s, "world");
	} catch (ParseException &e) {
		s.excps.push_back(e);
		throw ParseException(so.i, s.i, "helloWorld", "");
	}
}

void helloOrHihi(State &s) {
	State so(s);
	try {
		repeatHi(s);
	} catch(ParseException &e1) {
		try {
			s.assign(so);
			helloWorld(s);
		} catch(ParseException &e2) {
			s.excps.push_back(e2);
			throw ParseException(so.i, s.i, "helloOrHihi", "");
		}
	}
}