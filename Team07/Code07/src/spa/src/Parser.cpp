#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <set>
#include "ParserLib.h"
#include "ParserSimple.h"
#include "TNode.h"
#include <iostream>

// core functions -------------------------------------------------------------

int Parse (std::string file) {
	std::ifstream t(file);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	
	State s(&str);
	TNode ast;
	try{
		ast = program(s);
	} catch (ParseException &e) {
		s.excps.push_back(e);
		std::cerr << prettyPrintException(s) << "\n";
	}
	int valPhase = 0;
	try {
		std::set<std::string> procs;
		validateUniqueProcedureNames(ast, procs);
		valPhase = 1;
		validateCallProcedureExists(ast, procs);
		std::cout << ast.toSexp() << "\n";
	} catch (int &valRes) {
		std::cerr << prettyPrintValidation(&str, valRes, valPhase == 0 ? "repeated procedure definition" : "call on undefined procedure");
	}
	return 0;
}