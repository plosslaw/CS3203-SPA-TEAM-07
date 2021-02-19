#include "ParserLib.h"
#include "ParserPql.h"
#include "ParserSimple.h"
#include "QueryMap.h"
#include "TNode.h"
#include <fstream>
#include <stdio.h>
#include <streambuf>
#include <string>
#include <unordered_set>

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

QueryMap parse_pql(std::string query) {
  
  State state(&query);
  QueryMap query_map = pql_query(state);
  // TODO(zs): validate query
  return query_map;
}
