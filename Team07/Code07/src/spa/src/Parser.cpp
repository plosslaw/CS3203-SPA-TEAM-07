#include <stdio.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <unordered_set>
#include "ParserLib.h"
#include "ParserSimple.h"
#include "TNode.h"

// core functions -------------------------------------------------------------

TNode Parse (std::string file) {
	std::ifstream t(file);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	
	State s(&str);
	try{
		TNode ast;
		ast = program(s);
		// parse
		int valPhase = 0;
		try {
			std::unordered_set<std::string> procs;
			validateUniqueProcedureNames(ast, procs);
			valPhase = 1;
			validateCallProcedureExists(ast, procs);
			return ast;
		} catch (int &valRes) {
			throw prettyPrintValidation(&str, valRes, valPhase == 0 ? "repeated procedure definition" : "call on undefined procedure");
		}
		// validate
	} catch (ParseException &e) {
		s.excps.push_back(e);
		throw prettyPrintException(s);
	}
}