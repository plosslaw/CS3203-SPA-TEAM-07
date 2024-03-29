#pragma once
#include <exception>
#include <string>
#include <unordered_set>
#include "ParserLib.h"
#include "TNode.h"

void validate_unique_procedure_names(TNode &root, std::unordered_set<std::string> &procs);
void validate_call_procedure_exists(TNode &root, std::unordered_set<std::string> &procs);

TNode program(State &s);
TNode procedure(State &s);
TNode stmtLst(State &s);
TNode stmt(State &s);
TNode read(State &s);
TNode print(State &s);
TNode call(State &s);
TNode while_stmt(State &s);
TNode if_stmt(State &s);
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