#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <set>
#include "ParserLib.h"
#include "ParserSimple.h"
#include "TNode.h"

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
	std::set<std::string> procs;
	int valRes = validateUniqueProcedureNames(ast, procs);
	if (valRes == -1) {
		std::cout << ast.toSexp() << "\n";
	} else{
		std::cerr << prettyPrintValidation(&str, valRes, "repeated procedure definition");
	}
	return 0;
}