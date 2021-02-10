#include "QueryMap.h"

#include "catch.hpp"
using namespace std;
#include <iostream>

TEST_CASE("QueryMap Test 1") {
  QueryMap query;

  query.addItem(SELECT, PayLoad(SINGLE, SYNONYM, "s"));
  query.addItem(DECLARATION, PayLoad(SINGLE, STATEMENT, "s"));
  query.addItem(DECLARATION, PayLoad(SINGLE, VARIABLE, "v"));
  query.addItem(SUCHTHAT, PayLoad(PAIR, MODIFIES, "s,v"));
  query.addItem(PATTERN, PayLoad(PAIR, SYN_ASSIGN, "_,_"));

  REQUIRE(query.getList(SELECT)[0].getValue() == "s");
  REQUIRE(query.getList(SELECT)[0].getType().single == SYNONYM);

  REQUIRE(query.getList(DECLARATION)[0].getValue() == "s");
  REQUIRE(query.getList(DECLARATION)[0].getType().single == STATEMENT);

  REQUIRE(query.getList(DECLARATION)[1].getValue() == "v");
  REQUIRE(query.getList(DECLARATION)[1].getType().single == VARIABLE);

  REQUIRE(query.getList(SUCHTHAT)[0].getValue() == "s,v");
  REQUIRE(query.getList(SUCHTHAT)[0].getType().pair == MODIFIES);

  REQUIRE(query.getList(PATTERN)[0].getValue() == "_,_");
  REQUIRE(query.getList(PATTERN)[0].getType().pair == SYN_ASSIGN);
}
