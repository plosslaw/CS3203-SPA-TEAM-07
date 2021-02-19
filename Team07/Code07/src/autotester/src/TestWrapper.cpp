#include "TestWrapper.h"
#include "../../spa/src/Parser.h"
#include "../../spa/src/ParserPql.h"
#include "../../spa/src/TNode.h"
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

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  // ...rest of your code...
  try {
    TNode ast = Parse(filename);
    // parse
    std::cout << ast.toSexp() << "\n";
    // pass to pkb (placeholder)
  } catch (std::string &e) {
    std::cerr << e << "\n";
  }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...
  QueryMap parsed_query = parse_pql(query);
  if (parsed_query.is_empty()) {
    std::cout << "Unable to parse PQL" << std::endl;
  }
  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}
