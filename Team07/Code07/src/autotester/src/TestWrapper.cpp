#include "TestWrapper.h"
#include "../../spa/src/Parser.h"
#include "../../spa/src/ParserPql.h"
#include "../../spa/src/TNode.h"
#include "../../spa/src/PKBBuilder.hpp"
#include <iostream>

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper *WrapperFactory::wrapper = 0;
AbstractWrapper *WrapperFactory::createWrapper() {
  if (wrapper == 0)
    wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

TestWrapper::TestWrapper() {
  // this creates a query evaluator with an empty pkb
  evaluator = QueryEvaluator();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  // ...rest of your code...
  try {
    // parse code to ast
    TNode ast = Parse(filename);
    // extract design
    PKBBuilder builder = PKBBuilder(ast);
    PKB pkb = builder.build();
    PKBQueryController controller = PKBQueryController(pkb);
    // save the pkb into the testwrapper
    this -> evaluator = QueryEvaluator(controller);
  } catch (std::string &e) {
    std::cerr << e << "\n";
    exit (EXIT_FAILURE);
  }

  return;
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
  // parse the query into query map
  QueryMap parsed_query = parse_pql(query);
  if (parsed_query.is_empty()) {
    std::cout << "Unable to parse PQL" << std::endl;
  }

  // evaluate the query
  auto evaluationResults = evaluator.QERunQuery(parsed_query);

  // set value of results list
  results = std::list<std::string> (evaluationResults.begin(), evaluationResults.end());

  // print results to stdout
  for (auto item: evaluationResults) {
    std::cout << item << " ";
  }
  std::cout << std::endl;

  return;
}
