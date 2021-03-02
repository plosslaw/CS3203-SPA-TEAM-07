#pragma once
#include "QueryMap.h"
#include "TNode.h"
#include <string>

TNode Parse(std::string);
QueryMap parse_pql(std::string query);
