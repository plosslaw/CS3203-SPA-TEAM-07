#pragma once
#include <exception>
#include <string>
#include <vector>
#include "ParserLib.h"
#include "TNode.h"

using namespace std;

string name(State &s);
TNode assign(State &s);
TNode cond_expr(State &s);
TNode rel_expr(State &s);
TNode rel_factor(State &s);
TNode expr(State &s);
TNode expr_1(State &s, TNode &lchild);
TNode term(State &s);
TNode term_1(State &s, TNode &lchild);
TNode factor(State &s);
TNode variable(State &s);
TNode constant(State &s);