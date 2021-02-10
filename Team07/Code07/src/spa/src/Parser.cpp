#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include "ParserLib.h"

using namespace std;


// core functions -------------------------------------------------------------
void helloWorld(State &s);

int Parse (string file) {
	ifstream t(file);
	string str((istreambuf_iterator<char>(t)),
                 istreambuf_iterator<char>());
	cout << str <<"\n\n\n\n";

	// TESTING PARSE hello world :
	State s("hello world asd");
	try{
		helloWorld(s);
	} catch (ParseException &e) {
		cout << e.what() << "\n";
	}
	cout << s.toString() << "\n";
	// ------
	return 0;
}

void helloWorld(State &s) {
	State so(s);
	try {
		stringMatch(s, "hello");
		whitespace(s);
		stringMatch(s, "world");
	} catch (ParseException &e) {
		throw ParseException(so.i, s.i, "helloWorld", "", &e);
	}
}