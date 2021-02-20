#include "ParserLib.h"
#include "ParserPql.h"
#include "ParserSimple.h"
#include "QueryMap.h"
#include "TNode.h"
#include "ParserIndexMapper.h"
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
	ParserMapper map(&str);
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
			throw prettyPrintValidation(map, valRes, valPhase == 0 ? "repeated procedure definition" : "call on undefined procedure");
		}
		// validate
	} catch (ParseException &e) {
		s.excps.push_back(e);
		throw prettyPrintException(map, s);
	}
}

QueryMap parse_pql(std::string query) {
  State state(&query);
	ParserMapper map(&query);
	try{
    QueryMap parsed_query_map = pql_query(state);
    return pql_validate(parsed_query_map);
	} catch (ParseException &e) {
		state.excps.push_back(e);
		throw prettyPrintException(map, state);
	}
}
